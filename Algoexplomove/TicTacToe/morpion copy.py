import math

# This function is used to draw the board's current state every time the user turn arrives. 
def ConstBoard(board):
    print("Current State Of Board : \n\n")
    for i in range (0, len(board)):
        if((i>0) and (i%n)==0):
            print("\n")
        if(board[i]==0):
            print("- ",end=" ")
        if (board[i]==1):
            print("O ",end=" ")
        if(board[i]==-1):    
            print("X ",end=" ")
    print("\n\n")

# This function takes the user move as input and make the required changes on the board.
def User1Turn(board):
    pos=int(input(f"Enter X's position from [1...{n*n}]: "))
    if(board[pos-1]!=0):
        print("Wrong Move!!!")
        exit(0) 
    board[pos-1]=-1

def User2Turn(board):
    pos=int(input(f"Enter O's position from [1...{n*n}]: "))
    if(board[pos-1]!=0):
        print("Wrong Move!!!")
        exit(0)
    board[pos-1]=1

# MinMax function.
def minimax(board, player):
    x=analyzeboard(board)
    if(x!=0):
        return (x*player)
    pos=-1
    value=-2
    for i in range(0, len(board)):
        if(board[i]==0):
            board[i]=player
            score=-minimax(board,(player*-1))
            if(score>value):
                value=score
                pos=i
            board[i]=0

    if(pos==-1):
        return 0
    return value
    
# This function makes the computer's move using minmax algorithm.
def CompTurn(board):
    pos=-1
    value=-2
    for i in range(0, len(board)):
        if(board[i]==0):
            board[i]=1
            score=-minimax(board, -1)
            board[i]=0
            if(score>value):
                value=score
                pos=i
 
    board[pos]=1


# This function is used to analyze a game.
def analyzeboard(board):
    cb=[]
    # Rows
    for i in range(n):
        cb.append([i*n+j for j in range(n)])
    # Columns
    for j in range(n):
        cb.append([i*n+j for i in range(n)])
    # Diagonal top-left to bottom-right
    cb.append([i*(n+1) for i in range(n)])
    # Diagonal top-right to bottom-left
    cb.append([(i+1)*(n-1) for i in range(n-1, -1, -1)])
    
    for i in range(len(cb)):
        if(board[cb[i][0]] != 0 and all(board[cb[i][j]] == board[cb[i][0]] for j in range(1, n))):
            return board[cb[i][0]]
    return 0

# Main Function.
def main():
    global n
    n = input("Enter the board size n: ")
    n = int(n)
    board = [0] * (n*n) # The board is now a list of size nn.
    choice = input("Enter 1 for single player, 2 for multiplayer: ")
    choice = int(choice)

    if choice == 1:
        print("Computer : O Vs. You : X")
        player = input("Enter to play 1(st) or 2(nd) : ")
        player = int(player)
        for i in range(n*n):
            if analyzeboard(board, n) != 0:
                break
            if (i + player) % 2 == 0:
                CompTurn(board, n)
            else:
                ConstBoard(board, n)
                User1Turn(board, n)
    else:
        for i in range(n*n):
            if analyzeboard(board, n) != 0:
                break
            if i % 2 == 0:
                ConstBoard(board, n)
                User1Turn(board, n)
            else:
                ConstBoard(board, n)
                User2Turn(board, n)
    x = analyzeboard(board, n)
    if x == 0:
        ConstBoard(board, n)
        print("Draw!!!")
    if x == -1:
        ConstBoard(board, n)
        print("X Wins!!! O Loses!!!")
    if x == 1:
        ConstBoard(board, n)
        print("O Wins!!! X Loses!!!")

#---------------#
main()
#---------------#