
DIRS = [(-1,0), (1,0), (0,-1), (0,1)]

def largest_island(grid):
    rows, cols = len(grid), len(grid[0])
    visited = [[False for _ in range(cols)] for _ in range(rows)]

    def valid_cell(r, c):
        return r >= 0 and r < len(grid) and c >= 0 and c < len(grid[0])

    max_size = 0
    for r in range(rows):
        for c in range(cols):
            if grid[r][c] == 1:
                area = 0
                explore_stack = [(r, c)]
                while len(explore_stack) > 0:
                    r, c = explore_stack.pop()
                    if not valid_cell(r,c) or visited[r][c] or grid[r][c] == 0:
                        continue
                    area += 1
                    visited[r][c] = True
                
                    for dr, dc in DIRS:
                        explore_stack.append((r+dr, c+dc))
                max_size = max(area, max_size)

    return max_size


ISLAND = [
    [0, 0, 1, 0, 0],
    [0, 1, 1, 1, 0],
    [0, 1, 0, 0, 0],
    [1, 1, 0, 0, 1]
    ]

print(largest_island(ISLAND)) 
