import copy
import pprint

with open("small_input.txt") as f:
    data = f.read().strip().split("\n")

for line in data:
    info = line.split(" ")
    info = info[1:]
    target = [int(x) for x in info[-1][1:-1].split(",")]
    print(target)
    info = info[:-1]
    info = [i[1:-1] for i in info]
    info = [[int(x) for x in i.split(",")] for i in info]
    print(info)
    start = [[0 for _ in target], 0]
    queue = []
    queue.append(start)
    seen = {tuple(start[0]): 0}
    while queue:
        break_flag = 0
        new, pushes = queue.pop(0)
        for button in info:
            added = copy.deepcopy(new)
            for index in button:
                added[index] += 1
            searcher = (x - y for x, y in zip(target, added))
            if searcher in seen:
                print(f"found in {pushes + 1 + seen[searcher][1]}")
                break_flag = 1
            if tuple(added) not in seen:
                seen[tuple(added)] = pushes + 1
                queue.append([added, pushes + 1])
        pprint.pprint(queue)
        print("")
        if break_flag == 1:
            break

info = line.split(" ")
info = info[1:]
target = [int(x) for x in info[-1][1:-1].split(",")]
print(target)
info = info[:-1]
info = [i[1:-1] for i in info]
info = [[int(x) for x in i.split(",")] for i in info]
print(info)
start = [[0 for _ in target], 0]
queue = []
queue.append(start)
seen = {tuple(start[0]): 0}


new, pushes = queue.pop(0)
for button in info:
    added = copy.deepcopy(new)
    for index in button:
        added[index] += 1
    searcher = (x - y for x, y in zip(target, added))
    if searcher in seen:
        print(f"found in {pushes + 1 + seen[searcher][1]}")
        break_flag = 1
    if tuple(added) not in seen:
        seen[tuple(added)] = pushes + 1
        queue.append([added, pushes + 1])
