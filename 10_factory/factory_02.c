#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct hashing_node {
  struct hashing_node *children[300];
};

int parse_line(char line[], int buttons[20][20], int target[20]) {
  strtok(line, " []()");
  char *token = strtok(NULL, " []()");
  int row = 0;
  while (*token != '{') {
    char c = *token;
    while (c != '\0') {
      if (isdigit(c)) {
        buttons[row][c - '0'] = 1;
      }
      token++;
      c = *token;
    }
    token = strtok(NULL, " []()");
    row++;
  }
  token++;

  int val = 0;
  int spot = 0;
  for (; *token != '\n'; token++) {
    if (*token == ',' || *token == '}') {
      target[spot] = val;
      spot++;
      val = 0;
    } else {
      val = 10 * val + (*token - '0');
    }
  }
  return row;
}

int check(const int candidate[20], const int target[20]) {
  for (int i = 0; i < 20; i++) {
    if (candidate[i] != target[i]) {
      return 1;
    }
  }
  return 0;
}

int too_big(const int candidate[20], const int target[20]) {
  for (int i = 0; i < 20; i++) {
    if (candidate[i] > target[i]) {
      return 1;
    }
  }
  return 0;
}

void push(int candidate[20], const int buttons[20][20], const int row) {
  for (int i = 0; i < 20; i++) {
    candidate[i] += buttons[row][i];
  }
}

int get_max_step(const int target[20], const int buttons[20][20],
                 const int state[20], int row) {
  int stepper = INT_MAX;
  for (int i = 0; i < 20; i++) {
    if (buttons[row][i] == 1) {
      stepper = target[i] - state[i] < stepper ? target[i] - state[i] : stepper;
    }
  }
  return stepper < INT_MAX ? stepper : 0;
}

int walk(int target[20], int buttons[20][20], int pushes, const int state[20],
         int rows, int row) {
  if (row >= rows) {
    return INT_MAX;
  }
  int new_state[20] = {0};
  int stepper = get_max_step(target, buttons, state, row);
  // printf("stepper is %02d row is %02d our of %02d\n state is ", stepper, row,
  //        rows);
  // for (int j = 0; j < 20; j++) {
  //   printf("%03d ", state[j]);
  // }
  // printf("\n target is");
  // for (int j = 0; j < 20; j++) {
  //   printf("%03d ", target[j]);
  // }
  // printf("\n\n ");

  for (; stepper >= 0; stepper--) {
    // int break_flag = 0;
    for (int j = 0; j < 20; j++) {
      new_state[j] = state[j] + stepper * buttons[row][j];
      // if (new_state[j] > target[j]) {
      //   break_flag = 1;
      //   break;
      // }
    }
    // if (break_flag == 1) {
    //   continue;
    // }
    if (check(new_state, target) == 0) {
      return pushes + stepper;
    }
    int later =
        walk(target, buttons, pushes + stepper, new_state, rows, row + 1);
    if (later < INT_MAX) {
      return later;
    }
  }
  return INT_MAX;
}

int comp_buttons(const void *a, const void *b) {
  const int *row0 = (int *)a;
  const int *row1 = (int *)b;
  int r0 = 0;
  int r1 = 0;
  for (int i = 0; i < 20; i++) {
    r0 += row0[i];
    r1 += row1[i];
  }
  if (r0 < r1) {
    return 1;
  }
  if (r0 > r1) {
    return -1;
  }
  return 0;
}

int main(void) {
  FILE *fp = fopen("small_input.txt", "r");
  int buttons[20][20];
  int target[20];
  int answer = 0;
  for (char line[300]; fgets(line, 300, fp);) {
    for (int i = 0; i < 20; i++) {
      for (int j = 0; j < 20; j++) {
        buttons[i][j] = 0;
      }
      target[i] = 0;
    }
    int rows = parse_line(line, buttons, target);
    qsort(buttons, 20, sizeof(buttons[0]), comp_buttons);
    int initial_state[20] = {0};
    // int subanswer = walk(target, buttons, 0, initial_state, rows, 0);
    // printf("subanswer is %d\n", subanswer);
    // answer += subanswer;

    // printf("rows: %d\n", rows);
    // for (int i = 0; i < 20; i++) {
    //   for (int j = 0; j < 20; j++) {
    //     printf("%d ", buttons[i][j]);
    //   }
    //   printf("\n");
    // }
    // printf("\n");

    int subanswer = walk(target, buttons, 0, initial_state, rows, 0);
    answer += subanswer;
    printf("subanswer is %d\n", subanswer);

    // for (int i = 0; i < 20; i++) {
    //   printf("%d ", target[i]);
    // }
    // printf("\n\n");
  }
  fclose(fp);
  printf("answer is %d\n", answer);
  return 0;
}
