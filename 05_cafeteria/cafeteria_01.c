#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  long ranges[400] = {0};
  char line[100];
  const char *token;
  for (int i = 0; fgets(line, 99, fp); i += 2) {
    if (line[0] == '\n') {
      break;
    }
    token = strtok(line, " -\n");
    ranges[i] = atol(token);
    token = strtok(NULL, " -\n");
    ranges[i + 1] = atol(token);
  }

  int answer = 0;
  for (; fgets(line, 99, fp);) {
    token = strtok(line, " -\n");
    long search = atol(token);
    for (int i = 0; ranges[i] != 0; i += 2) {
      if (ranges[i] <= search && search <= ranges[i + 1]) {
        answer++;
        break;
      }
    }
  }
  fclose(fp);
  printf("answer is %d\n", answer);
  return 0;
}
