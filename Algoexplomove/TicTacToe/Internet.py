def cb(b):
    print("Current State Of Board:\n")
    for i in range(9):
        if i > 0 and i % 3 == 0:
            print("\n")
        if b[i] == 0:
            print("- ", end=" ")
        if b[i] == 1:
            print("O ", end=" ")
        if b[i] == -1:
            print("X ", end=" ")
    print("\n")

def u1t(b):
    p = input("Enter X's position from [1...9]: ")
    p = int(p)
    if b[p-1] != 0:
        print("Wrong Move!!!")
        exit(0)
    b[p-1] = -1

def u2t(b):
    p = input("Enter O's position from [1...9]: ")
    p = int(p)
    if b[p-1] != 0:
        print("Wrong Move!!!")
        exit(0)
    b[p-1] = 1

def mm(b, p):
    x = ab(b)
    if x != 0:
        return x * p
    pos = -1
    value = -2
    for i in range(9):
        if b[i] == 0:
            b[i] = p
            score = -mm(b, -p)
            if score > value:
                value = score
                pos = i
            b[i] = 0
    if pos == -1:
        return 0
    return value

def ct(b):
    pos = -1
    value = -2
    for i in range(9):
        if b[i] == 0:
            b[i] = 1
            score = -mm(b, -1)
            b[i] = 0
            if score > value:
                value = score
                pos = i
    b[pos] = 1

def ab(b):
    c = [[0,1,2],[3,4,5],[6,7,8],[0,3,6],[1,4,7],[2,5,8],[0,4,8],[2,4,6]]
    for i in range(8):
        if b[c[i][0]] != 0 and b[c[i][0]] == b[c[i][1]] and b[c[i][0]] == b[c[i][2]]:
            return b[c[i][2]]
    return 0

def m():
    c = input("Enter 1 for single player, 2 for multiplayer: ")
    c = int(c)
    b = [0]*9
    if c == 1:
        print("Computer: O Vs. You: X")
        p = input("Enter to play 1(st) or 2(nd): ")
        p = int(p)
        for i in range(9):
            if ab(b) != 0:
                break
            if (i + p) % 2 == 0:
                ct(b)
            else:
                cb(b)
                u1t(b)
    else:
        for i in range(9):
            if ab(b) != 0:
                break
            if i % 2 == 0:
                cb(b)
                u1t(b)
            else:
                cb(b)
                u2t(b)
    x = ab(b)
    if x == 0:
         cb(b)
         print("Draw!!!")
    if x == -1:
        cb(b)
        print("X wins")
    if x == 1:
        cb(b)
        print("O wins")
       

m()
#---------------#


