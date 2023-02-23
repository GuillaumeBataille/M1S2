class TicTacToe:
  def __init__(self, size=3):
    self.board = [[' ' for _ in range(size)] for _ in range(size)]
    self.size = size

  def is_winner(self, player):
    # Check rows
    for row in self.board:
      if all(cell == player for cell in row):
        return True

    # Check columns
    for col in range(self.size):
      if all(self.board[row][col] == player for row in range(self.size)):
        return True

    # Check diagonals
    if all(self.board[i][i] == player for i in range(self.size)):
      return True
    if all(self.board[i][self.size - i - 1] == player for i in range(self.size)):
      return True

    return False

  def is_tie(self):
    for row in self.board:
      if ' ' in row:
        return False
    return True

  def get_best_move(self, player):
    def minimax(board, depth, maximizingPlayer):
      if self.is_winner('X'):
        return 10 - depth
      elif self.is_winner('O'):
        return depth - 10
      elif self.is_tie():
        return 0

      if maximizingPlayer:
        value = float('-inf')
        for i in range(self.size):
          for j in range(self.size):
            if board[i][j] == ' ':
              board[i][j] = 'X'
              value = max(value, minimax(board, depth + 1, False))
              board[i][j] = ' '
        return value
      else:
        value = float('inf')
        for i in range(self.size):
          for j in range(self.size):
            if board[i][j] == ' ':
              board[i][j] = 'O'
              value = min(value, minimax(board, depth + 1, True))
              board[i][j] = ' '
        return value

    value = float('-inf')
    best_move = None
    for i in range(self.size):
      for j in range(self.size):
        if self.board[i][j] == ' ':
          self.board[i][j] = 'X'
          move_value = minimax(self.board, 0, False)
          self.board[i][j] = ' '
          if move_value > value:
            value = move_value
            best_move = (i, j)
    return best_move

  def play(self, player, row, col):
    self.board[row][col] = player

  def display(self):
    for row in self.board:
      print(' | '.join(row))
    print()

game = TicTacToe()
while not game.is_winner('X') and not game.is_winner('O') and not game.is_tie():
  move = game.get_best_move('O')
  game.play('O', move[0], move[1])
  game.display()
  if game.is_winner('O'):
    print('O wins!')
    break
  if game.is_tie():
    print('Tie!')
    break

  row = int(input('Enter row: '))
  col = int(input('Enter col: '))
  game.play('X', row, col)
  game.display()
  if game.is_winner('X'):
    print('X wins!')
    break

if game.is_tie():
  print('Tie!')
