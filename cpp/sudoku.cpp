/**
 * @code_url: https://github.com/LittleZoem/Sudoku
 * @author 李涛  24214438
 */

#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <stdexcept>

class Grid {

public:
    Grid(int box_size, int grid_size) {
        checkGrid(box_size, grid_size); // 检查参数合法性
        BOX_SIZE = box_size;
        GRID_SIZE = grid_size;
        grid.assign(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
    }

    std::vector<int> getRow(int row) { // 获取数独指定行的元素
        return grid[row];
    }

    std::vector<int> getColumn(int col) { // 获取数独指定列的元素
        std::vector<int> resCol;
        for (int i = 0; i < GRID_SIZE; i++) {
            resCol.push_back(grid[i][col]);
        }
        return resCol;
    }

    std::vector<int> getBox(int row, int col) { // 获取指定方格所在的BOX的元素
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
    void checkGrid(int box_size, int grid_size) { // 检查grid参数是否合法
        if (grid_size <= 0 || grid_size != box_size * box_size) {
            throw std::invalid_argument("invalid grid parameters!");
        }
    }
};


class Sudoku : public Grid {

public:
    Sudoku(int box_size, int grid_size) : Grid(box_size, grid_size) {}

    Sudoku(const Sudoku& other) : Grid(other.BOX_SIZE, other.GRID_SIZE) {
        grid = other.grid;
    }

    Sudoku& operator=(const Sudoku& other) {
        if (this != &other) {
            this->BOX_SIZE = other.BOX_SIZE;
            this->GRID_SIZE = other.GRID_SIZE;
            this->grid = other.grid;
        }
        return *this;
    }

     bool operator==(const Sudoku& other) const { //比较
        return this->grid == other.grid;
    }

    bool operator!=(const Sudoku& other) const {
        return !(*this == other);
    }

    std::string serialize() const { // 序列化成字符串
        std::string s;
        for (const auto& row : grid) {
            for (int num : row) {
                s += std::to_string(num);
            }
        }
        return s;
    }

    void deserialize(const std::string& data) { //反序列化，从字符串中恢复数独
        parse(data);
    }

    Sudoku* clone() const { // 克隆对象
        return new Sudoku(*this);
    }

    void parse(const std::string& input) { // 解析字符串输入，得到Sudoku
        checkInput(input);
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                grid[row][col] = input[row * GRID_SIZE + col] - '0';
            }
        }
    }

    std::vector<std::vector<std::vector<int>>> getInference() { // 推理棋盘，得到各单元格候选值
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
    void checkInput(std::string input) { // 检查输入字符串的有效性
        if (input.length() != GRID_SIZE * GRID_SIZE) {
            throw std::invalid_argument("invalid sudoku size!");
        }

        std::regex digitRegex("^[0-9]+$");
        if (!std::regex_match(input, digitRegex)) {
            throw std::invalid_argument("invalid sudoku number!");
        }
    }

    std::vector<int> getCandidates(int row, int col) { // 排除同行同列同box中的数，得到指定单元格的候选值
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


// 检查输出的候选向量和实际值是否一致
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

// 测试
void test(int grid_size, int box_size, std::string input, std::vector<std::vector<std::vector<int>>> truth) {
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
    if (testResult(inference, truth)) {
        puts("PASS!");
    } else {
        puts("WRONG!");
    }
}

int main() {
    int grid_size = 9, box_size = 3;

    // 测试1：输入序列长度异常
    std::string input1 = "01790360000008000090000050707201043000040207006437025070100006500003000000560172";
    std::vector<std::vector<std::vector<int>>> truth1 = { {{}} };

    //测试2：输入序列包含非数字
    std::string input2 = "a179##600000080000900000507072010430000402070064370250701000065000030000005601720";
    std::vector<std::vector<std::vector<int>>> truth2 = { {{}} };

    //测试3：正常数独序列
    std::string input3 = "017903600000080000900000507072010430000402070064370250701000065000030000005601720";
    std::vector<std::vector<std::vector<int>>> truth3 = {
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

    //测试4：完全解出的数独序列（不含0）
    std::string input4 = "534678912672195348198342567859761243426853791713924856961235487287419635345867129";
    std::vector<std::vector<std::vector<int>>> truth4 = {
        {{5}, {3}, {4}, {6}, {7}, {8}, {9}, {1}, {2}},
        {{6}, {7}, {2}, {1}, {9}, {5}, {3}, {4}, {8}},
        {{1}, {9}, {8}, {3}, {4}, {2}, {5}, {6}, {7}},
        {{8}, {5}, {9}, {7}, {6}, {1}, {2}, {4}, {3}},
        {{4}, {2}, {6}, {8}, {5}, {3}, {7}, {9}, {1}},
        {{7}, {1}, {3}, {9}, {2}, {4}, {8}, {5}, {6}},
        {{9}, {6}, {1}, {2}, {3}, {5}, {4}, {8}, {7}},
        {{2}, {8}, {7}, {4}, {1}, {9}, {6}, {3}, {5}},
        {{3}, {4}, {5}, {8}, {6}, {7}, {1}, {2}, {9}}
    };

    // 测试5：异常数独序列，存在空格没有候选值
    std::string input5 = "530076000600195000098002060800060003400803001700020006060000280000419005000080079";
    std::vector<std::vector<std::vector<int>>> truth5 = {
        {{5}, {3}, {1, 2, 4}, {}, {7}, {6}, {1, 4, 8, 9}, {1, 2, 4, 9}, {2, 4, 8}},
        {{6}, {2, 4, 7}, {2, 4, 7}, {1}, {9}, {5}, {3, 4, 7, 8}, {2, 3, 4}, {2, 4, 7, 8}},
        {{1}, {9}, {8}, {3}, {3, 4}, {2}, {1, 3, 4, 5, 7}, {6}, {4, 7}},
        {{8}, {1, 2, 5}, {1, 2, 5, 9}, {5, 7, 9}, {6}, {1, 4, 7}, {4, 5, 7, 9}, {2, 4, 5, 9}, {3}},
        {{4}, {2, 5}, {2, 5, 6, 9}, {8}, {5}, {3}, {5, 7, 9}, {2, 5, 9}, {1}},
        {{7}, {1, 5}, {1, 3, 5, 9}, {5, 9}, {2}, {1, 4}, {4, 5, 8, 9}, {4, 5, 9}, {6}},
        {{1, 3, 9}, {6}, {1, 3, 4, 5, 7, 9}, {3, 5, 7}, {3, 5}, {7}, {2}, {8}, {4}},
        {{2, 3}, {2, 7, 8}, {2, 3, 7}, {4}, {1}, {9}, {3, 6}, {3}, {5}},
        {{1, 2, 3}, {1, 2, 4, 5}, {1, 2, 3, 4, 5}, {2, 3, 5, 6}, {8}, {}, {1, 3, 4, 6}, {7}, {9}}
    };

    try {
        test(grid_size, box_size, input1, truth1);
        test(grid_size, box_size, input2, truth2);
        test(grid_size, box_size, input3, truth3);
        test(grid_size, box_size, input4, truth4);
        test(grid_size, box_size, input5, truth5);
    }
    catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }

    return 0;
}