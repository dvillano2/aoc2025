#include <limits.h>
#include <stdio.h>
#include <string.h>

int parse_line(char line[300], int buttons[20][20], int target[20]) {
  int button = 0;
  const char *token = strtok(line, " []().#");
  for (; *token != '{'; token = strtok(NULL, " []().#"), button++) {
    int button_spot;
    for (int i = 0;; i++) {
      if (token[i] == ',' || token[i] == '\0') {
        buttons[button][button_spot] = 1;
        if (token[i] == '\0') {
          break;
        }
      } else {
        button_spot = token[i] - '0';
      }
    }
  }
  int value = 0;
  int target_spot = 0;
  for (int i = 1;; i++) {
    if (token[i] == ',' || token[i] == '}') {
      target[target_spot] = value;
      value = 0;
      target_spot++;
      if (token[i] == '}') {
        break;
      }
    } else {
      value = 10 * value + token[i] - '0';
    }
  }
  return button;
}

void make_parity_target(int parity_target[20], const int target[20]) {
  for (int i = 0; i < 20; i++) {
    parity_target[i] = target[i] % 2;
  }
}

void make_tracker(int tracker[20], int mask, int buttons[20][20],
                  int button_count) {
  for (int j = 0; j < button_count; j++) {
    for (int k = 0; k < 20; k++) {
      tracker[k] += buttons[j][k] * (mask >> j & 1);
    }
  }
}

int check(const int parity_target[20], const int tracker[20]) {
  for (int j = 0; j < 20; j++) {
    if (parity_target[j] != tracker[j] % 2) {
      return 0;
    }
  }
  return 1;
}

int get_buttons_pushed(int mask) {
  int buttons_pushed = 0;
  while (mask > 0) {
    buttons_pushed += mask & 1;
    mask >>= 1;
  }
  return buttons_pushed;
}

void make_new_target(const int target[20], int new_target[20],
                     const int tracker[20]) {
  for (int j = 0; j < 20; j++) {
    new_target[j] = (target[j] - tracker[j]) / 2;
  }
}

int check_neg(const int target[20]) {
  for (int i = 0; i < 20; i++) {
    if (target[i] < 0) {
      return 1;
    }
  }
  return 0;
}

int check_zero(const int target[20]) {
  int c = 0;
  for (int i = 0; i < 20; i++) {
    c += target[i];
  }
  return c;
}

int walk(int button_count, int buttons[20][20], int target[20]) {
  if (check_neg(target) == 1) {
    return INT_MAX;
  }
  if (check_zero(target) == 0) {
    return 0;
  }
  int answer = INT_MAX;
  int parity_target[20] = {0};
  make_parity_target(parity_target, target);
  for (int mask = 0; mask < 1 << button_count; mask++) {
    int tracker[20] = {0};
    make_tracker(tracker, mask, buttons, button_count);
    int match = check(parity_target, tracker);
    if (match == 1) {
      int buttons_pushed = get_buttons_pushed(mask);
      int new_target[20];
      make_new_target(target, new_target, tracker);
      int lower = walk(button_count, buttons, new_target);
      if (lower != INT_MAX) {
        int smaller = buttons_pushed + 2 * lower;
        answer = smaller < answer ? smaller : answer;
      }
    }
  }
  return answer;
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  int answer = 0;
  for (char line[300]; fgets(line, 300, fp);) {
    int buttons[20][20] = {{0}};
    int target[20] = {0};
    int button_count = parse_line(line, buttons, target);
    int subanswer = walk(button_count, buttons, target);
    answer += subanswer;
  }
  fclose(fp);
  printf("answer is %d\n", answer);
  return 0;
}
