# Atomic Wasteland

AREA = [
    [1,  4,  5,  3,  2,  3,  6,  1],
    [3,'*',  3,  4,  6,  8,  9,  1],
    [3,  3,  2,  2,  1,  4,  4,  9],
    [0,  1,  1,'x',  9,'*',  8,  3],
    [7,  8,  9,  1,  4,  1,  0,  2],
]


UP = (-1, 0)
DOWN = (1, 0)
LEFT = (0, -1)
RIGHT = (0, 1)
DIRS = [UP, DOWN, LEFT, RIGHT]

def find_start(area):
    frow, fcol = -1, -1
    well_formed = False
    row = 0
    while row < len(area):
        area_row = area[row]
        col = 0
        while col < len(area_row):
            if area_row[col] == 'x':
                if well_formed:
                    return row, col
                frow, fcol = row, col
            if area_row[col] == '*':
                if frow >= 0:
                    return frow, fcol
                well_formed = True
            col += 1
        row += 1
    raise RuntimeError("Area is not well formed.")

best = None

def search_resources(area, candidate):
    global best
    found_candidate = None
    curb = -1 if not best else best[0]
    for rc in DIRS:
        row, col = candidate[-1]
        row += rc[0]
        col += rc[1]
        if row <0 or row >= len(area) or col < 0 or col >= len(area[0]):
            continue
        if (row, col) in candidate:
            continue
        ncandidate = candidate.copy()
        ncandidate.append((row, col))
        if area[row][col] == '*':
            if not best or best[0] > ncandidate[0]:
                best = ncandidate
            return ncandidate
        ncandidate[0] += area[row][col]
        if curb >= 0 and ncandidate[0] >= curb:
            continue
        sub_found_candidate = search_resources(area, ncandidate)
        if sub_found_candidate and (not found_candidate or found_candidate[0] > sub_found_candidate[0]):
            found_candidate = sub_found_candidate
    return found_candidate


def find_path(area):
    row, col = find_start(area)
    return search_resources(area, [0, (row, col)])


def main():
    path = find_path(AREA)
    print(path)


if __name__ == "__main__":
    main()
