import math
import random

# Define the player classes
class Player:
    def __init__(self, letter):
        self.letter = letter

    # Get the player's move
    def get_move(self, game):
        pass

# Human player
class HumanPlayer(Player):
    def __init__(self, letter):
        super().__init__(letter)

    def get_move(self, game):
        valid_square = False
        val = None
        while not valid_square:
            square = input(self.letter + '\'s turn. Enter a square (0-' + str(game.num_squares - 1) + ') to place ' + self.letter + ' in: ')
            try:
                val = int(square)
                if val not in game.available_moves():
                    raise ValueError
                valid_square = True
            except ValueError:
                print('Invalid square. Try again.')
        return val

# AI player
class AIPlayer(Player):
    def __init__(self, letter, depth):
        super().__init__(letter)
        self.depth = depth

    def get_move(self, game):
        if len(game.available_moves()) == game.num_squares:
            square = random.choice(game.available_moves())
        else:
            _, square = self.minimax(game, self.depth, True)
        return square

    def minimax(self, game, depth, max_player):
        if game.current_winner == self.letter:
            return (1, None)
        elif game.current_winner == game.other_player(self.letter):
            return (-1, None)
        elif len(game.available_moves()) == 0:
            return (0, None)
        elif depth == 0:
            return (0, None)

        if max_player:
            max_eval = -math.inf
            best_move = None
            for move in game.available_moves():
                game.make_move(move, self.letter)
                eval, _ = self.minimax(game, depth-1, False)
                game.undo_move(move)
                if eval > max_eval:
                    max_eval = eval
                    best_move = move
            return (max_eval, best_move)
        else:
            min_eval = math.inf
            best_move = None
            for move in game.available_moves():
                game.make_move(move, game.other_player(self.letter))
                eval, _ = self.minimax(game, depth-1, True)
                game.undo_move(move)
                if eval < min_eval:
                    min_eval = eval
                    best_move = move
            return (min_eval, best_move)

# Define the Tic Tac Toe game class
class TicTacToe:
    def __init__(self, size):
        self.size = size
        self.num_squares = size**2
        self.board = [' ' for _ in range(self.num_squares)]
        self.current_winner = None

    def print_board(self):
        for row in [self.board[i*self.size:(i+1)*self.size] for i in range(self.size)]:
            print('| ' + ' | '.join(row) + ' |')

    def make_move(self, square, letter):
        if self.board[square] == ' ':
            self.board[square] = letter
            if self.winner(square, letter):
                self.current_winner = letter
            return True
        return False

    def winner(self, square, letter):
        row_index = square // self.size
        col_index = square % self.size

        # Check row
        if all([self.board[self.size*row_index+i] == letter for i in range(self.size)]):
            return True

        # Check column
        if all([self.board[col_index+self.size*i] == letter for i in range(self.size)]):
            return True

        # Check diagonal
        if row_index == col_index:
            if all([self.board[i*self.size+i] == letter for i in range(self.size)]):
                return True

        # Check anti-diagonal
        if row_index + col_index == self.size - 1:
            if all([self.board[i*self.size+(self.size-1-i)] == letter for i in range(self.size)]):
                return True

        return False

    def available_moves(self):
        return [i for i, x in enumerate(self.board) if x == ' ']

    def num_empty_squares(self):
        return len(self.available_moves())

    def other_player(self, letter):
        return 'O' if letter == 'X' else 'X'
        
    def undo_move(self, square):
        self.board[square] = ' '

# Define the main function
def play_game():
    # Get the game settings from the user
    print('Welcome to Tic Tac Toe!')
    size = int(input('Enter the size of the grid: '))
    ai_enabled = input('Do you want to play against the computer? (y/n) ').lower() == 'y'
    ai_depth = None
    if ai_enabled:
        ai_depth = int(input('Enter the depth for the computer (recommended: 3): '))

    # Create the players
    p1 = HumanPlayer('X')
    if ai_enabled:
        p2 = AIPlayer('O', ai_depth)
    else:
        p2 = HumanPlayer('O')

    # Create the game
    game = TicTacToe(size)

    # Play the game
    players = [p1, p2]
    random.shuffle(players)
    while game.num_empty_squares() > 0 and not game.current_winner:
        for player in players:
            square = player.get_move(game)
            if game.make_move(square, player.letter):
                print(player.letter + ' makes a move to square ' + str(square))
                game.print_board()
                print('')
                if game.current_winner:
                    print(player.letter + ' wins!')
                    return
            else:
                print('That square is already occupied! Try again.')

    print('It\'s a tie!')

# Call the main function
if __name__ == '__main__':
    play_game()

