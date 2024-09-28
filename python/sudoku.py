import re

class Grid:
    def __init__(self, box_size, grid_size):
        self.check_grid(box_size, grid_size)
        self.BOX_SIZE = box_size
        self.GRID_SIZE = grid_size
        self.grid = [[0 for _ in range(self.GRID_SIZE)] for _ in range(self.GRID_SIZE)]

    def get_row(self, row):
        return self.grid[row]

    def get_column(self, col):
        return [self.grid[i][col] for i in range(self.GRID_SIZE)]

    def get_box(self, row, col):
        res_box = []
        box_start_row = (row // self.BOX_SIZE) * self.BOX_SIZE
        box_start_col = (col // self.BOX_SIZE) * self.BOX_SIZE
        for i in range(box_start_row, box_start_row + self.BOX_SIZE):
            for j in range(box_start_col, box_start_col + self.BOX_SIZE):
                res_box.append(self.grid[i][j])
        return res_box

    @staticmethod
    def check_grid(box_size, grid_size):
        if grid_size <= 0 or grid_size != box_size * box_size:
            raise ValueError("Invalid grid parameters!")


class Sudoku(Grid):
    def __init__(self, box_size, grid_size):
        super().__init__(box_size, grid_size)

    def parse(self, input_str):
        self.check_input(input_str)
        for row in range(self.GRID_SIZE):
            for col in range(self.GRID_SIZE):
                self.grid[row][col] = int(input_str[row * self.GRID_SIZE + col])

    def get_inference(self):
        inference = []
        for row in range(self.GRID_SIZE):
            row_arr = []
            for col in range(self.GRID_SIZE):
                if self.grid[row][col] == 0:
                    candidates = self.get_candidates(row, col)
                    row_arr.append(candidates)
                else:
                    row_arr.append([self.grid[row][col]])
            inference.append(row_arr)
        return inference

    def check_input(self, input_str):
        if len(input_str) != self.GRID_SIZE * self.GRID_SIZE:
            raise ValueError("Invalid Sudoku size!")

        if not re.match("^[0-9]+$", input_str):
            raise ValueError("Invalid Sudoku number!")

    def get_candidates(self, row, col):
        candidates = []
        same_row = self.get_row(row)
        same_col = self.get_column(col)
        same_box = self.get_box(row, col)

        for num in range(1, self.GRID_SIZE + 1):
            if (
                num not in same_row and
                num not in same_col and
                num not in same_box
            ):
                candidates.append(num)
        return candidates




def test_result(output, truth):
    for row in range(len(truth)):
        for col in range(len(truth[row])):
            if len(output[row][col]) != len(truth[row][col]):
                return False

            for k in range(len(truth[row][col])):
                if output[row][col][k] != truth[row][col][k]:
                    return False

    return True


def test():
    grid_size = 9
    box_size = 3
    input_str = "017903600000080000900000507072010430000402070064370250701000065000030000005601720"

    sudoku = Sudoku(box_size, grid_size)
    sudoku.parse(input_str)
    inference = sudoku.get_inference()

    truth = [
        [[2, 4, 5, 8], [1], [7], [9], [2, 4, 5], [3], [6], [4, 8], [2, 4, 8]],
        [[2, 3, 4, 5, 6], [2, 3, 4, 5], [3, 6], [1, 2, 5, 7], [8], [4, 5, 6, 7], [1, 3, 9], [1, 4, 9], [1, 2, 3, 4, 9]],
        [[9], [2, 3, 4, 8], [3, 6, 8], [1, 2], [2, 4, 6], [4, 6], [5], [1, 4, 8], [7]],
        [[5, 8], [7], [2], [5, 8], [1], [5, 6, 8, 9], [4], [3], [6, 8, 9]],
        [[1, 3, 5, 8], [3, 5, 8, 9], [3, 8, 9], [4], [5, 6, 9], [2], [1, 8, 9], [7], [1, 6, 8, 9]],
        [[1, 8], [6], [4], [3], [7], [8, 9], [2], [5], [1, 8, 9]],
        [[7], [2, 3, 4, 8, 9], [1], [2, 8], [2, 4, 9], [4, 8, 9], [3, 8, 9], [6], [5]],
        [[2, 4, 6, 8], [2, 4, 8, 9], [6, 8, 9], [2, 5, 7, 8], [3], [4, 5, 7, 8, 9], [1, 8, 9], [1, 4, 8, 9], [1, 4, 8, 9]],
        [[3, 4, 8], [3, 4, 8, 9], [5], [6], [4, 9], [1], [7], [2], [3, 4, 8, 9]]
    ]

    if test_result(inference, truth):
        print("PASS!")
    else:
        print("WRONG!")


if __name__ == '__main__':
    test()