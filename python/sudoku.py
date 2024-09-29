''' code url: https://github.com/LittleZoem/Sudoku
    author: 李涛
    ID:24214438
'''

import re
from typing import List

class Grid:
    def __init__(self, box_size: int, grid_size: int):
        '''检查参数合法性'''
        self.check_grid(box_size, grid_size)
        self.BOX_SIZE = box_size
        self.GRID_SIZE = grid_size
        self.grid = [[0] * grid_size for _ in range(grid_size)]

    def get_row(self, row: int) -> List[int]:
        '''返回数独指定行的元素列表'''
        return self.grid[row]

    def get_column(self, col: int) -> List[int]:
        '''返回数独指定列的元素列表'''
        return [self.grid[i][col] for i in range(self.GRID_SIZE)]

    def get_box(self, row: int, col: int) -> List[int]:
        '''返回指定方格所在的BOX的元素列表'''
        res_box = []
        box_start_row = (row // self.BOX_SIZE) * self.BOX_SIZE
        box_start_col = (col // self.BOX_SIZE) * self.BOX_SIZE
        for i in range(box_start_row, box_start_row + self.BOX_SIZE):
            for j in range(box_start_col, box_start_col + self.BOX_SIZE):
                res_box.append(self.grid[i][j])
        return res_box

    @staticmethod
    def check_grid(box_size: int, grid_size: int):
        '''检查grid参数是否合法'''
        if grid_size <= 0 or grid_size != box_size * box_size:
            raise ValueError("Invalid grid parameters!")


class Sudoku(Grid):
    def __init__(self, box_size: int, grid_size: int):
        super().__init__(box_size, grid_size)

    def parse(self, input_str: str):
        '''解析字符串输入，得到Sudoku'''
        self.check_input(input_str)
        for row in range(self.GRID_SIZE):
            for col in range(self.GRID_SIZE):
                self.grid[row][col] = int(input_str[row * self.GRID_SIZE + col])

    def get_inference(self) -> List[List[List[int]]]:
        '''推理棋盘，得到各单元格候选值'''
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

    def check_input(self, input_str: str):
        '''检查输入字符串的有效性'''
        if len(input_str) != self.GRID_SIZE * self.GRID_SIZE:
            raise ValueError("Invalid Sudoku size!")

        if not re.match("^[0-9]+$", input_str):
            raise ValueError("Invalid Sudoku number!")

    def get_candidates(self, row: int, col: int) -> List[int]:
        '''排除同行同列同box中的数，得到指定单元格的候选值'''
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

    def to_string(self) -> str:
        """将当前数独状态转换为字符串表示"""
        return ''.join(str(num) for row in self.grid for num in row)

    @classmethod
    def from_string(cls, input_str: str):
        """从字符串创建数独对象"""
        instance = cls(int(len(input_str)**0.5), len(input_str))
        instance.parse(input_str)
        return instance

    def clone(self):
        """克隆当前对象"""
        new_instance = Sudoku(self.BOX_SIZE, self.GRID_SIZE)
        new_instance.grid = [row[:] for row in self.grid]
        return new_instance

    def __eq__(self, other):
        """比较两个数独对象是否相等"""
        if not isinstance(other, Sudoku):
            return False
        return self.grid == other.grid




def test_result(output, truth):
    '''判断输出向量和正确值是否一致'''
    for row in range(len(truth)):
        for col in range(len(truth[row])):
            if len(output[row][col]) != len(truth[row][col]):
                return False

            for k in range(len(truth[row][col])):
                if output[row][col][k] != truth[row][col][k]:
                    return False

    return True

def test(grid_size, box_size, input_str, truth):
    '''给定输入和输出测试'''
    sudoku = Sudoku(box_size, grid_size)
    sudoku.parse(input_str)
    inference = sudoku.get_inference()
    if test_result(inference, truth):
        print("PASS!")
    else:
        print("WRONG!")



if __name__ == "__main__":
    grid_size = 9
    box_size = 3

    '''测试1：输入序列长度异常'''
    input1 = "01790360000008000090000050707201043000040207006437025070100006500003000000560172"
    truth1 = [[[]]]

    '''测试2：输入序列包含非数字'''
    input2 = "a179##600000080000900000507072010430000402070064370250701000065000030000005601720"
    truth2 = [[[]]]

    '''测试3：正常数独序列'''
    input3 = "017903600000080000900000507072010430000402070064370250701000065000030000005601720"
    truth3 = [
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

    '''测试4：完全解出的数独序列（不含0）'''
    input4 = "534678912672195348198342567859761243426853791713924856961235487287419635345867129"
    truth4 = [
        [[5], [3], [4], [6], [7], [8], [9], [1], [2]],
        [[6], [7], [2], [1], [9], [5], [3], [4], [8]],
        [[1], [9], [8], [3], [4], [2], [5], [6], [7]],
        [[8], [5], [9], [7], [6], [1], [2], [4], [3]],
        [[4], [2], [6], [8], [5], [3], [7], [9], [1]],
        [[7], [1], [3], [9], [2], [4], [8], [5], [6]],
        [[9], [6], [1], [2], [3], [5], [4], [8], [7]],
        [[2], [8], [7], [4], [1], [9], [6], [3], [5]],
        [[3], [4], [5], [8], [6], [7], [1], [2], [9]]
    ]

    '''测试5：异常数独序列，存在空格没有候选值'''
    input5 = "530076000600195000098002060800060003400803001700020006060000280000419005000080079"
    truth5 = [
        [[5], [3], [1, 2, 4], [], [7], [6], [1, 4, 8, 9], [1, 2, 4, 9], [2, 4, 8]],
        [[6], [2, 4, 7], [2, 4, 7], [1], [9], [5], [3, 4, 7, 8], [2, 3, 4], [2, 4, 7, 8]],
        [[1], [9], [8], [3], [3, 4], [2], [1, 3, 4, 5, 7], [6], [4, 7]],
        [[8], [1, 2, 5], [1, 2, 5, 9], [5, 7, 9], [6], [1, 4, 7], [4, 5, 7, 9], [2, 4, 5, 9], [3]],
        [[4], [2, 5], [2, 5, 6, 9], [8], [5], [3], [5, 7, 9], [2, 5, 9], [1]],
        [[7], [1, 5], [1, 3, 5, 9], [5, 9], [2], [1, 4], [4, 5, 8, 9], [4, 5, 9], [6]],
        [[1, 3, 9], [6], [1, 3, 4, 5, 7, 9], [3, 5, 7], [3, 5], [7], [2], [8], [4]],
        [[2, 3], [2, 7, 8], [2, 3, 7], [4], [1], [9], [3, 6], [3], [5]],
        [[1, 2, 3], [1, 2, 4, 5], [1, 2, 3, 4, 5], [2, 3, 5, 6], [8], [], [1, 3, 4, 6], [7], [9]]
    ]

    try:
        test(grid_size, box_size, input1, truth1)
        test(grid_size, box_size, input2, truth2)
        test(grid_size, box_size, input3, truth3)
        test(grid_size, box_size, input4, truth4)
        test(grid_size, box_size, input5, truth5)
    except Exception as e:
        print(f"Caught an exception: {e}")