const HUMAN_PLAYER = 'X';
const AI_PLAYER = 'O';

function minimax(board, depth, maximizingPlayer) {
  if (checkWinner(board, HUMAN_PLAYER)) {
    return { score: -10 };
  } else if (checkWinner(board, AI_PLAYER)) {
    return { score: 10 };
  } else if (getEmptyCells(board).length === 0) {
    return { score: 0 };
  }

  if (maximizingPlayer) {
    let bestScore = { score: -Infinity };
    for (let cell of getEmptyCells(board)) {
      let newBoard = makeMove(board, cell, AI_PLAYER);
      let score = minimax(newBoard, depth + 1, false).score;
      bestScore = score > bestScore.score ? { score, cell } : bestScore;
    }
    return depth === 0 ? bestScore.cell : bestScore;
  } else {
    let bestScore = { score: Infinity };
    for (let cell of getEmptyCells(board)) {
      let newBoard = makeMove(board, cell, HUMAN_PLAYER);
      let score = minimax(newBoard, depth + 1, true).score;
      bestScore = score < bestScore.score ? { score, cell } : bestScore;
    }
    return bestScore;
  }
}

function makeMove(board, cell, player) {
  let newBoard = [...board];
  newBoard[cell] = player;
  return newBoard;
}

function checkWinner(board, player) {
  let winningCombinations = [
    [0, 1, 2], [3, 4, 5], [6, 7, 8], [0, 3, 6], [1, 4, 7], [2, 5, 8], [0, 4, 8], [2, 4, 6]
  ];
  for (let combination of winningCombinations) {
    if (combination.every(cell => board[cell] === player)) {
      return true;
    }
  }
  return false;
}

function getEmptyCells(board) {
  return board.reduce((cells, cell, index) => {
    if (!cell) {
      cells.push(index);
    }
    return cells;
  }, []);
}

class TicTacToe {
  constructor(size, aiDepth) {
    this.size = size;
    this.aiDepth = aiDepth;
    this.board = Array(size * size).fill(null);
    this.currentPlayer = HUMAN_PLAYER;
    this.winner = null;
    this.gameOver = false;
  }

  makeHumanMove(cell) {
    if (this.gameOver || this.board[cell]) {
      return;
    }
    this.board = makeMove(this.board, cell, HUMAN_PLAYER);
    if (checkWinner(this.board, HUMAN_PLAYER)) {
      this.winner = HUMAN_PLAYER;
      this.gameOver = true;
    } else if (getEmptyCells(this.board).length === 0) {
      this.gameOver = true;
    } else {
      this.currentPlayer = AI_PLAYER;
      this.makeAIMove();
    }
  }

  makeAIMove() {
    if (this.gameOver) {
      return;
    }
    let { score, cell } = minimax(this.board, 0, true, this.aiDepth);
    this.board = makeMove(this.board, cell, AI_PLAYER);
    if (checkWinner(this.board, AI_PLAYER)) {
      this.winner = AI_PLAYER;
      this.gameOver = true;
    }
}

reset() {
  this.board = Array(this.size * this.size).fill(null);
  this.currentPlayer = HUMAN_PLAYER;
  this.winner = null;
  this.gameOver = false;
}
}

// Exemple d'utilisation
let game = new TicTacToe(3, 5); // Grille de taille 3x3, profondeur de parcours de 5
game.makeHumanMove(4); // Le joueur humain joue en position 4
game.makeHumanMove(0); // Le joueur humain joue en position 0
game.makeHumanMove(8); // Le joueur humain joue en position 8
console.log(game.board); // Affiche la grille actuelle
console.log(game.winner); // Affiche le gagnant, ou null si la partie n'est pas terminée
