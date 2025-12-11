#include <stdio.h>
#define BUF_SIZE 500 * 500
#define LOG 4

int get_num(char c) {
  if (c == ' ') {
    return 0;
  }
  return c - '0';
}

int get_vert(int spot, const char buf[], int width) {
  int runner = 0;
  int scale = 1;
  for (int i = 1; i < LOG + 1; i++) {
    int digit = get_num(buf[spot - i * width]);
    if (digit != 0) {
      runner += scale * digit;
      scale *= 10;
    }
  }
  return runner;
}

long get_block(int spot, const char buf[], int width) {
  long block = get_vert(spot, buf, width);
  int add_flag = buf[spot] == '+' ? 1 : 0;
  spot++;
  while (buf[spot] == ' ') {
    if (add_flag == 1) {
      block += get_vert(spot, buf, width);
    } else {
      long next = get_vert(spot, buf, width);
      if (next != 0) {
        block *= get_vert(spot, buf, width);
      }
    }
    spot++;
  }
  return block;
}

int main(void) {
  char buf[BUF_SIZE] = {0};
  FILE *fp = fopen("input.txt", "r");
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
  int width = 0;
  while (buf[width] != '\n') {
    width++;
  }
  width++;
  long answer = 0;
  for (int spot = LOG * width; buf[spot] != '\n'; spot++) {
    if (buf[spot] == ' ') {
      continue;
    }
    answer += get_block(spot, buf, width);
  }
  printf("answer is %ld\n", answer);
  return 0;
}
