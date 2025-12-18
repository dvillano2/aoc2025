#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define POINTS 496

enum Direction { UP, LEFT, DOWN, RIGHT, NUM_DIRECTIONS };

enum CornerDirection {
  UP_LEFT,
  DOWN_LEFT,
  DOWN_RIGHT,
  UP_RIGHT,
  NUM_CORNER_DIRECTIONS
};

enum Direction turn_left(enum Direction current) {
  return (current + 1) % NUM_DIRECTIONS;
}

enum Direction opposite(enum Direction current) {
  return (current + 2) % NUM_DIRECTIONS;
}

enum Direction turn_right(enum Direction current) {
  return (current + 3) % NUM_DIRECTIONS;
}

enum Direction get_next_dir(const long current_point[2],
                            const long next_point[2]) {
  if (current_point[0] == next_point[0]) {
    if (current_point[1] < next_point[1]) {
      return DOWN;
    }
    return UP;
  }
  if (current_point[0] < next_point[0]) {
    return RIGHT;
  }
  return LEFT;
}

enum Direction get_next_normal(enum Direction current_direction,
                               enum Direction current_normal,
                               long current_point[2], long next_point[2]) {
  enum Direction next_direction = get_next_dir(current_point, next_point);
  if (next_direction == turn_left(current_direction)) {
    return turn_left(current_normal);
  }
  return turn_right(current_normal);
}

enum CornerDirection combine_dir(enum Direction d0, enum Direction d1) {
  if (d0 == UP || d1 == UP) {
    return d0 + d1 == 1 ? UP_LEFT : UP_RIGHT;
  }
  if (d0 == LEFT || d1 == LEFT) {
    return DOWN_LEFT;
  }
  return DOWN_RIGHT;
}

int corner_to_int(enum CornerDirection d0) { return 1 << d0; }

int all_except(enum Direction d0, enum Direction d1) {
  enum CornerDirection combined = combine_dir(d0, d1);
  return 15 - corner_to_int(combined);
}

struct corner_walker {
  int corner;
  enum Direction next_dir;
  enum Direction next_normal;
};

struct corner {
  long point[2];
  int corner_int;
};

struct corner_walker make_inside(const long points[POINTS][2], int index,
                                 enum Direction current_dir,
                                 enum Direction current_normal) {
  long current_point[2] = {points[index][0], points[index][1]};
  int next_index = (index + 1) % POINTS;
  long next_point[2] = {points[next_index][0], points[next_index][1]};
  enum Direction next_dir = get_next_dir(current_point, next_point);
  enum Direction next_normal =
      get_next_normal(current_dir, current_normal, current_point, next_point);
  int corner;
  if (next_dir == current_normal) {
    corner = corner_to_int(combine_dir(current_normal, next_normal));
  } else {
    corner = all_except(opposite(current_normal), opposite(next_normal));
  }
  struct corner_walker this_corner = {corner, next_dir, next_normal};
  return this_corner;
}

long find_leftmost(const long points[POINTS][2],
                   enum Direction *starting_direction,
                   enum Direction *starting_normal) {
  int m = INT_MAX;
  int argmin = 0;
  for (int i = 1; i < POINTS; i++) {
    if (points[i][0] < m) {
      m = points[i][0];
      argmin = i;
    }
  }
  int next_index = (argmin + 1) % POINTS;
  if (points[argmin][1] < points[next_index][1]) {
    *starting_direction = DOWN;
  } else {
    *starting_direction = UP;
  }
  *starting_normal = RIGHT;
  return argmin + 1;
}

void make_insides(struct corner corners[POINTS], long points[POINTS][2]) {
  enum Direction current_direction = UP;
  enum Direction current_normal = RIGHT;
  int index = find_leftmost(points, &current_direction, &current_normal);
  for (int i = 0; i < POINTS; i++) {
    struct corner_walker this_corner =
        make_inside(points, index, current_direction, current_normal);
    corners[index].point[0] = points[index][0];
    corners[index].point[1] = points[index][1];
    corners[index].corner_int = this_corner.corner;
    index = (index + 1) % POINTS;
    current_direction = this_corner.next_dir;
    current_normal = this_corner.next_normal;
  }
}

int check_direction(struct corner c, enum CornerDirection d) {
  return (c.corner_int >> d) & 1;
}

long av(long x) { return x < 0 ? -x : x; }

