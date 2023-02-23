def display_game(state):
  for row in state:
    print(*row)
  print("\n")

initial_state = [
  [1, 2, 3],
  [4, 5, 6],
  [7, 8, 0]
]

display_game(initial_state)