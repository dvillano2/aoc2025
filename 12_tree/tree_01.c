#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_line(char line[300], int size[2]) {
  if (line[0] == '\n' || line[1] == ':' || line[2] != 'x') {
    return -1;
  }
  const char *token = strtok(line, "x: \n");
  size[0] = atoi(token);
  token = strtok(NULL, "x: \n");
  size[1] = atoi(token);
  int area = 0;
  for (int i = 0; i < 6; i++) {
    token = strtok(NULL, "x: \n");
    area += atoi(token);
  }
  return area * 9;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int answer = 0;
  int size[2] = {0};
  for (char line[300]; fgets(line, 300, fp);) {
    int area = parse_line(line, size);
    if (area < size[0] * size[1]) {
      answer++;
    }
  }
  printf("answer is %d\n", answer);
  return 0;
}