long inside_area(struct corner c0, struct corner c1) {
  struct corner corner_0, corner_1;
  long c00 = c0.point[0];
  long c10 = c1.point[0];
  long c01 = c0.point[1];
  long c11 = c1.point[1];
  if (c00 > c10 || (c00 == c10 && c01 > c11)) {
    corner_0 = c1;
    corner_1 = c0;
  } else {
    corner_0 = c0;
    corner_1 = c1;
  }

  if (corner_0.point[0] == corner_1.point[0]) {
    if (0 == check_direction(corner_0, DOWN_LEFT) &&
        0 == check_direction(corner_0, DOWN_RIGHT)) {
      return 0;
    }
    if (0 == check_direction(corner_1, UP_LEFT) &&
        0 == check_direction(corner_1, UP_RIGHT)) {
      return 0;
    }
  } else if (corner_0.point[1] == corner_1.point[1]) {
    if (0 == check_direction(corner_0, UP_RIGHT) &&
        0 == check_direction(corner_0, DOWN_RIGHT)) {
      return 0;
    }
    if (0 == check_direction(corner_1, UP_LEFT) &&
        0 == check_direction(corner_1, DOWN_LEFT)) {
      return 0;
    }
  } else if (corner_0.point[1] < corner_1.point[1]) {
    if (0 == check_direction(corner_0, DOWN_RIGHT) ||
        0 == check_direction(corner_1, UP_LEFT)) {
      return 0;
    }
  } else {
    if (0 == check_direction(corner_0, UP_RIGHT) ||
        0 == check_direction(corner_1, DOWN_LEFT)) {
      return 0;
    }
  }
  return (1 + av(corner_0.point[0] - corner_1.point[0])) *
         (1 + av(corner_0.point[1] - corner_1.point[1]));
}

long check_inside(struct corner corner_0, struct corner corner_1,
                  const long points[POINTS][2]) {
  long x_max = corner_0.point[0] > corner_1.point[0] ? corner_0.point[0]
                                                     : corner_1.point[0];
  long x_min = corner_0.point[0] < corner_1.point[0] ? corner_0.point[0]
                                                     : corner_1.point[0];
  long y_max = corner_0.point[1] > corner_1.point[1] ? corner_0.point[1]
                                                     : corner_1.point[1];
  long y_min = corner_0.point[1] < corner_1.point[1] ? corner_0.point[1]
                                                     : corner_1.point[1];
  for (int i = 0; i < POINTS; i++) {
    int j = (i + 1) % POINTS;
    long check_0[2] = {points[i][0], points[i][1]};
    long check_1[2] = {points[j][0], points[j][1]};
    if (x_min < check_0[0] && check_0[0] < x_max && y_min < check_0[1] &&
        check_0[1] < y_max) {
      return 0;
    }
    if (check_0[0] == check_1[0]) {
      long z_max = check_0[1] > check_1[1] ? check_0[1] : check_1[1];
      long z_min = check_0[1] < check_1[1] ? check_0[1] : check_1[1];
      if ((x_min < check_0[0] && check_0[0] < x_max) &&
          ((z_min < y_min && y_min < z_max) ||
           (z_min < y_max && y_max < z_max))) {
        return 0;
      }
    }
    if (check_0[1] == check_1[1]) {
      long z_max = check_0[0] > check_1[0] ? check_0[0] : check_1[0];
      long z_min = check_0[0] < check_1[0] ? check_0[0] : check_1[0];
      if ((y_min < check_0[1] && check_0[1] < y_max) &&
          ((z_min < x_min && x_min < z_max) ||
           (z_min < x_max && x_max < z_max))) {
        return 0;
      }
    }
  }
  return inside_area(corner_0, corner_1);
}

long final_check(const struct corner corners[POINTS],
                 const long points[POINTS][2]) {
  long answer = 0;
  for (int i = 0; i < POINTS; i++) {
    for (int j = i + 1; j < POINTS; j++) {
      struct corner corner_0 = corners[i];
      struct corner corner_1 = corners[j];
      long area = check_inside(corner_0, corner_1, points);
      answer = answer > area ? answer : area;
    }
  }
  return answer;
}

void parse_line(char line[], long points[POINTS][2], int spot) {
  const char *token = strtok(line, ",\n");
  points[spot][0] = atol(token);
  token = strtok(NULL, ",\n");
  points[spot][1] = atol(token);
}

int main(void) {
  FILE *fp = fopen("input.txt", "r");
  long points[POINTS][2] = {{0}};
  struct corner corners[POINTS] = {0};
  int spot = 0;
  for (char line[100]; fgets(line, 100, fp); spot++) {
    parse_line(line, points, spot);
  }
  fclose(fp);
  make_insides(corners, points);
  long answer = final_check(corners, points);
  printf("answer is %ld\n", answer);
  return 0;
}
