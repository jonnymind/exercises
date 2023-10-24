#include <iostream>
#include <vector>
#include <ranges>

using Matrix = std::vector<std::vector<int>>;
using Step = std::pair<int, int>;
using Path = std::vector<Step>;

Matrix matrix = 
    {
        {2, 1, 4, 5, 2},
        {2, 1, 1, 1, 8},
        {3, 1, 1, 1, 4},
        {1, 9, 1, 9, 3}, 
        {1, 1, 5, 2, 3}
    };

static const int DEPTH = 7;

struct State {
    int cur_depth = 0;
    int sum = 0;
    Path path;
    Path idxs;
    State(int depth, const Matrix& source): cur_depth{depth} {
        for(std::size_t row=0; row < source.size(); ++row) {
            idxs.push_back({0, source[0].size()});    
        }
    }
};

struct Solution {
    int sum = 0;
    Path path;
};

bool similarPaths(const Path& solution, const Path& path) {
    if(solution.empty() || path.empty() || path.size() >= solution.size()) {
        return false;
    }

    auto soli = solution.cbegin();
    for(const auto& elem: path) {
        if(elem != *soli) {
            break;
        }
        soli++;
    }
    if (*(soli--) == path.back()) {
        return false;
    }

    for(const auto& elem: path) {
        if(std::find(solution.begin(), solution.end(), elem) == solution.end()) {
            return false;
        }
    }

    return true;
}

void dfs(const Matrix& matrix, Step current, State state, Solution& solution) {
    auto row = current.first;
    if(row < 0 or row > matrix.size()) {
        return;
    }
    
    auto& idxOfRow = state.idxs[row];
    if(idxOfRow.first > idxOfRow.second) {
        return;
    }

    auto col = current.second ? idxOfRow.second : idxOfRow.first;

    state.path.push_back({row, col});
    
    if (9 * state.cur_depth < solution.sum) {
        return;
    }

    if ( similarPaths(solution.path, state.path)) {
        return;
    }

    state.sum += matrix[row][col];
    
    if(current.second) {
        --idxOfRow.second;
    } else {
        ++idxOfRow.first;
    }

    if(--state.cur_depth == 0) {
       if (state.sum > solution.sum) {
            solution.path = state.path;
            solution.sum = state.sum;
       }
    } else {
        for(auto row = 0u; row < matrix.size(); ++row) {
            dfs(matrix, {row, 0}, state, solution);
            dfs(matrix, {row, 1}, state, solution);
        }
    }
}

Solution find_best_sum_from_sides(const Matrix& matrix, int depth) {
    auto solution = Solution();
    for(auto row = 0u; row < matrix.size(); ++row) {
        dfs(matrix, {row, 0}, State(DEPTH, matrix), solution);
        dfs(matrix, {row, 1}, State(DEPTH, matrix), solution);
    }
    
    return solution;
}

int main() {
    auto solution = find_best_sum_from_sides(matrix, DEPTH);
    std::cout << "Found " << solution.sum << "{";
    for(auto& entry: solution.path) {
        std::cout << "(" << entry.first << ", " << entry.second << ")";
        if(entry != solution.path.back()) {
            std::cout << " ";
        }
    }
    std::cout << "}\n";
    return 0;
}
