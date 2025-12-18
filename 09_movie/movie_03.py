import copy

NORMALS = ["u", "r", "d", "l"]

with open("input.txt") as f:
    data = [
        [int(num) for num in line.split(",")]
        for line in f.read().strip().split("\n")
    ]


def data_next(index, data):
    if index == len(data) - 1:
        return 0
    return index + 1


def opposite(direction):
    if direction == "u":
        return "d"
    if direction == "d":
        return "u"
    if direction == "l":
        return "r"
    if direction == "r":
        return "l"


def turn_left(direction):
    if direction == "u":
        return "l"
    if direction == "d":
        return "r"
    if direction == "l":
        return "d"
    if direction == "r":
        return "u"


def turn_right(direction):
    if direction == "u":
        return "r"
    if direction == "d":
        return "l"
    if direction == "l":
        return "u"
    if direction == "r":
        return "d"


def get_next_normal(
    current_direction, current_normal, current_point, next_point
):
    next_dir = get_next_dir(current_point, next_point)
    if next_dir == turn_left(current_direction):
        return turn_left(current_normal)
    return turn_right(current_normal)


def get_next_dir(current_point, next_point):
    if current_point[0] == next_point[0]:
        if current_point[1] < next_point[1]:
            return "d"
        return "u"
    if current_point[0] < next_point[0]:
        return "r"
    return "l"


def combine_dir(dir_1, dir_2):
    if dir_2 in ["u", "d"]:
        dir_1, dir_2 = dir_2, dir_1
    return dir_1 + dir_2


def all_except(dir_1, dir_2):
    all_dir = ["ul", "ur", "dl", "dr"]
    all_dir.remove(combine_dir(dir_1, dir_2))
    return all_dir


def make_inside(data, index, current_dir, current_normal):
    current_point = data[index]
    next_index = data_next(index, data)
    next_point = data[next_index]
    next_normal = get_next_normal(
        current_dir, current_normal, current_point, next_point
    )
    next_dir = get_next_dir(current_point, next_point)
    corner = []
    if next_dir == current_normal:
        corner = [combine_dir(current_normal, next_normal)]
    else:
        corner = all_except(opposite(current_normal), opposite(next_normal))

    return corner, next_dir, next_normal


def make_insides(data, index=120, current_dir="l", current_normal="u"):
    new_data = copy.deepcopy(data)
    for _ in data:
        corner, next_dir, next_normal = make_inside(
            data, index, current_dir, current_normal
        )
        new_data[index].append(corner)
        print(new_data[index])
        index = data_next(index, data)
        current_dir = next_dir
        current_normal = next_normal
    return new_data


def valid_corners(sorted_new_data, index0, index1):
    data0 = sorted_new_data[index0]
    data1 = sorted_new_data[index1]
    if data0[0] == data1[0]:
        if data0[1] < data1[1]:
            if "dr" not in data0[2] and "dl" not in data0[2]:
                return 0
            if "ur" not in data1[2] and "ul" not in data1[2]:
                return 0
        if data0[1] > data1[1]:
            if "ur" not in data0[2] and "ul" not in data0[2]:
                return 0
            if "dr" not in data1[2] and "dl" not in data1[2]:
                return 0
    if data0[1] == data1[1]:
        if "dr" not in data0[2] and "ur" not in data0[2]:
            return 0
        if "dl" not in data1[2] and "ul" not in data1[2]:
            return 0
    if data0[1] < data1[1]:
        if "dr" not in data0[2]:
            return 0
        if "ul" not in data1[2]:
            return 0
    if data0[1] > data1[1]:
        if "ur" not in data0[2]:
            return 0
        if "dl" not in data1[2]:
            return 0
    return (1 + abs(data1[0] - data0[0])) * (1 + abs(data1[1] - data0[1]))


