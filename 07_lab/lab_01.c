#include <stdio.h>
#define BUF_SIZE 400 * 400

int get_width(const char buf[]) {
  int width = 0;
  while (buf[width] != '\n') {
    width++;
  }
  return width + 1;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  char buf[BUF_SIZE] = {0};
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
  int answer = 0;
  int width = get_width(buf);
  for (int spot = width; buf[spot] != '\0'; spot++) {
    if (buf[spot - width] == 'S' || buf[spot - width] == '|') {
      if (buf[spot] == '^') {
        buf[spot - 1] = '|';
        buf[spot + 1] = '|';
        answer++;
      } else {
        buf[spot] = '|';
      }
    }
  }
  printf("answer is %d\n", answer);
  return 0;
}
