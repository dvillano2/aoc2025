#include <stdio.h>
#include <string.h>

void count_doubles(const char *token, long *count) {
  int starting_length;
  long start = 0;
  long end = 0;
  int spot = 0;
  int scale = 1;
  // parsing
  while (token[spot] != '-') {
    start = start * 10 + (token[spot] - '0');
    spot++;
  }
  starting_length = spot;
  spot++;
  while (token[spot] != '\0') {
    end = end * 10 + (token[spot] - '0');
    spot++;
  }
  for (int i = 0; i < (starting_length + 1) / 2; i++) {
    scale *= 10;
  }

  long starting_half;
  if (starting_length % 2 == 1) {
    starting_half = scale / 10;
  } else {
    starting_half = start / scale;
  }

  long invalid = starting_half * scale + starting_half;
  while (invalid <= end) {
    if (start <= invalid) {
      *count += invalid;
    }
    starting_half++;
    if (starting_half >= scale) {
      scale *= 10;
    }
    invalid = starting_half * scale + starting_half;
  }
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  char buf[10000] = {0};
  int bytes_read = fread(buf, 1, sizeof(buf) - 1, fp);
  buf[bytes_read] = '\0';
  fclose(fp);
  long count = 0;
  const char *token;
  token = strtok(buf, ",\n\0");
  while (token != NULL) {
    count_doubles(token, &count);
    token = strtok(NULL, ",\n\0");
  }
  printf("answer is %ld\n", count);
  return 0;
}
