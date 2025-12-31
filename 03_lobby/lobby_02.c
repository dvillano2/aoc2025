#include <stdio.h>

long replace(long current, int val) {
  current *= 10;
  current += val;
  long scale = 1;
  for (int i = 0; i < 12; i++) {
    scale *= 10;
  }
  while (scale >= 10) {
    int left = (current / scale) % 10;
    int right = (current / (scale / 10)) % 10;
    if (left < right) {
      long remain = current % scale;
      current = ((current / (scale * 10)) * scale) + remain;
      return current;
    }
    scale /= 10;
  }
  return current / 10;
}

long max_jolt(const char line[]) {
  long current = 0;
  for (int i = 0; i < 12; i++, line++) {
    current = current * 10 + (*line - '0');
  }
  for (; *line != '\n'; line++) {
    current = replace(current, *line - '0');
  }
  return current;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  long joltage = 0;
  for (char line[300]; fgets(line, 300, fp);) {
    joltage += max_jolt(line);
  }
  fclose(fp);
  printf("answer is %ld\n", joltage);
  return 0;
}
