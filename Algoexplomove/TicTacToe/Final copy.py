#This function is used to draw the board's current state every time the user turn arrives. 
def ConstBoard(board, size):
    print("Current State Of Board : \n\n")
    for i in range(0, (size * size)):
        if i > 0 and i % size == 0:
            print("\n")
        if board[i] == 0:
            print("- ", end=" ")
        elif board[i] == 1:
            print("O ", end=" ")
        else:
            print("X ", end=" ")
    print("\n\n")


#This function takes the user move as input and make the required changes on the board.
def User1Turn(board):
    pos=int(input("Enter X's position from [1...{len(board)}]: "));
    if(board[pos-1]!=0):
        print("Wrong Move!!!");
        exit(0) ;
    board[pos-1]=-1;

def User2Turn(board):
    pos=int(input("Enter O's position from [1...{len(board)}]: "));
    if(board[pos-1]!=0):
        print("Wrong Move!!!");
        exit(0);
    board[pos-1]=1;

#MinMax function.
# MinMax function with depthmax parameter.
def minimax(board, player, depth, depthmax):
    x = analyzeboard(board)
    if x != 0 or depth == depthmax:
        return x * player
    pos = -1
    value = -2
    for i in range(0, len(board)):
        if board[i] == 0:
            board[i] = player
            score = minimax(board, (player * -1), depth + 1, depthmax)
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
    for i in range(0, len(board)):
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
    n = int(len(board) ** 0.5)  # Compute the size of the board
    cb = []
    # Generate a list of all the possible winning combinations
    for i in range(n):
        row = [i * n + j for j in range(n)]
        cb.append(row)
        col = [i + j * n for j in range(n)]
        cb.append(col)
    diag1 = [i * (n + 1) for i in range(n)]
    diag2 = [(i + 1) * (n - 1) for i in range(n)]
    cb.append(diag1)
    cb.append(diag2)

    for i in range(len(cb)):
        if board[cb[i][0]] != 0 and all(board[cb[i][j]] == board[cb[i][0]] for j in range(1, n)):
            return board[cb[i][0]]

    if 0 not in board:
        return -1

    return 0


def main():
    choice = int(input("Enter 1 for single player, 2 for multiplayer: "))
    n = int(input("Enter the size of the board (e.g. 3 for 3x3): "))
    board = [0] * n*n
    player = 1

    if choice == 1:
        print("Computer: O vs. You: X")
        player = int(input("Enter to play 1 (X) or 2 (O): "))
        if player == 1:
            depthmax = int(input("Enter the depthmax parameter (e.g. 2): "))
        else:
            depthmax = float('inf')

    while True:
        ConstBoard(board,n)

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
                ConstBoard(board,n)
                print("Congratulations! You won!")
            else:
                ConstBoard(board,n)
                print("Player", -player, "wins!")
            break

        player = -player

        if all(board):
            ConstBoard(board,n)
            print("It's a tie!")
            break

main()