#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TILES 500

long av(long x) { return x < 0 ? -x : x; }

void parse_line(char line[], long buf[TILES][2], int spot) {
  const char *token = strtok(line, ",\n");
  buf[spot][0] = atol(token);
  token = strtok(NULL, ",\n");
  buf[spot][1] = atol(token);
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  long tiles[TILES][2] = {{0}};
  int spot = 0;
  for (char line[100]; fgets(line, 100, fp); spot++) {
    parse_line(line, tiles, spot);
  }
  fclose(fp);
  long answer = 0;
  for (int i = 0; i < TILES; i++) {
    if (tiles[i][0] == 0) {
      break;
    }
    for (int j = i + 1; j < TILES; j++) {
      if (tiles[j][0] == 0) {
        break;
      }
      long side_0 = 1 + av(tiles[i][0] - tiles[j][0]);
      long side_1 = 1 + av(tiles[i][1] - tiles[j][1]);
      long area = side_0 * side_1;
      answer = area > answer ? area : answer;
    }
  }
  printf("answer is %ld\n", answer);
  return 0;
}
