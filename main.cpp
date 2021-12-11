#include <iostream>
#include <vector>
using namespace std;

const int SIZE = 4;

// |--- x --->
// |
// y
// |
// v

struct Pos {
  int x;
  int y;
};

// Draw the board onto the terminal
void draw(const int board[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      cout << board[i][j] << "\t";
    }
    cout << endl;
  }
}

// Return a list of free spots on the board
vector<Pos> get_free(const int board[SIZE][SIZE]) {
  vector<Pos> free;
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      if (board[i][j] == 0) {
        free.push_back({.x = j, .y = i});
      }
    }
  }
  return free;
}

// Helper function to print a list of free spots for debugging
void print_free(const vector<Pos>& free) {
  for (const Pos& p : free) {
    cout << "(" << p.x << ", " << p.y << ")"
         << " ";
  }
  cout << endl;
}

// Spawn a 2 (90% chance) or a 4 (10% chance) onto a free spot
// If successful, return true
// If there are no free spots, return false
bool spawn(int board[SIZE][SIZE]) {
  vector<Pos> free = get_free(board);
  if (free.empty()) {
    return false;
  }
  int idx = random() % free.size();
  int number = random() % 10 == 0 ? 4 : 2;
  Pos p = free[idx];
  board[p.y][p.x] = number;
  return true;
}

// Perform the up move
void move_up(int board[SIZE][SIZE]) {
  // Look at each column (board[...][j]) independently
  for (int j = 0; j < SIZE; j++) {
    // Keep track of the furthest row we might need to
    // swap values or merge with, starting from row 0
    int prev_i = 0;
    // Look at rows from top to bottom, from row 1
    for (int i = 1; i < SIZE; i++) {
      if (board[i][j] == 0) {
        // If current row is 0, look at the next row
        continue;
      } else if (board[prev_i][j] == 0) {
        // If current row is not 0 but furthest row is,
        // swap the values (push the non-0 value up)
        board[prev_i][j] = board[i][j];
        board[i][j] = 0;
      } else if (board[prev_i][j] == board[i][j]) {
        // If current row and furthest row are the same and non-0,
        // we should do a merge:
        // -> set furthest row to the sum of the two (= doubling it)
        // -> advance furthest row by 1 (a slot can only merge 1x/turn)
        // -> set current row to 0 (it was pushed up)
        board[prev_i][j] *= 2;
        prev_i++;
        board[i][j] = 0;
      } else {
        // If current row and furthest row are non-0 and different:
        // -> advance furthest row by 1
        // -> now the new furthest row should be empty
        // -> swap values with current row (push the non-0 value up)
        prev_i++;
        int val = board[i][j];
        board[i][j] = 0;
        board[prev_i][j] = val;
      }
    }
  }
}

// Perform the down move
void move_down(int board[SIZE][SIZE]) {
  // Look at each column (board[...][j]) independently
  for (int j = 0; j < SIZE; j++) {
    // Similar to the up move, but looking at rows from bottom to up
    int prev_i = SIZE - 1;
    for (int i = SIZE - 2; i >= 0; i--) {
      if (board[i][j] == 0) {
        continue;
      } else if (board[prev_i][j] == 0) {
        board[prev_i][j] = board[i][j];
        board[i][j] = 0;
      } else if (board[prev_i][j] == board[i][j]) {
        board[prev_i][j] *= 2;
        board[i][j] = 0;
        prev_i--;
      } else {
        prev_i--;
        int val = board[i][j];
        board[i][j] = 0;
        board[prev_i][j] = val;
      }
    }
  }
}

// Perform the left move
void move_left(int board[SIZE][SIZE]) {
  // Look at each row (board[i][...]) independently
  for (int i = 0; i < SIZE; i++) {
    // Similar to the up move, but swapping i and js
    // Looking at columns from left to right
    int prev_j = 0;
    for (int j = 1; j < SIZE; j++) {
      if (board[i][j] == 0) {
        continue;
      } else if (board[i][prev_j] == 0) {
        board[i][prev_j] = board[i][j];
        board[i][j] = 0;
      } else if (board[i][prev_j] == board[i][j]) {
        board[i][prev_j] *= 2;
        board[i][j] = 0;
        prev_j++;
      } else {
        prev_j++;
        int val = board[i][j];
        board[i][j] = 0;
        board[i][prev_j] = val;
      }
    }
  }
}

// Perform the right move
void move_right(int board[SIZE][SIZE]) {
  // Look at each row (board[i][...]) independently
  for (int i = 0; i < SIZE; i++) {
    // Similar to the left move, but looking at columns from right to left
    int prev_j = SIZE - 1;
    for (int j = SIZE - 2; j >= 0; j--) {
      if (board[i][j] == 0) {
        continue;
      } else if (board[i][prev_j] == 0) {
        board[i][prev_j] = board[i][j];
        board[i][j] = 0;
      } else if (board[i][prev_j] == board[i][j]) {
        board[i][prev_j] *= 2;
        board[i][j] = 0;
        prev_j--;
      } else {
        prev_j--;
        int val = board[i][j];
        board[i][j] = 0;
        board[i][prev_j] = val;
      }
    }
  }
}

// Get the user input to perform a move
void make_move(int board[SIZE][SIZE]) {
  char input;
  cin >> input;

  switch (input) {
    case 'w':
      move_up(board);
      break;
    case 's':
      move_down(board);
      break;
    case 'a':
      move_left(board);
      break;
    case 'd':
      move_right(board);
      break;
  }
}

// Initialize the board to 0s and spawn 2
void init_board(int board[SIZE][SIZE]) {
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      board[i][j] = 0;
    }
  }
  spawn(board);
  spawn(board);
}

// Run a terminal version of https://play2048.co/
int main() {
  int board[SIZE][SIZE];
  init_board(board);

  do {
    draw(board);
    make_move(board);
  } while (spawn(board));
}