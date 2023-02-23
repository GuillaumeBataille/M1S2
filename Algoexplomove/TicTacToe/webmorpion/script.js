const gameContainer = document.getElementById("game");
const sizeSelector = document.getElementById("size");
const startButton = document.getElementById("start");

let board = [];
let currentPlayer = "X";
let gameInProgress = false;
let gameSize = 3;

// Créer la grille de jeu
function createBoard() {
  for (let i = 0; i < gameSize; i++) {
    board[i] = [];
    for (let j = 0; j < gameSize; j++) {
      board[i][j] = null;
      const cell = document.createElement("div");
      cell.classList.add("cell");
      cell.addEventListener("click", () => {
        if (gameInProgress) {
          makeMove(i, j);
        }
      });
      gameContainer.appendChild(cell);
    }
  }
}

// Commencer la partie
function startGame() {
  gameSize = parseInt(sizeSelector.value);
  gameContainer.innerHTML = "";
  createBoard();
  currentPlayer = "X";
  gameInProgress = true;
}

// Faire un mouvement
function makeMove(row, col) {
  if (board[row][col] === null) {
    board[row][col] = currentPlayer;
    const cell = gameContainer.children[row * gameSize + col];
    cell.textContent = currentPlayer;
    cell.classList.add(currentPlayer.toLowerCase());
    checkForWin();
    switchPlayer();
  }
}

// Vérifier s'il y a un vainqueur
function checkForWin() {
  // Vérifier les lignes
  for (let i = 0; i < gameSize; i++) {
    if (board[i].every(val => val === currentPlayer)) {
      endGame();
    }
  }
  // Vérifier les colonnes
  for (let j = 0; j < gameSize; j++) {
    if (board.every(row => row[j] === currentPlayer)) {
      endGame();
    }
  }
  // Vérifier la diagonale principale
  if (board.every((row, i) => row[i] === currentPlayer)) {
    endGame();
  }
  // Vérifier la diagonale secondaire
  if (board.every((row, i) => row[gameSize - 1 - i] === currentPlayer)) {
    endGame();
  }
}

// Changer le joueur courant
function switchPlayer() {
  currentPlayer = currentPlayer === "X" ? "O" : "X";
}

// Fin de la partie
function endGame() {
  gameInProgress = false;
  alert(`${currentPlayer} a gagné!`);
}

startButton.addEventListener("click", startGame);
