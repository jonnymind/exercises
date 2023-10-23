
DIRS = [(-1,0), (1,0), (0,-1), (0,1)]


def valid_cell(grid, cell):
    return cell[0] >= 0 and cell[0] < len(grid) and cell[1] >= 0 and cell[1] < len(grid[0])


def explore_islands(grid, current, state, solution):
    if not valid_cell(grid, current):
        return
    
    r, c = current
    hypothesis, visited = state
    if visited[r][c] or grid[r][c] == 0:
        return 

    visited[r][c] = True
    hypothesis.append(current)

    # Evaluate the hypothesis and eventually set the solution
    if len(hypothesis) > solution[0]:
        solution[1] = hypothesis.copy()
        solution[0] = len(hypothesis)

    # Get validity constraints from the state.
    for dr, dc in DIRS:
        new_cell = (r + dr, c + dc)   
        explore_islands(grid, new_cell, state, solution)
    

def largest_island(grid):
    rows, cols = len(grid), len(grid[0])
    visited = [[False for _ in range(cols)] for _ in range(rows)]
    solution = [0, []]
    
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == 1 and not visited[r][c]:
                explore_islands(grid, (r, c), ([], visited), solution)
                
    return solution


ISLAND = [
    [0, 0, 1, 0, 0],
    [0, 1, 1, 1, 0],
    [0, 1, 0, 0, 0],
    [1, 1, 0, 0, 1]
    ]

print(largest_island(ISLAND)) 
