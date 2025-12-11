#include <stdio.h>

int max_jolt(const char line[]) {
  int answer = 0;
  int tens = *line - '0';
  for (int i = 1; line[i] != '\n'; i++) {
    int ones = line[i] - '0';
    int candidate = 10 * tens + ones;
    answer = candidate > answer ? candidate : answer;
    tens = ones > tens ? ones : tens;
  }
  return answer;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int joltage = 0;
  for (char line[300]; fgets(line, 300, fp);) {
    joltage += max_jolt(line);
  }
  fclose(fp);
  printf("answer if %d\n", joltage);
  return 0;
}
