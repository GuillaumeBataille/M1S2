import time
import os
def cb(b, n):
    print("Current State Of Board:\n")
    for i in range(n * n):
        if i > 0 and i % n == 0:
            print("\n")
        if b[i] == 0:
            print("- ", end=" ")
        if b[i] == 1:
            print("O ", end=" ")
        if b[i] == -1:
            print("X ", end=" ")
    print("\n")

def u1t(b,n):
    p = input(f"Enter X's position from [1...{n*n}]: ")
    p = int(p)
    if b[p-1] != 0:
        print("Wrong Move!!!")
        exit(0)
    b[p-1] = -1

def u2t(b, n):
    p = input(f"Enter O's position from [1...{n*n}]: ")
    p = int(p)
    if b[p-1] != 0:
        print("Wrong Move!!!")
        exit(0)
    b[p-1] = 1

def mm(b, p, n, depth, file):
    x = ab(b, n)
    if x != 0 or depth == 0:
        return x * p
    pos = -1
    value = -2
    for i in range(n * n):
        if b[i] == 0:
            b[i] = p
            score = -mm(b, -p, n, depth - 1, file)
            if score > value:
                value = score
                pos = i
            b[i] = 0
    if pos == -1:
        return 0
    
    # write to file
    file.write( f"depth={depth}: value={value}, position={pos}, player={p}\n")    
    return value


def ct(b, n, depth,file):
    pos = -1
    value = -2
    for i in range(n * n):
        if b[i] == 0:
            b[i] = 1
            score = -mm(b, -1,n,depth,file)
            b[i] = 0
            if score > value:
                value = score
                pos = i
    b[pos] = 1

def ab(b, n):
    # check rows
    for i in range(n):
        row = b[i*n : (i+1)*n]
        if all(x == 1 for x in row):
            return 1
        elif all(x == -1 for x in row):
            return -1
    # check columns
    for i in range(n):
        col = b[i::n]
        if all(x == 1 for x in col):
            return 1
        elif all(x == -1 for x in col):
            return -1
    # check diagonals
    diag1 = b[::n+1]
    diag2 = b[n-1:n*n-1:n-1]
    if all(x == 1 for x in diag1) or all(x == 1 for x in diag2):
        return 1
    elif all(x == -1 for x in diag1) or all(x == -1 for x in diag2):
        return -1
    return 0

def _count_generator(reader):
    b = reader(1024 * 1024)
    while b:
        yield b
        b = reader(1024 * 1024)

def m():
    c = input("Enter 1 for single player, 2 for multiplayer: ")
    c = int(c)
    n = int(input("Enter grid size (e.g., 3 for 3x3, 4 for 4x4, etc.): "))
    depth = int(input("Enter search depth for the AI (e.g., 1 for a quick game, 5 for a stronger opponent): "))
    b = [0] * (n * n)
    if c == 1:
        print("Computer: O Vs. You: X")
        p = input("Enter to play 1(st) or 2(nd): ")
        p = int(p)
        
        # open file for writing
        print( )
        path = os.path.dirname(os.path.abspath(__file__))+"/minimax_tree.txt"
        with open(path, "w+") as file:
            for i in range(n * n):
                if ab(b,n) != 0:
                    break
                if (i + p) % 2 == 0:
                    start_time = time.time()  # start the timer
                    ct(b,n,depth,file)
                    elapsed_time = time.time() - start_time  # calculate elapsed time
                    print(f"AI took: {elapsed_time:.2f} seconds to play")
                    file_size = os.path.getsize(path)
                    print("File size: {:.2f} Mb".format(file_size/ (1024 * 1024)))
                    file.seek(0)
                    lines = file.readlines()
                    num_lines = len(lines)
                    print("Nodes numbers : ", num_lines, "lines.")
                    print("Writing flowrate: {:.2f} Mb/s".format((file_size/ (1024 * 1024))/ elapsed_time))
                else:
                    cb(b, n)
                    u1t(b,n)
    else:
        for i in range(n * n):
            if ab(b,n) != 0:
                break
            if i % 2 == 0:
                cb(b, n)
                u1t(b,n)
            else:
                cb(b, n)
                u2t(b,n)
    x = ab(b,n)
    if x == 0:
        cb(b, n)
        print("Draw!!!")
    if x == -1:
        cb(b, n)
        print("X wins")
    if x == 1:
        cb(b, n)
        print("O wins")

m()
