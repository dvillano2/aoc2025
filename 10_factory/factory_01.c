#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>

int parse_line(char line[], int buttons[]) {
  char *token = strtok(line, " []()");
  int scale = 1;
  int target = 0;
  for (; *token != '\0'; token++, scale <<= 1) {
    if (*token == '#') {
      target += scale;
    }
  }
  int spot = 0;
  token = strtok(NULL, " []()");
  while (*token != '{') {
    char c = *token;
    while (c != '\0') {
      if (isdigit(c)) {
        buttons[spot] += 1 << (c - '0');
      }
      token++;
      c = *token;
    }
    token = strtok(NULL, " []()");
    spot++;
  }
  return target;
}

int walk(int target, int pushes, int prefix, int buttons[], int spot) {
  if (buttons[spot] == 0) {
    return INT_MAX;
  }
  int update = prefix ^ buttons[spot];
  if (update == target) {
    return pushes + 1;
  }
  int with = walk(target, pushes + 1, update, buttons, spot + 1);
  int without = walk(target, pushes, prefix, buttons, spot + 1);
  return with < without ? with : without;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int buttons[20];
  int answer = 0;
  for (char line[300]; fgets(line, 300, fp);) {
    for (int i = 0; i < 20; i++) {
      buttons[i] = 0;
    }
    int target = parse_line(line, buttons);
    int subanswer = walk(target, 0, 0, buttons, 0);
    answer += subanswer;
  }
  fclose(fp);
  printf("final answer is %d\n", answer);
  return 0;
}
