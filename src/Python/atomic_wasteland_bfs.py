# Atomic Wasteland.
# We are dropped in an atomic wasteland at a random location marked by an 'x'.
# The number on the map represent the radiation level of each zone.
# We need to reach a resource cache (marked by an '*') through the path exposing
# us to the least radiation possible.

# Breadth-first search implementation.

AREA = [
    [1,  4,  5,  3,  2,  3,  6,  1],
    [3,'*',  3,  4,  6,  8,  9,  1],
    [3,  3,  2,  2,  1,  4,  4,  9],
    [0,  1,  1,'x',  9,'*',  8,  3],
    [7,  8,  9,  1,  4,  1,  0,  2],
]

DIRS = [(-1, 0), (1, 0), (0, -1), (0, 1)]

def find_start(area):
    frow, fcol = -1, -1
    found_start = False
    found_resource = False
    for row in range(len(area)):
        area_row = area[row]
        for col in range(len(area_row)):
            if found_start and found_resource:
                return frow, fcol
            if area_row[col] == 'x':
                found_start = True
                frow, fcol = row, col
            if area_row[col] == '*':
                found_resource = True
    raise RuntimeError("Area is not well formed.")


def value_of_path(area, path):
    if path is None:
        return -float("inf")
    
    value = 0
    for row, col in path:
        if type(area[row][col]) != str:
            value += area[row][col]
    # Values represent a radiation level, so they are a negative to be avoided
    return -value


def valid_position(area, row, col):
    return row >= 0 and row < len(area) and col >= 0 and col < len(area[0])


def find_path(area):
    row, col = find_start(area)
    stack = [(0, [(row, col)])]
    candidate = [-float("inf"), None]

    while stack:
        path_value, path = stack.pop()
        
        # Candidate path value starts at -inf and is set only when a valid path is found.
        # As the value of a path decreases as we meet radioactive areas, if we are on a path that is
        # already worse than the candidate, we're sure we can abandon that route.
        if candidate[0] > path_value:
            continue

        # Detect a complete path
        row, col = path[-1]
        if area[row][col] == '*':
            candidate[0] = path_value
            candidate[1] = path
            continue
            
        # Fill with alternative paths to follow
        for drow, dcol in DIRS:
            if valid_position(area, row + drow, col + dcol):
                npath = path.copy()
                npath.append( (row + drow, col + dcol) )
                stack.append( (value_of_path(area, path), npath) )

    return candidate[1]


def main():
    path = find_path(AREA)
    print(f"Found path with value {value_of_path(AREA, path)}: {path}")


if __name__ == "__main__":
    main()
