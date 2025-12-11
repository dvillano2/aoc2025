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
  long tracker[BUF_SIZE] = {0};
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
  long answer = 0;
  int width = get_width(buf);
  for (int spot = width; buf[spot + 1] != '\0'; spot++) {
    if (buf[spot] == '\n') {
      answer = 0;
      continue;
    }
    if (buf[spot - width] == 'S') {
      buf[spot] = '|';
      tracker[spot] = 1;
    }
    if (buf[spot - width] == '|') {
      if (buf[spot] == '^') {
        buf[spot - 1] = '|';
        buf[spot + 1] = '|';
        tracker[spot - 1] += tracker[spot - width];
        tracker[spot + 1] += tracker[spot - width];
      } else {
        buf[spot] = '|';
        tracker[spot] += tracker[spot - width];
      }
    }
    // only works because there's a final line of dots
    answer += tracker[spot];
  }
  printf("answer is %ld\n", answer);
  return 0;
}
