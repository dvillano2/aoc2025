#include <stdio.h>
#define BUF_SIZE 200 * 200

int get_width(const char buf[]) {
  int i = 1;
  while (buf[i] != '\n') {
    i++;
  }
  return i + 1;
}

int access(int i, char buf[], int width) {
  if (buf[i] == '.' || buf[i] == '\n') {
    return 0;
  }
  int count = 0;
  if (i / width > 0) {
    count += buf[i - width - 1] == '@';
    count += buf[i - width] == '@';
    count += buf[i - width + 1] == '@';
  }
  count += buf[i - 1] == '@';
  count += buf[i + 1] == '@';
  count += buf[i + width - 1] == '@';
  count += buf[i + width] == '@';
  count += buf[i + width + 1] == '@';
  if (count < 4) {
    buf[i] = '.';
    return 1;
  }
  return 0;
}

int main(void) {
  char buf[BUF_SIZE] = {0};
  FILE *fp = fopen("input.txt", "r");
  size_t bytes_read = fread(buf, 1, BUF_SIZE - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
  const int width = get_width(buf);
  int count = 0;
  while (1) {
    int subcount = 0;
    for (int i = 0; buf[i] != '\0'; i++) {
      subcount += access(i, buf, width);
    }
    if (subcount == 0) {
      break;
    }
    count += subcount;
  }
  printf("answer is %d\n", count);
  return 0;
}
