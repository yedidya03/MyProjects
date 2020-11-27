import numpy as np

mat = [[5,3,0,0,7,0,0,0,0],
        [6,0,0,1,9,5,0,0,0],
        [0,9,8,0,0,0,0,6,0],
        [8,0,0,0,6,0,0,0,3],
        [4,0,0,8,0,3,0,0,1],
        [7,0,0,0,2,0,0,0,6],
        [0,6,0,0,0,0,2,8,0],
        [0,0,0,4,1,9,0,0,5],
        [0,0,0,0,8,0,0,7,9]]

def isNumPosible (num, x, y, grid) :
        if num in grid[y] : return False

        col = list(grid[j][x] for j in range(9))
        if num in col : return False

        xBlock = x // 3
        yBlock = y // 3

        block = []
        for q in range (yBlock * 3, (yBlock + 1) * 3) :
                block += grid[q][xBlock * 3 : (xBlock + 1) * 3]

        if num in block : return False

        return True

def solve():
       global mat

       for y in range(9):
               for x in range(9):
                       if mat[y][x] == 0 :
                               for num in range(1, 10):
                                       if isNumPosible(num, x, y, mat):
                                               mat[y][x] = num
                                               solve()
                                               mat[y][x] = 0
                               return

       print(np.matrix(mat), "\n")

if __name__ == '__main__':

        solve()
