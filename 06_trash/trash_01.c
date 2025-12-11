#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUF_SIZE 100 * 100

int parse_line(char line[], int buf[], int spot) {
  const char *token = strtok(line, " \n\0");
  for (; token != NULL; spot++) {
    buf[spot] = atoi(token);
    token = strtok(NULL, " \n\0");
  }
  return spot + 1;
}

int main(void) {
  int buf[BUF_SIZE] = {0};
  int spot = 0;
  int width = 0;
  FILE *fp = fopen("input.txt", "r");
  char line[BUF_SIZE];
  for (; fgets(line, sizeof(line), fp);) {
    if (line[0] == '*' || line[0] == '+') {
      break;
    }
    spot = parse_line(line, buf, spot);
    width = width == 0 ? spot : width;
    memset(line, '\0', sizeof(line));
  }

  long answer = 0;
  int index = -1;
  for (int i = 0; line[i] != '\n'; i++) {
    if (line[i] == ' ') {
      continue;
    }
    index++;
    long runner;
    if (line[i] == '+') {
      runner = 0;
      for (int j = index; buf[j] != 0; j += width) {
        runner += buf[j];
      }
    } else {
      runner = 1;
      for (int j = index; buf[j] != 0; j += width) {
        runner *= buf[j];
      }
    }
    answer += runner;
  }
  fclose(fp);
  printf("answer is %ld\n", answer);
  return 0;
}
