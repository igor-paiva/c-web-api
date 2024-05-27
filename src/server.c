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

#include "http.h"
#include "routes.h"
#include "db_config.h"

#define QLEN 10
#define MAX_THREADS 10
#define MAX_REQUEST_HEADERS 25
#define MAX_RESPONSE_HEADERS 10
#define MAX_NO_CONTENT_ERROR_REQ_MSG 1000
#define SERVER_NAME "Igor C Server"

typedef struct ThreadParam {
  int accept_sd;
  struct sockaddr_in client_addr;
} ThreadParam;

sem_t running_threads;

int tables_count;
TableDefinition * tables_config[MAX_TABLES];

int res_headers_count = 0;
HttpHeader response_headers[MAX_RESPONSE_HEADERS];

/* const char * ALLOWED_HOSTS[] = { "localhost:3000" }; */

void handle_critical_failure(int rt, char * message) {
  if (rt < 0) {
    fprintf(stdout, "%s\n", message);
    exit(1);
  }
}

Route * find_route(char * method, char * path) {
  int i;

  for (i = 0; i < MAX_ROUTES; i++) {
    if (routes[i] == NULL) break;

    boolean path_cmp = FALSE;
    boolean method_cmp = strncmp(routes[i]->method, method, HTTP_METHOD_LENGTH) == 0;

    if (strchr(routes[i]->path, ':')) {
      int reti;
      regex_t regex;

      reti = regcomp(&regex, routes[i]->regex, 0);

      if (reti) {
        fprintf(stdout, "\nFail to compile Regex\n");
        continue;
      }

      reti = regexec(&regex, path, 0, NULL, 0);

      if (!reti) {
        path_cmp = TRUE;
      } else if (reti == REG_NOMATCH) {
        path_cmp = FALSE;
      } else {
        char msgbuf[100];

        regerror(reti, &regex, msgbuf, sizeof(msgbuf));

        fprintf(stdout, "\nRegex match failed: %s\n", msgbuf);
      }
    } else {
      path_cmp = strncmp(routes[i]->path, path, ROUTE_PATH_LENGTH) == 0;
    }

    if (method_cmp && path_cmp)
      return routes[i];
  }

  return NULL;
}

void load_database_config() {
  load_database_info(tables_config, &tables_count);

  init_mutexes(tables_config, &tables_count);
}

void set_response_headers() {
  HttpHeader server_name = { .name = "Server" };

  strncpy(server_name.value, SERVER_NAME, HTTP_HEADER_VALUE_LENGTH);

  HttpHeader conection = { .name = "Connection", .value = "close" };
  HttpHeader content_type = { .name = "Content-Type", .value = "application/json" };

  response_headers[res_headers_count++] = conection;
  response_headers[res_headers_count++] = server_name;
  response_headers[res_headers_count++] = content_type;
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

void add_response_headers(char * response_msg) {
  int i;

  for (i = 0; i < res_headers_count; i++) {
    char * response_msg_tmp = response_msg;

    // should I copy?
    // memccpy(response_msg_tmp, response_msg, '\0', MAX_REQUEST_DATA * sizeof(char));

    sprintf(
      response_msg,
      "%s%s: %s\n",
      response_msg_tmp,
      response_headers[i].name,
      response_headers[i].value
    );
  }
}

void send_response(
  int descriptor,
  char * response_msg,
  char * content,
  size_t content_length,
  const HttpStatus * status
) {
  char * init_line;
  char * response_msg_tmp = response_msg;

  // should I copy?
  // memccpy(response_msg_tmp, response_msg, '\0', MAX_REQUEST_DATA * sizeof(char));

  init_line = mount_initial_line(status);

  /* add HTTP initial line to request response */
  sprintf(response_msg, "%s", init_line);

  /* add server HTTP response headers to the response */
  add_response_headers(response_msg);

  /* add content length and content to the response */
  sprintf(
    response_msg,
    "%sContent-Length: %ld\n\n%s",
    response_msg_tmp,
    content_length,
    content
  );

  send(descriptor, response_msg, MAX_REQUEST_DATA, 0);

  free(init_line);
}

void print_req_result(char * method, char * path, const HttpStatus * status) {
  fprintf(stdout, "\n%s %s => %d %s\n", method, path, status->status, status->name);
}

void print_receive_req(char * method, char * path) {
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);

  fprintf(
    stdout,
    "%s %s => %02d/%02d/%d  %02d:%02d:%02d %ld GMT\n",
    method,
    path,
    tm.tm_mon + 1,
    tm.tm_mday,
    tm.tm_year + 1900,
    tm.tm_hour,
    tm.tm_min,
    tm.tm_sec,
    tm.tm_gmtoff / 3600
  );
}

