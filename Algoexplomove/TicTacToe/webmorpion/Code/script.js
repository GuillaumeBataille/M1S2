const board = document.querySelector("#board");
const size = parseInt(prompt("Enter board size (min 3, max 10)"));
const aiDepth = parseInt(prompt("Enter AI depth (min 1, max 10)"));

// Initialize board
board.style.setProperty("--size", size);
for (let i = 0; i < size * size; i++) {
  const cell = document.createElement("div");
  cell.textContent = "";
  cell.addEventListener("click", handleClick);
  board.appendChild(cell);
}

// Initialize game
let player = "X";
let gameOver = false;
let cells = [...document.querySelectorAll("#board div")];
let emptyCells = cells.slice();

function handleClick() {
  if (gameOver) return;
  if (!emptyCells.includes(this)) return;
  this.textContent = player;
  emptyCells.splice(emptyCells.indexOf(this), 1);
  if (checkWin()) {
    gameOver = true;
    alert(`${player} wins!`);
    return;
  } else if (emptyCells.length === 0) {
    gameOver = true;
    alert("Tie game!");
    return;
  }
  switchPlayer();
  if (player === "O") {
    const move = minimax(0, "O").index;
    cells[move].click();
  }
}

function checkWin() {
  const lines = [];
  for (let i = 0; i < size; i++) {
    lines.push(cells.slice(i * size, (i + 1) * size)); // rows
    lines.push(cells.filter((_, j) => j % size === i)); // columns
  }
  lines.push(cells.filter((_, i) => i % (size + 1) === 0)); // diagonal \
  lines.push(cells.filter((_, i) => i % (size - 1) === 0).slice(1, -1)); // diagonal /
  return lines.some(line => line.every(cell => cell.textContent === player));
}

function switchPlayer() {
  player = player === "X" ? "O" : "X";
}

function minimax(depth, player) {
    if (player === "X") {
      var best = {score: -Infinity};
    } else {
      var best = {score: Infinity};
    }
    if (depth === aiDepth || checkWin()) {
      const score = evaluate();
      return {score};
    }
    for (let i = 0; i < emptyCells.length; i++) {
      const move = emptyCells[i];
      move.textContent = player;
      emptyCells.splice(i, 1);
      const score = minimax(depth + 1, player === "X" ? "O" : "X").score;
      move.textContent = "";
      emptyCells.splice(i, 0, move);
      if (player === "X") {
        if (score > best.score) {
          best.score = score;
          best.index = cells.indexOf(move);
        }
      } else {
        if (score < best.score) {
          best.score = score;
          best.index = cells.indexOf(move);
        }
      }
    }
    return best;
  }
  
  function evaluate() {
    let score = 0;
    const lines = [];
    for (let i = 0; i < size; i++) {
      lines.push(cells.slice(i * size, (i + 1) * size)); // rows
      lines.push(cells.filter((_, j) => j % size === i)); // columns
    }
    lines.push(cells.filter((_, i) => i % (size + 1) === 0)); // diagonal \
    lines.push(cells.filter((_, i) => i % (size - 1) === 0).slice(1, -1)); // diagonal /
    for (const line of lines) {
      const counts = line.reduce((acc, cell) => {
        acc[cell.textContent] = (acc[cell.textContent] || 0) + 1;
        return acc;
      }, {});
      if (counts.X === size) {
        score += 10;
      } else if (counts.O === size) {
        score -= 10;
      } else if (counts.X && !counts.O) {
        score += counts.X;
      } else if (counts.O && !counts.X) {
        score -= counts.O;
      }
    }
    return score;
  }


  /////
  function play(cell) {
    if (!cell.textContent && !winner) {
      cell.textContent = humanPlayer;
      const index = cells.indexOf(cell);
      emptyCells.splice(emptyCells.indexOf(cell), 1);
      if (checkWin()) {
        declareWinner(humanPlayer);
      } else if (!emptyCells.length) {
        declareDraw();
      } else {
        const index = minimax(0, aiPlayer).index;
        cells[index].textContent = aiPlayer;
        emptyCells.splice(emptyCells.indexOf(cells[index]), 1);
        if (checkWin()) {
          declareWinner(aiPlayer);
        } else if (!emptyCells.length) {
          declareDraw();
        }
      }
    }
  }
  
  function checkWin() {
    const lines = [];
    for (let i = 0; i < size; i++) {
      lines.push(cells.slice(i * size, (i + 1) * size)); // rows
      lines.push(cells.filter((_, j) => j % size === i)); // columns
    }
    lines.push(cells.filter((_, i) => i % (size + 1) === 0)); // diagonal \
    lines.push(cells.filter((_, i) => i % (size - 1) === 0).slice(1, -1)); // diagonal /
    for (const line of lines) {
      const counts = line.reduce((acc, cell) => {
        acc[cell.textContent] = (acc[cell.textContent] || 0) + 1;
        return acc;
      }, {});
      if (counts.X === size) {
        winner = "X";
        return true;
      } else if (counts.O === size) {
        winner = "O";
        return true;
      }
    }
    return false;
  }
  
  function declareWinner(player) {
    winner = player;
    result.textContent = `${winner} wins!`;
    restartButton.style.display = "inline-block";
  }
  
  function declareDraw() {
    result.textContent = "Draw!";
    restartButton.style.display = "inline-block";
  }
  
  function restart() {
    cells.forEach(cell => cell.textContent = "");
    emptyCells = [...cells];
    winner = null;
    result.textContent = "";
    restartButton.style.display = "none";
    if (aiPlayer === "X") {
      const index = Math.floor(Math.random() * emptyCells.length);
      emptyCells[index].textContent = aiPlayer;
      emptyCells.splice(index, 1);
    }
  }
  
  play();
