#include <iostream>
#include <vector>
#include <string>
#include <regex>

class Grid {

public:
    // int BOX_SIZE;
    // int GRID_SIZE;
    // std::vector<std::vector<int>> grid;

    Grid(int box_size, int grid_size) {
        checkGrid(box_size, grid_size);
        BOX_SIZE = box_size;
        GRID_SIZE = grid_size;
        // grid = std::vector<std::vector<int>>(GRID_SIZE);
        // for (int i = 0; i < GRID_SIZE; i++) {
        //     grid[i] = std::vector<int>(GRID_SIZE, 0);
        // }
        grid.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    }

    std::vector<int> getRow(int row) {
        return grid[row];
    }

    std::vector<int> getColumn(int col) {
        std::vector<int> resCol;
        for (int i = 0; i < GRID_SIZE; i++) {
            resCol.push_back(grid[i][col]);
        }
        return resCol;
    }

    std::vector<int> getBox(int row, int col) {
        std::vector<int> resBox;
        int boxStartRow = (row / BOX_SIZE) * BOX_SIZE;
        int boxStartCol = (col / BOX_SIZE) * BOX_SIZE;
        for (int i = boxStartRow; i < boxStartRow + BOX_SIZE; i++) {
            for (int j = boxStartCol; j < boxStartCol + BOX_SIZE; j++) {
                resBox.push_back(grid[i][j]);
            }
        }
        return resBox;
    }

protected:
    int BOX_SIZE;
    int GRID_SIZE;
    std::vector<std::vector<int>> grid;

private:
    void checkGrid(int box_size, int grid_size) {
        if (grid_size <= 0 || grid_size != box_size * box_size) {
            throw std::invalid_argument("invalid grid parameters!");
        }
    }
};


class Sudoku : public Grid {

public:
    Sudoku(int box_size, int grid_size) : Grid(box_size, grid_size) {}

    void parse(const std::string& input) {
        checkInput(input);
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                grid[row][col] = input[row * GRID_SIZE + col] - '0';
            }
        }
    }

    std::vector<std::vector<std::vector<int>>> getInference() {
        std::vector<std::vector<std::vector<int>>> inference;
        for (int row = 0; row < GRID_SIZE; row++) {
            std::vector<std::vector<int>> rowArr;
            for (int col = 0; col < GRID_SIZE; col++) {
                if (grid[row][col] == 0) {
                    auto candidates = getCandidates(row, col);
                    rowArr.push_back(candidates);
                } else {
                    rowArr.push_back({ grid[row][col] });
                }
            }
            inference.push_back(rowArr);
        }
        return inference;
    }

private:
    void checkInput(std::string input) {
        if (input.length() != GRID_SIZE * GRID_SIZE) {
            throw std::invalid_argument("invalid sudoku size!");
        }

        std::regex digitRegex("^[0-9]+$");
        if (!std::regex_match(input, digitRegex)) {
            throw std::invalid_argument("invalid sudoku number!");
        }
    }

    std::vector<int> getCandidates(int row, int col) {
        std::vector<int> candidates;
        auto sameRow = getRow(row);
        auto sameCol = getColumn(col);
        auto sameBox = getBox(row, col);

        for (int num = 1; num <= GRID_SIZE; num++) {
            if (std::count(sameRow.begin(), sameRow.end(), num) == 0 &&
                std::count(sameCol.begin(), sameCol.end(), num) == 0 &&
                std::count(sameBox.begin(), sameBox.end(), num) == 0) {
                candidates.push_back(num);
            }
        }
        return candidates;
    }
};


bool testResult(std::vector<std::vector<std::vector<int>>> output, std::vector < std::vector<std::vector<int>>> truth) {
    for (int row = 0; row < truth.size(); row++) {
        for (int col = 0; col < truth[row].size(); col++) {
            if (output[row][col].size() != truth[row][col].size()) return false;

            for (int k = 0; k < truth[row][col].size(); k++) {
                if (output[row][col][k] != truth[row][col][k]) return false;
            }
        }
    }

    return true;
}

int main() {
    int grid_size = 9, box_size = 3;
    std::string input = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";
    Sudoku sudoku = Sudoku(box_size, grid_size);
    sudoku.parse(input);
    auto inference = sudoku.getInference();
    // for (int row = 0; row < grid_size; row++) {
    //     for (int col = 0; col < grid_size; col++) {
    //         for (auto const& num : inference[row][col]) {
    //             printf("%d", num);
    //         }
    //         printf(" ");
    //     }
    //     puts("");
    // }
    std::vector<std::vector<std::vector<int>>> truth = {
        {{2, 4, 5, 8}, {1}, {7}, {9}, {2, 4, 5}, {3}, {6}, {4, 8}, {2, 4, 8}},
        {{2, 3, 4, 5, 6}, {2, 3, 4, 5}, {3, 6}, {1, 2, 5, 7}, {8}, {4, 5, 6, 7}, {1, 3, 9}, {1, 4, 9}, {1, 2, 3, 4, 9}},
        {{9}, {2, 3, 4, 8}, {3, 6, 8}, {1, 2}, {2, 4, 6}, {4, 6}, {5}, {1, 4, 8}, {7}},
        {{5, 8}, {7}, {2}, {5, 8}, {1}, {5, 6, 8, 9}, {4}, {3}, {6, 8, 9}},
        {{1, 3, 5, 8}, {3, 5, 8, 9}, {3, 8, 9}, {4}, {5, 6, 9}, {2}, {1, 8, 9}, {7}, {1, 6, 8, 9}},
        {{1, 8}, {6}, {4}, {3}, {7}, {8, 9}, {2}, {5}, {1, 8, 9}},
        {{7}, {2, 3, 4, 8, 9}, {1}, {2, 8}, {2, 4, 9}, {4, 8, 9}, {3, 8, 9}, {6}, {5}},
        {{2, 4, 6, 8}, {2, 4, 8, 9}, {6, 8, 9}, {2, 5, 7, 8}, {3}, {4, 5, 7, 8, 9}, {1, 8, 9}, {1, 4, 8, 9}, {1, 4, 8, 9}},
        {{3, 4, 8}, {3, 4, 8, 9}, {5}, {6}, {4, 9}, {1}, {7}, {2}, {3, 4, 8, 9}}
    };

    if (testResult(inference, truth)) {
        puts("PASS!");
    } else {
        puts("WRONG!");
    }
}