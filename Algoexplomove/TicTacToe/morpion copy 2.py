import time

# Cette fonction est utilisée pour dessiner l'état actuel de la grille à chaque fois que le tour de l'utilisateur arrive.
def draw_board(board):
    print("Current State Of Board : \n\n")
    n = len(board)
    for i in range(n):
        for j in range(n):
            if(board[i][j]==0):
                print("- ", end=" ")
            elif(board[i][j]==1):
                print("O ", end=" ")
            else:    
                print("X ", end=" ")
        print("\n")
    print("\n")

# Cette fonction prend le mouvement de l'utilisateur en entrée et effectue les changements nécessaires sur la grille.
def user_turn(board, player):
    pos=input(f"Enter {player}'s position as row,col (e.g., 1,2): ")
    row, col = map(int, pos.split(","))
    if board[row-1][col-1] != 0:
        print("Wrong Move!!!")
        exit(0) 
    board[row-1][col-1] = player

# MinMax function.
def minimax(board, player):
    n = len(board)
    x = analyze_board(board, n)
    if x != 0:
        return x * player
    pos = (-1, -1)
    value = -2
    for i in range(n):
        for j in range(n):
            if board[i][j] == 0:
                board[i][j] = player
                score = -minimax(board, player * -1)
                board[i][j] = 0
                if score > value:
                    value = score
                    pos = (i, j)
    if pos == (-1, -1):
        return 0
    return value

# Cette fonction effectue le tour de l'ordinateur en utilisant l'algorithme Minmax.
def comp_turn(board, player):
    n = len(board)
    pos = (-1, -1)
    value = -2
    for i in range(n):
        for j in range(n):
            if board[i][j] == 0:
                board[i][j] = player
                score = -minimax(board, -player)
                board[i][j] = 0
                if score > value:
                    value = score
                    pos = (i, j)
    row, col = pos
    board[row][col] = player

# Cette fonction est utilisée pour analyser une partie.
def analyze_board(board, n):
    for i in range(n):
        if all(board[i][j] == board[i][0] and board[i][j] != 0 for j in range(n)):
            return board[i][0]
    for j in range(n):
        if all(board[i][j] == board[0][j] and board[i][j] != 0 for i in range(n)):
            return board[0][j]
    if all(board[i][i] == board[0][0] and board[i][i] != 0 for i in range(n)):
        return board[0][0]
    if all(board[i][n-i-1] == board[0][n-1] and board[i][n-i-1] != 0 for i in range(n)):
        return board[0][n-1]
    return 0

#Main Function.
def main():
    # Get the board size from the user
    board_size = input("Enter the size of the board: ")
    board_size = int(board_size)

    # Create an empty board with the specified size
    board = [0] * (board_size * board_size)

    # Determine the number of players
    num_players = input("Enter the number of players (1 or 2): ")
    num_players = int(num_players)

    # Set up the players
    players = [-1, 1]  # Player 1 is X (-1), player 2 is O (1)
    if num_players == 1:
        print("Computer: O Vs. You: X")
        player = input("Enter 1 to play first or 2 to play second: ")
        player = int(player) - 1  # Subtract 1 to convert to 0-indexing
    elif num_players == 2:
        print("Player 1: X Vs. Player 2: O")
        player = 0  # Player 1 goes first
    else:
        print("Invalid number of players.")
        return

    # Play the game
    for i in range(board_size * board_size):
        ConstBoard(board)
        if analyzeboard(board, board_size) != 0:
            break
        if i % 2 == player:
            if num_players == 1:
                print("Thinking...")
                start_time = time.time()
                draw_board(board, board_size)
                end_time = time.time()
                print("AI's move took {:.4f} seconds.".format(end_time - start_time))
            else:
                user_input = input("Player 1, enter your move (row, column): ")
                row, col = [int(x) for x in user_input.split(',')]
                pos = (row - 1) * board_size + (col - 1)
                if board[pos] != 0:
                    print("Invalid move!")
                    return
                board[pos] = players[player]
        else:
            if num_players == 1:
                user_input = input("Enter your move (row, column): ")
                row, col = [int(x) for x in user_input.split(',')]
                pos = (row - 1) * board_size + (col - 1)
                if board[pos] != 0:
                    print("Invalid move!")
                    return
                board[pos] = players[player]
            else:
                user_input = input("Player 2, enter your move (row, column): ")
                row, col = [int(x) for x in user_input.split(',')]
                pos = (row - 1) * board_size + (col - 1)
                if board[pos] != 0:
                    print("Invalid move!")
                    return
                board[pos] = players[player]

        player = (player + 1) % 2

    x = analyzeboard(board, board_size)
    ConstBoard(board)
    if x == 0:
        print("Draw!")
    elif x == -1:
        print("Player 1 (X) wins!")
    elif x == 1:
        if num_players == 1:
            print("Computer (O) wins!")
        else:
            print("Player 2 (O) wins!")
