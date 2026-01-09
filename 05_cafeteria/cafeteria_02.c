#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_ranges(const void *r0, const void *r1) {
  const long (*range0)[2] = r0;
  const long (*range1)[2] = r1;
  if ((*range0)[1] < (*range1)[1]) {
    return 1;
  }
  if ((*range0)[1] > (*range1)[1]) {
    return -1;
  }
  if ((*range0)[0] < (*range1)[0]) {
    return 1;
  }
  if ((*range0)[0] > (*range1)[0]) {
    return -1;
  }
  return 0;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  long ranges[200][2] = {{0}};
  char line[100];
  for (int i = 0; fgets(line, 99, fp); i++) {
    if (line[0] == '\n') {
      break;
    }
    const char *token = strtok(line, " -\n");
    ranges[i][0] = atol(token);
    token = strtok(NULL, " -\n");
    ranges[i][1] = atol(token);
  }
  fclose(fp);
  qsort(ranges, 200, sizeof(ranges[0]), compare_ranges);
  int low = 0;
  int high = 0;
  long answer = 0;
  while (ranges[low][0] != 0) {
    long floor = ranges[low][0];
    while (ranges[high + 1][1] >= floor) {
      high++;
      floor = floor < ranges[high][0] ? floor : ranges[high][0];
    }
    answer += ranges[low][1] - floor + 1;
    high++;
    low = high;
  }
  printf("answer is %ld\n", answer);
  return 0;
}
