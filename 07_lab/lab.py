with open("input.txt") as f:
    tree = f.read().strip().split("\n")
count = [[0 if c != "S" else 1 for c in line] for line in tree]
ANSWER1 = 0
for i, line in enumerate(tree[1:]):
    for j, c in enumerate(line):
        above = count[i][j]
        if c == "^":
            ANSWER1 += above > 0
            count[i + 1][j - 1] += above
            count[i + 1][j + 1] += above
        else:
            count[i + 1][j] += above
print("p1 answer:", ANSWER1, "\np2 answer:", sum(count[-1]))
