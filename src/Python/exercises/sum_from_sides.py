from random import randint

ROWS = 5
COLS = 5
DEPTH = 9

def make_matrix(rows, cols):
    matrix = []
    for r in range(rows):
        matrix.append([])
        for c in range(cols):
            matrix[r].append(randint(1,9))
    return matrix


def make_indices(rows, cols):
    idx = []
    for r in range(rows):
        idx.append([0, cols-1])
    return idx


def check_similar_path(solution, path):
    if len(solution) == 0 or len(path) == 0 or len(path) > len(solution):
        return False
    if path != solution[0: len(path)]:
        for cell in path:
            if cell not in solution:
                return False
        return True
    return False


def dfs(matrix, current, state, solution):
    depth, sum, path, idxs = state
    
    row = current[0]
    if row < 0 or row >= len(matrix):
        return
    
    if idxs[row][0] > idxs[row][1]:
        return 
    
    idx_col = current[1]
    col = idxs[row][idx_col]
    next_cell = (row, col)
    # print(f"Exploring: depth={len(path)} ({row, col})       ", end="\r", flush=True)

    # Cull 1 -- should not happen but ...
    if (row, col) in path:
        return
    
    # Cull 2, is it too late to get a good solution?
    if depth * 9 + sum < solution[0]:
        return
    
    # Cull 3, if a path is different but has the same items to an existing solution, we can discard it
    if check_similar_path(solution[1], path):
        return
    
    path.append(next_cell)
    if idx_col == 1:
        idxs[row][1] -= 1
    else:
        idxs[row][0] += 1

    cell_value = matrix[row][col]
    try:
        sum += cell_value
        if depth <= 1:
            if solution[0] < sum:
                solution[0] = sum
                solution[1] = path.copy()
            return

        for nrow in range(len(matrix)):
            dfs(matrix, (nrow, 0), [depth - 1, sum, path, idxs], solution)
            dfs(matrix, (nrow, 1), [depth - 1, sum, path, idxs], solution)

    finally:
        if idx_col == 1:
            idxs[row][1] += 1
        else:
            idxs[row][0] -= 1
        path.pop()


def best_sum_from_sides(matrix, depth):
    solution = [0, []]
    
    for row in range(len(matrix)):
        dfs(matrix, (row, 0), [depth, 0, [], make_indices(len(matrix), len(matrix[0]))], solution)
        dfs(matrix, (row, 1), [depth, 0, [], make_indices(len(matrix), len(matrix[0]))], solution)

    return solution


# matrix = make_matrix(ROWS, COLS)
matrix = [
    [2, 1, 4, 5, 2], 
    [2, 1, 1, 1, 8], 
    [3, 1, 1, 1, 4],
    [1, 9, 1, 9, 3],  
    [1, 1, 5, 2, 3]]
print(f"Matrix:\n{matrix}")
solution = best_sum_from_sides(matrix, DEPTH)
print("")
print(f"Found = {solution}")
