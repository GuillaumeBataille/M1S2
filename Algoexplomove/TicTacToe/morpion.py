import time

def ConstBoard(board, n):
    print("Current State Of Board : \n\n")
    for i in range(n):
        for j in range(n):
            if board[i * n + j] == 0:
                print("| - ", end="")
            elif board[i * n + j] == 1:
                print("| O ", end="")
            elif board[i * n + j] == -1:
                print("| X ", end="")
        print("|")
        for k in range(n):
            print("----", end="")
        print("")
    print("\n")

#This function takes the user move as input and make the required changes on the board.
def User1Turn(board):
    pos=input("Enter X's position from [1...n]: ")
    pos=int(pos)
    if(board[pos-1]!=0):
        print("Wrong Move!!!")
        exit(0) 
    board[pos-1]=-1

def User2Turn(board):
    pos=input("Enter O's position from [1...n]: ")
    pos=int(pos)
    if(board[pos-1]!=0):
        print("Wrong Move!!!")
        exit(0)
    board[pos-1]=1

#MinMax function.
def minimax(board,player,n):
    x=analyzeboard(board,n)
    if(x!=0):
        return (x*player)
    pos=-1
    value=-2
    for i in range(0,n*n):
        if(board[i]==0):
            board[i]=player
            score=-minimax(board,(player*-1),n)
            if(score>value):
                value=score
                pos=i
            board[i]=0

    if(pos==-1):
        return 0
    return value
    
#This function makes the computer's move using minmax algorithm.
def CompTurn(board,n):
    pos=-1
    value=-2
    for i in range(0,n*n):
        if(board[i]==0):
            board[i]=1
            score=-minimax(board, -1,n)
            board[i]=0
            if(score>value):
                value=score
                pos=i
 
    board[pos]=1


def analyzeboard(board, n):
    cb = [[i+j for i in range(n)] for j in range(0, n*(n-1)+1, n)]
    cb += [[i*n+j for i in range(n)] for j in range(n)]
    cb += [[i*(n+1) for i in range(n)], [(i+1)*(n-1) for i in range(n)]]

    for i in range(0, len(cb)):
        if(board[cb[i][0]] != 0 and
           board[cb[i][0]] == board[cb[i][1]] and
           board[cb[i][0]] == board[cb[i][2]]):
            return board[cb[i][2]]
    return 0


#Main Function.
def main():
    
    n=input("Size n (n*n) : ")
    n=int(n)
    board = []
    for i in range(n*n):
        board.append(0)
    print(board)
    choice=input("Enter 1 for single player, 2 for multiplayer: ")
    choice=int(choice)
    #The broad is considered in the form of a single dimentional array.
    #One player moves 1 and other move -1.


    if(choice==1):
        print("Computer : O Vs. You : X")
        player= input("Enter to play 1(st) or 2(nd) :")
        player = int(player)
        for i in range (0,n*n):
            if(analyzeboard(board,n)!=0):
                break
            if((i+player)%2==0):
                print("Thinking...")
                start_time = time.time()
                CompTurn(board,n)
                end_time = time.time()
                print("AI's move took {:.4f} seconds.".format(end_time - start_time))
            else:
                ConstBoard(board,n)
                User1Turn(board)
    else:
        for i in range (0,9):
            if(analyzeboard(board,n)!=0):
                break
            if((i)%2==0):
                ConstBoard(board,n)
                User1Turn(board)
            else:
                ConstBoard(board,n)
                User2Turn(board)
         

    x=analyzeboard(board,n)
    if(x==0):
         ConstBoard(board,n)
         print("Draw!!!")
    if(x==-1):
         ConstBoard(board,n)
         print("X Wins!!! Y Loose !!!")
    if(x==1):
         ConstBoard(board,n)
         print("X Loose!!! O Wins !!!!")
       
#---------------#
main()
#---------------#


