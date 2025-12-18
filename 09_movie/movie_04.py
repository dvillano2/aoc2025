import copy

NORMALS = ["u", "r", "d", "l"]

with open("small_input.txt") as f:
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


def find_leftmost(data):
    m = float("inf")
    argmin = -1
    for i, info in enumerate(data):
        if info[0] < m:
            m = info[0]
            argmin = i
    next_corner = data_next(argmin, data)
    if data[argmin][1] < data[next_corner][1]:
        return argmin + 1, "d", "r"
    return argmin + 1, "u", "r"


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


def make_insides(data):
    new_data = copy.deepcopy(data)
    index, current_dir, current_normal = find_leftmost(data)
    for _ in data:
        corner, next_dir, next_normal = make_inside(
            data, index, current_dir, current_normal
        )
        new_data[index].append(corner)
        index = data_next(index, data)
        current_dir = next_dir
        current_normal = next_normal
    return new_data


def inside_area(corner_0, corner_1):
    if corner_0[0] > corner_1[0]:
        corner_0, corner_1 = corner_1, corner_0
    if corner_0[0] == corner_1[0]:
        if corner_0[1] > corner_1[1]:
            corner_0, corner_1 = corner_1, corner_0
        if "dl" not in corner_0[2] and "dr" not in corner_0[2]:
            return 0
        if "ul" not in corner_1[2] and "ur" not in corner_1[2]:
            return 0
    elif corner_0[1] == corner_1[1]:
        if "ur" not in corner_0[2] and "dr" not in corner_0[2]:
            return 0
        if "ul" not in corner_1[2] and "dl" not in corner_1[2]:
            return 0
    elif corner_0[1] < corner_1[1]:
        if "dr" not in corner_0[2] or "ul" not in corner_1[2]:
            return 0
    elif corner_0[1] > corner_1[1]:
        if "ur" not in corner_0[2] or "dl" not in corner_1[2]:
            return 0
    return (1 + abs(corner_0[0] - corner_1[0])) * (
        1 + abs(corner_0[1] - corner_1[1])
    )


def check_inside(corner_0, corner_1, data):
    x_max = max(corner_0[0], corner_1[0])
    x_min = min(corner_0[0], corner_1[0])
    y_max = max(corner_0[1], corner_1[1])
    y_min = min(corner_0[1], corner_1[1])
    for i, check_0 in enumerate(data):
        j = data_next(i, data)
        check_1 = data[j]
        if x_min < check_0[0] < x_max and y_min < check_0[1] < y_max:
            return 0
        if check_0[0] == check_1[0]:
            z_max = max(check_0[1], check_1[1])
            z_min = min(check_0[1], check_1[1])
            if x_min < check_0[0] < x_max and (
                z_min < y_min < z_max or z_min < y_max < z_max
            ):
                return 0
        if check_0[1] == check_1[1]:
            z_max = max(check_0[0], check_1[0])
            z_min = min(check_0[0], check_1[0])
            if y_min < check_0[1] < y_max and (
                z_min < x_min < z_max or z_min < x_max < z_max
            ):
                return 0
    return inside_area(corner_0, corner_1)


def final_check(data):
    answer = 0
    for i, corner_0 in enumerate(data):
        for corner_1 in data[i + 1 :]:
            area = check_inside(corner_0, corner_1, data)
            answer = max(answer, area)
    return answer