def interior_blocker(data0, data1, check):
    x_max = max(data0[0], data1[0])
    x_min = min(data0[0], data1[0])
    y_max = max(data0[1], data1[1])
    y_min = min(data0[1], data1[1])
    if not (x_min <= check[0] <= x_max and y_min <= check[1] <= y_max):
        return False
    if x_min < check[0] < x_max and y_min < check[1] < y_max:
        return True
    if [check[0], check[1]] in [
        [x_min, y_min],
        [x_max, y_min],
        [x_max, y_max],
        [x_min, y_max],
    ]:
        return False
    if len(check[2]) == 3:
        return False
    return True


def interior_blocker_2(data0, data1, original_data):
    x_max = max(data0[0], data1[0])
    x_min = min(data0[0], data1[0])
    y_max = max(data0[1], data1[1])
    y_min = min(data0[1], data1[1])
    data_copy = copy.deepcopy(original_data)
    data_copy.append(data[0])
    for check_0, check_1 in zip(original_data, data_copy[1:]):
        if check_0[0] == check_1[0]:
            if check_0[1] > check_1[1]:
                check_0, check_1 = check_1, check_0
            if x_min < check_0[0] < x_max:
                if check_0[1] in [y_max, y_min] and len(check_0[2]) == 1:
                    return True
                if check_1[1] in [y_max, y_min] and len(check_1[2]) == 1:
                    return True
                if check_0[1] < y_min < check_1[1]:
                    return True
                if check_0[1] < y_max < check_1[1]:
                    return True
            if check_0[0] == x_min or check_0[0] == x_max:
                if y_min < check_0[1] < y_max and len(check_0[2]) == 1:
                    return True
                if y_min < check_1[1] < y_max and len(check_1[2]) == 1:
                    return True

        if check_0[1] == check_1[1]:
            if check_0[0] > check_1[0]:
                check_0, check_1 = check_1, check_0
            if y_min < check_0[1] < y_max:
                if check_0[0] in [x_max, x_min] and len(check_0[2]) == 1:
                    return True
                if check_1[0] in [x_max, x_min] and len(check_1[2]) == 1:
                    return True
                if check_0[0] < x_min < check_1[0]:
                    return True
                if check_0[0] < x_max < check_1[0]:
                    return True
            if check_0[1] == y_min or check_0[1] == y_max:
                if x_min < check_0[0] < x_max and len(check_0[2]) == 1:
                    return True
                if x_min < check_1[0] < x_max and len(check_1[2]) == 1:
                    return True

        return False


def minimal_interior_blocker(data0, data1, original_data_with_corners):
    x_max = max(data0[0], data1[0])
    x_min = min(data0[0], data1[0])
    y_max = max(data0[1], data1[1])
    y_min = min(data0[1], data1[1])
    data_copy = copy.deepcopy(original_data_with_corners)
    data_copy.append(data[0])
    for check_0, check_1 in zip(original_data_with_corners, data_copy[1:]):
        if check_0[0] == check_1[0]:
            if check_0[1] > check_1[1]:
                check_0, check_1 = check_1, check_0
            if x_min < check_0[0] < x_max:
                if (
                    check_0[1] < y_min < check_1[1]
                    or check_0[1] < y_max < check_1[1]
                ):
                    return True
        if check_0[1] == check_1[1]:
            if check_0[0] > check_1[0]:
                check_0, check_1 = check_1, check_0
            if y_min < check_0[1] < y_max:
                if (
                    check_0[0] < x_min < check_1[0]
                    or check_0[0] < x_max < check_1[0]
                ):
                    return True
        False


def final_check(sorted_new_data, original_data):
    answer = 0
    for i, data0 in enumerate(sorted_new_data):
        for j in range(i + 1, len(sorted_new_data)):
            data1 = sorted_new_data[j]
            break_flag = 0
            if minimal_interior_blocker(data0, data1, original_data):
                break_flag = 1
                break
            if break_flag == 0:
                new_area = valid_corners(sorted_new_data, i, j)
                if new_area > answer:
                    print(i, j, new_area)
                    answer = new_area
    return answer
