import math
import random

# Définition des valeurs possibles pour les cases du jeu
EMPTY = "-"
PLAYER_X = "X"
PLAYER_O = "O"

# Fonction pour afficher le plateau de jeu
def print_board(board):
    for row in board:
        print(" ".join(row))

# Fonction pour créer un nouveau plateau de jeu
def create_board(size):
    return [[EMPTY for _ in range(size)] for _ in range(size)]

# Fonction pour déterminer si un joueur a gagné
def check_winner(board, player):
    # Vérifier les lignes
    for row in board:
        if all(cell == player for cell in row):
            return True

    # Vérifier les colonnes
    for col in range(len(board)):
        if all(board[row][col] == player for row in range(len(board))):
            return True

    # Vérifier la diagonale principale
    if all(board[i][i] == player for i in range(len(board))):
        return True

    # Vérifier la diagonale opposée
    if all(board[i][len(board)-1-i] == player for i in range(len(board))):
        return True

    return False

def is_game_over(board):
    # Vérifier s'il y a des cases vides
    if any(EMPTY in row for row in board):
        return False

    # Vérifier si l'un des joueurs a gagné
    if check_winner(board, PLAYER_X) or check_winner(board, PLAYER_O):
        return True

    # Si le plateau est plein et aucun joueur n'a gagné, la partie est terminée
    return True


# Fonction pour évaluer le score de la configuration actuelle du jeu
def evaluate(board, depth):
    if check_winner(board, PLAYER_X):
        return 10 - depth
    elif check_winner(board, PLAYER_O):
        return depth - 10
    else:
        return 0

# Fonction pour obtenir le meilleur score que l'IA peut atteindre avec une certaine profondeur
def minimax(board, depth, max_player, target_depth):
    # Vérifier si le jeu est terminé ou si la profondeur maximale a été atteinte
    if is_game_over(board) or depth == target_depth:
        return evaluate(board, depth)

    # Choisir le joueur en fonction de la profondeur actuelle
    player = PLAYER_X if max_player else PLAYER_O

    # Initialiser le score en fonction du joueur
    best_score = -math.inf if max_player else math.inf

    # Parcourir toutes les cases vides du plateau
    for row in range(len(board)):
        for col in range(len(board)):
            if board[row][col] == EMPTY:
                # Jouer le coup pour ce joueur
                board[row][col] = player
                # Appeler la fonction minimax récursivement pour obtenir le score pour ce coup
                score = minimax(board, depth+1, not max_player, target_depth)
                # Mettre à jour le meilleur score en fonction du joueur
                if max_player:
                    best_score = max(best_score, score)
                else:
                    best_score = min(best_score, score)
                # Annuler le coup
                board[row][col] = EMPTY

    return best_score

# Fonction pour faire jouer l'IA
def get_computer_move(board, depth):
    best_score = -math.inf
    best_move = None

    # Parcourir toutes les cases vides du plateau
    for row in range(len(board)):
        for col in range(len(board)):
            if board[row][col] == EMPTY:
                # Jouer le coup pour l'IA
                board[row][col] = PLAYER_X
                # Appeler la fonction minimax pour obtenir le score de ce coup
                score = minimax(board, 0, False, depth)
                # Mettre à jour le meilleur score et le meilleur coup
                if score > best_score:
                    best_score = score
                    best_move = (row, col)
                # Annuler le coup
                board[row][col] = EMPTY

    return best_move

# Fonction pour faire jouer un humain
def get_human_move(board):
    while True:
        try:
            row = int(input("Entrez le numéro de ligne pour jouer (à partir de 1) : ")) - 1
            col = int(input("Entrez le numéro de colonne pour jouer (à partir de 1) : ")) - 1
            if board[row][col] == EMPTY:
                return (row, col)
            else:
                print("Cette case est déjà occupée.")
        except ValueError:
            print("Veuillez entrer un nombre valide.")

# Fonction pour faire jouer une partie complète
def play_game():
    # Demander la taille du plateau de jeu
    while True:
        try:
            size = int(input("Entrez la taille du plateau de jeu : "))
            if size >= 3:
                break
            else:
                print("La taille doit être supérieure ou égale à 3.")
        except ValueError:
            print("Veuillez entrer un nombre valide.")

    # Demander la profondeur maximale pour l'IA
    while True:
        try:
            depth = int(input("Entrez la profondeur maximale pour l'IA : "))
            if depth >= 1:
                break
            else:
                print("La profondeur doit être supérieure ou égale à 1.")
        except ValueError:
            print("Veuillez entrer un nombre valide.")

    # Créer le plateau de jeu
    board = create_board(size)
    print_board(board)

    # Choisir aléatoirement qui commence à jouer
    first_player = PLAYER_X
    print(f"{first_player} commence à jouer.")

    # Boucle principale du jeu
    while not is_game_over(board):
        if first_player == PLAYER_X:
            # Tour de l'IA
            print("Tour de l'IA...")
            row, col = get_computer_move(board, depth)
            board[row][col] = PLAYER_X
            print_board(board)
            if check_winner(board, PLAYER_X):
                print("L'IA a gagné.")
                return
        else:
            # Tour de l'humain
            print("Tour de l'humain.")
            row, col = get_human_move(board)
            board[row][col] = PLAYER_O
            print_board(board)
            if check_winner(board, PLAYER_O):
                print("L'humain a gagné.")
                return

        # Passer le tour au joueur suivant
        first_player = PLAYER_X if first_player == PLAYER_O else PLAYER_O

print("Match nul.")

# Fonction principale pour lancer le jeu
def main():
    play_game()

if __name__ == "__main__":
    main()