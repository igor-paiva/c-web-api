#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <pthread.h>
#include <semaphore.h>

#define QLEN 10
#define TRUE 1
#define FALSE 0
#define MAX_THREADS 10
#define MAX_REQUEST_MSG 16384

typedef struct ThreadParam {
  int accept_sd;
  struct sockaddr_in client_addr;
} ThreadParam;

sem_t running_threads;

void handle_critical_failure(int rt, char * message) {
  if (rt < 0) {
    fprintf(stdout, "%s\n", message);
    exit(1);
  }
}

void set_server_addr(struct sockaddr_in * serv_addr, char * addr, char * port) {
  int errcode;
  struct sockaddr_in * temp_addr;
  struct addrinfo hints, * result;

  memset(&hints, 0, sizeof (hints));

  hints.ai_family = PF_UNSPEC;
  hints.ai_flags |= AI_CANONNAME;
  hints.ai_socktype = SOCK_STREAM;

  errcode = getaddrinfo(addr, NULL, &hints, &result);

  if (errcode != 0) {
    perror("getaddrinfo");
    return;
  }

  serv_addr->sin_family = AF_INET;
  temp_addr = (struct sockaddr_in *) result->ai_addr;
  serv_addr->sin_addr = temp_addr->sin_addr; 
  serv_addr->sin_port = htons(atoi(port));
}

void send_response(int descriptor, char * response_msg) {
  send(descriptor, response_msg, MAX_REQUEST_MSG, 0);
}

void handle_request(char * request, int descriptor) {
  char * bufout;

  bufout = (char *) malloc(MAX_REQUEST_MSG * sizeof(char));

  sprintf(bufout, "Received: %s\n", request);

  send_response(descriptor, bufout);
}

void * answer_client(void * param) {
  char * bufin;
  ThreadParam * param_struct = (ThreadParam *) param;

  bufin = (char *) malloc(MAX_REQUEST_MSG * sizeof(char));

  if (bufin) {
    int rec_bytes;
    memset(bufin, 0x0, MAX_REQUEST_MSG * sizeof(char));

    rec_bytes = recv(param_struct->accept_sd, bufin, MAX_REQUEST_MSG, 0);

    if (rec_bytes >= 0) {
      fprintf(stdout, "\nBufin:\n%s\n", bufin);

      handle_request(bufin, param_struct->accept_sd);
    }

    free(bufin);
  } else {
    fprintf(stdout, "\nFail to allocate memory\n");
    send_response(param_struct->accept_sd , "Fail to allocate memory\n");
  }

  free(param);
  close(param_struct->accept_sd);
  sem_post(&running_threads);
  pthread_exit(NULL);
}

int main(int argc, char * argv[]) {
  struct sockaddr_in server_addr;
  int sd, bind_res, listen_res;

  if (argc < 3) {
    printf("You must provide 2 arguments: the server address and port \n");
    exit(0);
  }

  memset((char *) &server_addr, 0, sizeof(server_addr));

  set_server_addr(&server_addr, argv[1], argv[2]); 

  sd = socket(AF_INET, SOCK_STREAM, 0);
  
  handle_critical_failure(sd, "Fail to create the socket\n");

  bind_res = bind(sd, (struct sockaddr *) &server_addr, sizeof(server_addr));

  handle_critical_failure(bind_res, "Bind fail\n");

  listen_res = listen(sd, QLEN);

  handle_critical_failure(listen_res, "Fail to listen on socket\n");

  sem_init(&running_threads, 0, MAX_THREADS);

  fprintf(stdout, "Server listening in %s:%s...\n\n", argv[1], argv[2]);

  while (TRUE) {
    pthread_t tid;
    int accept_sd;
    socklen_t addr_len;
    ThreadParam * param;
    struct sockaddr_in client_addr;
    
    addr_len = sizeof(client_addr);
    accept_sd = accept(sd, (struct sockaddr *) &client_addr, &addr_len);

    if (accept_sd < 0) {
      fprintf(stdout, "\nAccept fail\n");
      continue;
    }

    param = (ThreadParam *) malloc (sizeof(ThreadParam));

    param->accept_sd = accept_sd;
    param->client_addr = client_addr;

    sem_wait(&running_threads);

    pthread_create(&tid, 0, (void * (*)(void *)) answer_client, (void *) param);
  }

  return 0;
}
