#include <stdio.h>
#include <string.h>

int detect_subpattern(long pattern) {
  if (pattern < 11) {
    return 0;
  }
  long scale = 1;
  while (scale < pattern) {
    scale *= 10;
  }
  scale /= 10;
  long subpattern = pattern / scale;
  long matcher = subpattern;
  long jumper = 10;
  while (scale > 1) {
    while (matcher <= pattern) {
      if (matcher == pattern) {
        return 1;
      }
      matcher = matcher * jumper + subpattern;
    }
    scale /= 10;
    jumper *= 10;
    subpattern = pattern / scale;
    matcher = subpattern;
  }
  return 0;
}

void count_doubles(const char *token, long *count) {
  long start = 0;
  long end = 0;
  int spot = 0;
  while (token[spot] != '-') {
    start = start * 10 + (token[spot] - '0');
    spot++;
  }
  spot++;
  while (token[spot] != '\0') {
    end = end * 10 + (token[spot] - '0');
    spot++;
  }

  long pattern = 1;
  long jumper = 10;
  long invalid = pattern;
  while (invalid <= end) {
    while (invalid <= end) {
      if (start <= invalid && invalid > 9) {
        *count += invalid;
      }
      invalid = invalid * jumper + pattern;
    }
    pattern++;
    if (detect_subpattern(pattern) == 1) {
      pattern++;
    }
    if (pattern == jumper) {
      jumper *= 10;
    }
    invalid = jumper > 10 ? pattern * jumper + pattern : pattern;
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
