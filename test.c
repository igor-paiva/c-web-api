#include <stdio.h>
#include <malloc.h>
#include <string.h>

int main(void) {
  int * v;

  v = (int *) malloc (100 * sizeof(int));

  for (int i = 0; i < 10; i++) {
    v[i] = i + 1;
  }

  for (int i = 0; i < 10; i++) {
    printf("%d\n", v[i]);
  }

  v = (int *) realloc (v, 50 * sizeof(int));

  for (int i = 0; i < 10; i++) {
    printf("%d\n", v[i]);
  }

  // struct test {
  //   char name[50];
  //   char value[50];
  // };

  // struct test teste1;
  // struct test teste2;
  // char input[255] = "/person?name=Igor&age=22";

  // char * start_position = strchr(input, '?') + sizeof(char);

  // char format[102];

  // // "%50[^=]=%50[^&]"
  // sprintf(format, "%c%d[^=]=%c%d[^& ;]", '%', 50, '%', 50);


  // // sscanf(start_position, "%50[^=]", teste.name);

  // // sscanf(start_position + (4 * sizeof(char)), "=%50[^&]", teste.value);

  // sscanf(start_position, format, teste1.name, teste1.value);

  // start_position = strchr(input, '&') + sizeof(char);

  // sscanf(start_position, format, teste2.name, teste2.value);

  // printf("%s: %s\n", teste1.name, teste1.value);

  // printf("%s: %s\n", teste2.name, teste2.value);

  return 0;
}