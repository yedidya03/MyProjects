import numpy as np
from PIL import Image, ImageDraw

size = 20

upNumbers = [[9],
             [6,3],
             [2,1,3],
             [2,1,3],
             [2,2,2],
             [1,1,3],
             [1,5,3],
             [2,3,1,3],
             [1,12],
             [2,11],
             [1,4,1,5],
             [2,4,1,4],
             [1,2,9],
             [1,3,4,3],
             [2,1,3,5],
             [1,2,4,3],
             [1,1,6],
             [1,2,3],
             [2,1,3],
             [12]]

leftNumbers = [[2],
               [6],
               [4,1],
               [3,3],
               [3,3,1],
               [2,3,1],
               [2,2,1],
               [2,5,1],
               [2,4,2,1],
               [2,11,1],
               [1,10,2,2],
               [2,2,4,2,5],
               [3,1,7,3],
               [2,1,2,1,4],
               [2,10],
               [1,8],
               [1,7],
               [4,8],
               [11],
               [9]]
'''
upNumbers = [[1,2],
             [3],
             [2,1],
             [3],
             [1,2]]

leftNumbers = [[3],
               [5],
               [1,1],
               [1,1,1],
               [1,1]]
'''
grid = [[-1 for x in range(size)] for y in range(size)]

def checkLine(line, numbers, place) :
    line = list(line)
    numbers = list(numbers)

    if numbers == [] :
        print ("false 1.0 : Not all are empty")
        return all(dot == 0 for dot in line)

    if place > (size - 1) : place = (size - 1)

    currNumberIndex = 0
    currNumber = numbers[currNumberIndex]
    duringNumber = False

    for i in range(place + 1) :
        if duringNumber and line[i] == 0:
            #print(line, place)
            #print ("False 2.0 - during number but = 0")
            return False

        if (not duringNumber) and (line[i] != 0):
            duringNumber = True

        if duringNumber:
            currNumber -= 1

        if currNumber <= 0 :
            if i < place and line[i + 1] != 0 :
                #print(line, place)
                #print ("False 3.0 - there is no 0 after the block")
                return False

            duringNumber = False
            currNumberIndex += 1
            if currNumberIndex < len(numbers) :
                currNumber = numbers[currNumberIndex]

        sum = currNumber
        for j in range(currNumberIndex + 1, len(numbers)) :
            sum += 1
            sum += numbers[j]

        if (size - i - 1) < sum :
            #print(line, place)
            #print ("False 5.0 - remaining wont fit inside")
            return False

    return True

def isDotPosibble(x, y) :
    if not checkLine(grid[y], leftNumbers[y], x): return False

    if not checkLine((grid[i][x] for i in range(size)), upNumbers[x], y): return False

    return True

def solve() :
    for y in range(size):
        for x in range(size):
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
    img = Image.new("RGB", (size, size), (255, 255, 255))
    pixelMap = img.load()

    for y in range(size):
        for x in range (size):
            if grid[y][x] == 1 :
                pixelMap[y, x] = (0,0,0)

    img.show()

if __name__ == "__main__" :
    if (len(upNumbers) == size and len(leftNumbers) == size) :
        solve()

    else :
        print ("Numbers are not in the currect size")

