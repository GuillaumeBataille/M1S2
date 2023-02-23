import time

class TicTacToe:
    def __init__(self, size=3, depth=None):
        self.size = size
        self.board = [[' ' for _ in range(size)] for _ in range(size)]
        self.depth = depth

    def display_board(self):
        for i in range(self.size):
            row = "|".join(self.board[i][j] for j in range(self.size))
            print("---" * self.size)
            print("|" + row + "|")
        print("---" * self.size)
        
    def is_empty(self, x, y):
        return self.board[x][y] == ' '

    def has_won(self, symbol):
        # Check rows
        for row in self.board:
            if all(cell == symbol for cell in row):
                return True

        # Check columns
        for i in range(self.size):
            if all(self.board[j][i] == symbol for j in range(self.size)):
                return True

        # Check diagonals
        if all(self.board[i][i] == symbol for i in range(self.size)):
            return True
        if all(self.board[i][self.size-i-1] == symbol for i in range(self.size)):
            return True

        return False

    def minimax(self, symbol, depth):
        if self.has_won(symbol):
            return (1 if symbol == 'X' else -1) * (self.depth - depth + 1)

        if all(all(cell != ' ' for cell in row) for row in self.board):
            return 0

        if symbol == 'X':
            best_value = float('-inf')
            for i in range(self.size):
                for j in range(self.size):
                    if self.is_empty(i, j):
                        self.board[i][j] = symbol
                        value = self.minimax('O', depth+1)
                        self.board[i][j] = ' '
                        best_value = max(best_value, value)
        else:
            best_value = float('inf')
            for i in range(self.size):
                for j in range(self.size):
                    if self.is_empty(i, j):
                        self.board[i][j] = symbol
                        value = self.minimax('X', depth+1)
                        self.board[i][j] = ' '
                        best_value = min(best_value, value)

        return best_value

    def get_best_move(self, symbol):
        best_move = None
        best_value = float('-inf') if symbol == 'X' else float('inf')
        for i in range(self.size):
            for j in range(self.size):
                if self.is_empty(i, j):
                    self.board[i][j] = symbol
                    value = self.minimax(symbol, 0)
                    self.board[i][j] = ' '
                    self.board[i][j] = ' '
                    if symbol == 'X':
                        if value > best_value:
                            best_value = value
                            best_move = (i, j)
                    else:
                        if value < best_value:
                            best_value = value
                            best_move = (i, j)
        return best_move

def play_game():
    size = int(input("Enter the size of the Tic-Tac-Toe board (3 for a 3x3 board, 4 for a 4x4 board, etc.): "))
    depth = int(input("Enter the depth of the Minimax algorithm (how many moves ahead to consider): "))

    game = TicTacToe(size, depth)
    game.display_board()

    while True:
        human_move = tuple(map(int, input("Enter your move (row column): ").split()))
        x, y = human_move
        if not game.is_empty(x, y):
            print("Invalid move. Try again.")
            continue

        game.board[x][y] = 'X'
        game.display_board()

        if game.has_won('X'):
            print("You have won the game!")
            break

        if all(all(cell != ' ' for cell in row) for row in game.board):
            print("The game is a draw.")
            break

        print("Thinking...")
        start_time = time.time()
        ai_move = game.get_best_move('O')
        end_time = time.time()
        print("AI's move took {:.4f} seconds.".format(end_time - start_time))

        game.board[ai_move[0]][ai_move[1]] = 'O'
        game.display_board()

        if game.has_won('O'):
            print("The AI has won the game.")
            break

if __name__ == "__main__":
    play_game()

