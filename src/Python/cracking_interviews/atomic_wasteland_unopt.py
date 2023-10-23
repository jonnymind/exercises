# Atomic Wasteland.
# We are dropped in an atomic wasteland at a random location marked by an 'x'.
# The number on the map represent the radiation level of each zone.
# We need to reach a resource cache (marked by an '*') through the path exposing
# us to the least radiation possible.

# Depth-first search implementation, with depth culling.

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
                if found_start:
                    raise RuntimeError("Area is not well formed.")
                found_start = True
                frow, fcol = row, col
            if area_row[col] == '*':
                found_resource = True
    raise RuntimeError("Area is not well formed.")


def valid_position(area, hypothesis):
    row, col = hypothesis
    return row >= 0 and row < len(area) and col >= 0 and col < len(area[0])


def value_of_path(area, path):
    if path is None:
        return -float("inf")
    
    value = 0
    for row, col in path:
        if type(area[row][col]) != str:
            value += area[row][col]
    # Values represent a radiation level, so they are a negative to be avoided
    return -value


def search_resources(area, current, status, solution):
    path = status[0]
    
    # Validate the hypothesis
    if not valid_position(area, current) or current in path:
        return
    
    # Prepare a tentative hypothesis and get ready to backtrack.
    path.append(current)
    try:
        # Evaluate the hypothesis
        path_value = value_of_path(area, path)
        
        # Candidate path value starts at -inf and is set only when a valid path is found.
        # As the value of a path decreases as we meet radioactive areas, if we are on a path that is
        # already worse than the solution, we're sure we can abandon that route.
        if path_value < solution[0]:
            return
        
        row, col = path[-1]
        if area[row][col] == '*':
            if path_value > solution[0]:
                solution[0] = path_value
                solution[1] = path.copy()
            return
        
        for rc in DIRS:
            search_resources(area, (row + rc[0], col + rc[1]), status, solution)
    
    finally:
        path.pop() # Backtrack

def find_path(area):
    row, col = find_start(area)
    solution = [-float("inf"), None]
    search_resources(area, (row, col), [[]], solution)
    return solution


path = find_path(AREA)
print(f"Found path with value {path}")
