
DIRS = [(-1,0), (1,0), (0,-1), (0,1)]

def largest_island(grid):
    rows, cols = len(grid), len(grid[0])
    visited = [[False for _ in range(cols)] for _ in range(rows)]
    max_size = 0

    def valid_cell(r, c):
        return r >= 0 and r < len(grid) and c >= 0 and c < len(grid[0])

    def explore_islands(r, c):
        if not valid_cell(r, c) or visited[r][c] or grid[r][c] == 0:
            return 0

        visited[r][c] = True

        # Get validity constraints from the state.
        local_area = 1
        for dr, dc in DIRS:
            local_area += explore_islands(r + dr, c + dc)   
        return local_area
        
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == 1 and not visited[r][c]:
                max_size = max(max_size, explore_islands(r, c))
                
    return max_size


ISLAND = [
    [0, 0, 1, 0, 0],
    [0, 1, 1, 1, 0],
    [0, 1, 0, 0, 0],
    [1, 1, 0, 0, 1]
    ]

print(largest_island(ISLAND)) 
