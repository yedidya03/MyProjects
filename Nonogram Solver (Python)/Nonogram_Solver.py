import numpy as np
from PIL import Image

width = 30
height = 20

upNumbers = [[1],
             [1],
             [2],
             [4],
             [7],
             [9],
             [2,8],
             [1,8],
             [8],
             [1,9],
             [2,7],
             [3,4],
             [6,4],
             [8,5],
             [1,11],
             [1,7],
             [8],
             [1,4,8],
             [6,8],
             [4,7],
             [2,4],
             [1,4],
             [5],
             [1,4],
             [1,5],
             [7],
             [5],
             [3],
             [1],
             [1]]

leftNumbers = [[8,7,5,7],
               [5,4,3,3],
               [3,3,2,3],
               [4,3,2,2],
               [3,3,2,2],
               [3,4,2,2],
               [4,5,2],
               [3,5,1],
               [4,3,2],
               [3,4,2],
               [4,4,2],
               [3,6,2],
               [3,2,3,1],
               [4,3,4,2],
               [3,2,3,2],
               [6,5],
               [4,5],
               [3,3],
               [3,3],
               [1,1]]

grid = [[-1 for x in range(width)] for y in range(height)]

def checkLine(line, numbers, place) :
    line = list(line)
    numbers = list(numbers)

    # If the numbers list is empty it means that the line should be empty too
    if numbers == [] :
        return all(dot == 0 for dot in line)

    # Making sure that place is'nt grater then the length of line
    if place > (len(line) - 1) : place = (len(line) - 1)

    currNumberIndex = 0
    currNumber = numbers[currNumberIndex]
    duringNumber = False

    for i in range(place + 1) :
        if duringNumber and line[i] == 0:
            return False                # The block did'nt end but there is 0 in the middle

        # Starting a new block
        if (not duringNumber) and (line[i] != 0):
            duringNumber = True

        # Every dot we decrease currNumber by 1 to track when it should be ending
        if duringNumber:
            currNumber -= 1

        # When the block ends
        if currNumber <= 0 :
            if i < place and line[i + 1] != 0 :
                return False            # The block ended but there is no 0 at the end

            duringNumber = False
            currNumberIndex += 1
            if currNumberIndex < len(numbers) :
                currNumber = numbers[currNumberIndex]

        sum = currNumber
        for j in range(currNumberIndex + 1, len(numbers)) :
            sum += 1
            sum += numbers[j]

        if (len(line) - i - 1) < sum :
            return False                # The remaining of the line is too short for the numbers following

    return True

def isDotPosibble(x, y) :
    if not checkLine(grid[y], leftNumbers[y], x): return False

    if not checkLine((grid[i][x] for i in range(height)), upNumbers[x], y): return False

    return True

def solve() :
    for y in range(height):
        for x in range(width):
            if grid[y][x] == -1 :
                for i in range(2):
                    grid[y][x] = i
                    if isDotPosibble(x, y) :
                        solve()
                    grid[y][x] = -1
                return

    drawImage()
    print(np.matrix(grid), "\n")

def drawImage () :
    img = Image.new("RGB", (width, height), (255, 255, 255))
    pixelMap = img.load()

    for y in range(height):
        for x in range (width):
            if grid[y][x] == 1 :
                pixelMap[x, y] = (0,0,0)

    img.show()

if __name__ == "__main__" :
    solve()