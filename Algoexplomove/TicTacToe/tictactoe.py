class TicTacToe:
    def __init__(self):
        self.board = [["-" for x in range(3)] for y in range(3)]

    def display_board(self):
        for row in self.board:
            print(" ".join(row))

    def check_win(self, char):
        # check rows
        for row in self.board:
            if row == [char, char, char]:
                return True
        # check columns
        for col in range(3):
            if (self.board[0][col] == char and
                    self.board[1][col] == char and
                    self.board[2][col] == char):
                return True
        # check diagonals
        if (self.board[0][0] == char and
                self.board[1][1] == char and
                self.board[2][2] == char):
            return True
        if (self.board[0][2] == char and
                self.board[1][1] == char and
                self.board[2][0] == char):
            return True
        return False

    def play(self):
        while True:
            print("Player X turn:")
            x, y = map(int, input().split())
            self.board[x][y] = "X"
            self.display_board()
            if self.check_win("X"):
                print("Player X wins!")
                break
            print("Player O turn:")
            x, y = map(int, input().split())
            self.board[x][y] = "O"
            self.display_board()
            if self.check_win("O"):
                print("Player O wins!")
                break

game = TicTacToe()
game.play()