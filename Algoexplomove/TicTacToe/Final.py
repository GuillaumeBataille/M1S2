#This function is used to draw the board's current state every time the user turn arrives. 
def ConstBoard(board):
    print("Current State Of Board : \n\n");
    for i in range (0,16):
        if((i>0) and (i%4)==0):
            print("\n");
        if(board[i]==0):
            print("- ",end=" ");
        if (board[i]==1):
            print("O ",end=" ");
        if(board[i]==-1):    
            print("X ",end=" ");
    print("\n\n");

#This function takes the user move as input and make the required changes on the board.
def User1Turn(board):
    pos=int(input("Enter X's position from [1...16]: "));
    if(board[pos-1]!=0):
        print("Wrong Move!!!");
        exit(0) ;
    board[pos-1]=-1;

def User2Turn(board):
    pos=int(input("Enter O's position from [1...16]: "));
    if(board[pos-1]!=0):
        print("Wrong Move!!!");
        exit(0);
    board[pos-1]=1;

#MinMax function.
# MinMax function with depthmax parameter.
def minimax(board, player, depth, depthmax):
    x = analyzeboard(board)
    if x != 0 or depth == depthmax:
        return (x * player)
    pos = -1
    value = -2
    for i in range(0, 16):
        if board[i] == 0:
            board[i] = player
            score = -minimax(board, (player * -1), depth + 1, depthmax)
            board[i] = 0
            if score > value:
                value = score
                pos = i
    if pos == -1:
        return 0
    return value

# This function makes the computer's move using minmax algorithm.
def CompTurn(board, depthmax):
    pos = -1
    value = -2
    for i in range(0, 16):
        if board[i] == 0:
            board[i] = 1
            score = -minimax(board, -1, 0, depthmax)
            board[i] = 0
            if score > value:
                value = score
                pos = i
    board[pos] = 1



#This function is used to analyze a game.
def analyzeboard(board):
    cb=[[0,1,2,3],[4,5,6,7],[8,9,10,11],[12,13,14,15],[0,4,8,12],[1,5,9,13],[2,6,10,14],[3,7,11,15],[0,5,10,15],[3,6,9,12]];

    for i in range(0,10):
        if(board[cb[i][0]] != 0 and
           board[cb[i][0]] == board[cb[i][1]] and
           board[cb[i][0]] == board[cb[i][2]] and
           board[cb[i][0]] == board[cb[i][3]]):
            return board[cb[i][3]];
    return 0;

def main():
    choice = int(input("Enter 1 for single player, 2 for multiplayer: "))
    board = [0] * 16
    player = 1

    if choice == 1:
        print("Computer: O vs. You: X")
        player = int(input("Enter to play 1 (X) or 2 (O): "))
        if player == 1:
            depthmax = int(input("Enter the depthmax parameter (e.g. 2): "))
        else:
            depthmax = float('inf')

    while True:
        ConstBoard(board)

        if analyzeboard(board) != 0:
            if analyzeboard(board) == player:
                print("Congratulations! You won!")
            else:
                print("You lost. Better luck next time.")
            break

        if player == 1:
            User1Turn(board)
        elif choice == 1:
            CompTurn(board, depthmax)
        else:
            User2Turn(board)

        if analyzeboard(board) != 0:
            if analyzeboard(board) == player:
                ConstBoard(board)
                print("Congratulations! You won!")
            else:
                ConstBoard(board)
                print("Player", -player, "wins!")
            break

        player = -player

        if all(board):
            ConstBoard(board)
            print("It's a tie!")
            break

main()