#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BOXES 1000
#define CONNECTIONS 1000

void parse_line(char line[], int buf[BOXES][3], int spot) {
  const char *token = strtok(line, ",\n");
  buf[spot][0] = atoi(token);
  token = strtok(NULL, ",\n");
  buf[spot][1] = atoi(token);
  token = strtok(NULL, ",\n");
  buf[spot][2] = atoi(token);
}

void heap_push(long heap[CONNECTIONS][3], const long connection[3]) {
  if (connection[2] < heap[0][2]) {
    for (int i = 0; i < 3; i++) {
      heap[0][i] = connection[i];
    }
    int spot = 0;
    while (1) {
      int child_0 = 2 * spot + 1;
      if (child_0 >= CONNECTIONS) {
        break;
      }
      int child_1 = child_0 + 1;
      int child;
      if (child_1 >= CONNECTIONS) {
        child = child_0;
      } else {
        child = heap[child_0][2] > heap[child_1][2] ? child_0 : child_1;
      }
      if (heap[spot][2] >= heap[child][2]) {
        break;
      }
      const long tmp[3] = {heap[spot][0], heap[spot][1], heap[spot][2]};
      for (int i = 0; i < 3; i++) {
        heap[spot][i] = heap[child][i];
      }
      for (int i = 0; i < 3; i++) {
        heap[child][i] = tmp[i];
      }
      spot = child;
    }
  }
}

int compare(const void *a, const void *b) {
  const long *d0 = (const long *)a;
  const long *d1 = (const long *)b;

  if (*(d0 + 2) < *(d1 + 2)) {
    return -1;
  }
  if (*(d0 + 2) > *(d1 + 2)) {
    return 1;
  }
  return 0;
}

long dist(const int box_0[3], const int box_1[3]) {
  long d = 0;
  for (int i = 0; i < 3; i++) {
    long coord = box_0[i] - box_1[i];
    d += coord * coord;
  }
  return d;
}

long find(long parents[BOXES], long x) {
  if (parents[x] != x) {
    parents[x] = find(parents, parents[x]);
  }
  return parents[x];
}

int merge(long parents[BOXES], int sizes[BOXES], long x, long y) {
  long px = find(parents, x);
  long py = find(parents, y);
  if (px != py) {
    parents[px] = parents[py];
    sizes[py] += sizes[px];
  }
  return sizes[py];
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int boxes[BOXES][3] = {{0}};
  int spot = 0;
  for (char line[100]; fgets(line, 100, fp); spot++) {
    parse_line(line, boxes, spot);
  }
  fclose(fp);

  long parents[BOXES];
  int sizes[BOXES];
  for (int i = 0; i < BOXES; i++) {
    parents[i] = i;
    sizes[i] = 1;
  }

  long heap[CONNECTIONS][3];
  while (1) {
    for (int i = 0; i < CONNECTIONS; i++) {
      for (int j = 0; j < 3; j++) {
        heap[i][j] = LONG_MAX;
      }
    }
    for (long i = 0; i < BOXES; i++) {
      for (long j = i + 1; j < BOXES; j++) {
        if (find(parents, i) != find(parents, j)) {
          long connection[3] = {i, j, dist(boxes[i], boxes[j])};
          heap_push(heap, connection);
        }
      }
    }
    qsort(heap, CONNECTIONS, sizeof(heap[0]), compare);

    int break_flag = 0;
    for (int i = 0; i < CONNECTIONS; i++) {
      int new_size = merge(parents, sizes, heap[i][0], heap[i][1]);
      if (new_size == BOXES) {
        int num0 = (int)heap[i][0];
        int num1 = (int)heap[i][1];
        long coord0 = (long)boxes[num0][0];
        long coord1 = (long)boxes[num1][0];
        long product = coord0 * coord1;
        printf("answer is %ld\n", product);
        break_flag = 1;
        break;
      }
    }
    if (break_flag == 1) {
      break;
    }
  }

  return 0;
}