void handle_route_found(
  int descriptor,
  Route * route,
  char * bufout,
  char * request_path,
  char * request
) {
  boolean has_body;
  const HttpStatus * status;
  HttpHeaderList headers = { .list = NULL };
  QueryParamList query_params = { .count = 0 };
  char * received_content = NULL, * response_data = NULL;

  has_body = req_has_body(route->method);

  response_data = (char *) malloc (MAX_REQUEST_DATA * sizeof(char));

  headers.list = (HttpHeader *) malloc(MAX_REQUEST_HEADERS * sizeof(HttpHeader));

  if (response_data == NULL || headers.list == NULL) {
    send_response(descriptor, bufout, NULL, 0, &HTTP_INTERNAL_SERVER_ERROR);
    goto end;
  }

  memset(response_data, 0x0, MAX_REQUEST_DATA * sizeof(char));

  if (has_body) {
    received_content = get_content(request);
  }

  if (strchr(route->path, '?') != NULL) {
    state op_state = get_query_params(request_path, &query_params.count, query_params.list);

    if (is_error(op_state)) {
      send_response(descriptor, bufout, NULL, 0, error_to_http(op_state));
      goto end;
    }
  }

  ControllerActionParam controller_param = {
    .request_data = received_content,
    .request_response = response_data,
    .params_list = &query_params
  };

  status = (*route->handler)(&controller_param);

  send_response(
    descriptor,
    bufout,
    response_data,
    strlen(response_data) * sizeof(char),
    status
  );

end:
  print_req_result(route->method, request_path, status);

  if (has_body) free(received_content);
  free(response_data);
  free(headers.list);
  free(query_params.list);
}

void handle_request(char * request, int descriptor) {
  Route * route;
  char * bufout;
  char path[ROUTE_PATH_LENGTH];
  char method[HTTP_METHOD_LENGTH];
  char req_first_line[HTTP_FIRST_LINE_LENGTH];

  /* to not modify the request string */
  memccpy(req_first_line, request, '\n', HTTP_FIRST_LINE_LENGTH * sizeof(char));

  strcpy(method, strtok(req_first_line, " "));
  strcpy(path, strtok(NULL, " "));

  route = find_route(method, path);

  /* logging received requests in the console */
  print_receive_req(method, path);

  bufout = (char *) malloc(MAX_REQUEST_DATA * sizeof(char));

  if (bufout == NULL)
    send_response(descriptor, bufout, NULL, 0, &HTTP_INTERNAL_SERVER_ERROR);

  memset(bufout, 0x0, MAX_REQUEST_DATA * sizeof(char));

  if (route) {
    handle_route_found(descriptor, route, bufout, path, request);
  } else {
    print_req_result(method, path, &HTTP_NOT_FOUND);

    send_response(descriptor, bufout, NULL, 0, &HTTP_NOT_FOUND);
  }

  free(bufout);
}

void * answer_client(void * param) {
  char * bufin;
  ThreadParam * param_struct = (ThreadParam *) param;

  bufin = (char *) malloc(MAX_REQUEST_DATA * sizeof(char));

  if (bufin) {
    int rec_bytes;
    memset(bufin, 0x0, MAX_REQUEST_DATA * sizeof(char));

    rec_bytes = recv(param_struct->accept_sd, bufin, MAX_REQUEST_DATA, 0);

    if (rec_bytes >= 0) {
      /* to remove later */
      fprintf(stdout, "\nReceived request:\n%s\n\n", bufin);

      handle_request(bufin, param_struct->accept_sd);
    }

    free(bufin);
  } else {
    fprintf(stdout, "\nFail to allocate memory\n");
    send_response(param_struct->accept_sd, bufin, NULL, 0, &HTTP_INTERNAL_SERVER_ERROR);
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

  set_response_headers();

  load_database_config();

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
      fprintf(
        stdout,
        "\n%s:%d: Accept fail\n",
        inet_ntoa(client_addr.sin_addr),
        ntohs(client_addr.sin_port)
      );
      continue;
    }

    param = (ThreadParam *) malloc (sizeof(ThreadParam));

    if (param == NULL) {
      char bufout[MAX_NO_CONTENT_ERROR_REQ_MSG];
      send_response(accept_sd, bufout, NULL, 0, &HTTP_INTERNAL_SERVER_ERROR);
      continue;
    }

    param->accept_sd = accept_sd;
    param->client_addr = client_addr;

    sem_wait(&running_threads);

    pthread_create(&tid, 0, (void * (*)(void *)) answer_client, (void *) param);
  }

  destroy_mutexes(tables_config, &tables_count);

  return 0;
}
