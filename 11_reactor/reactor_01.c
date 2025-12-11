#include <stdio.h>
#include <string.h>
#define DEVICES 26 * 26 * 26

int name_to_int(const char *token) {
  int val = 0;
  for (int i = 0; i < 3; i++) {
    val = val * 26 + token[i] - 'a';
  }
  return val;
}

void parse_line(char line[300], int children[DEVICES][30]) {
  const char *token = strtok(line, " :\n");
  int parent = name_to_int(token);
  int i = 0;
  token = strtok(NULL, " :\n");
  while (token != NULL) {
    children[parent][i] = name_to_int(token);
    i++;
    token = strtok(NULL, " :\n");
  }
}

int walk(int start, int end, int children[DEVICES][30]) {
  if (start == end) {
    return 1;
  }
  int answer = 0;
  for (int i = 0; children[start][i] >= 0; i++) {
    int child = children[start][i];
    if (children[child][29] == -1) {
      int paths = walk(child, end, children);
      children[child][29] = paths;
    }
    answer += children[child][29];
  }
  return answer;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int children[DEVICES][30];
  for (int i = 0; i < DEVICES; i++) {
    for (int j = 0; j < 30; j++) {
      children[i][j] = -1;
    }
  }
  for (char line[300]; fgets(line, 300, fp);) {
    parse_line(line, children);
  }
  fclose(fp);
  int start = name_to_int("you");
  int end = name_to_int("out");
  int answer = walk(start, end, children);
  printf("answer is %d\n", answer);
  return 0;
}
