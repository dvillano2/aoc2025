#include <stdio.h>

void rotate(char line[], int *pos, int *count) {
  int neg = *line == 'L' ? -1 : 1;
  int val = 0;
  line++;
  while (*line != '\n') {
    val = 10 * val + (*line - '0');
    line++;
  }
  val *= neg;
  *pos += val;
  *pos %= 100;
  if (*pos < 0) {
    *pos += 100;
  }
  *count += *pos == 0;
}

int main(void) {
  int pos = 50;
  int count = 0;
  FILE *fp = fopen("input.txt", "r");
  for (char line[20]; fgets(line, sizeof(line), fp);) {
    rotate(line, &pos, &count);
  }
  fclose(fp);
  printf("answer is %d\n", count);
  return 0;
}
