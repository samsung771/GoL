#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <stdlib.h>

#define DIMX 60
#define DIMY 30

void printBoard(bool (&board)[DIMY][DIMX]) {
	std::string print;

	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			char square = 219;
			std::string letter;
			if (board[y][x] == 0) letter = "  ";
			else letter.append(2, square);

			print.append(letter);
		}
		print.append("\n");
	}
	std::cout << print;
}

void popBoard(bool (&board)[DIMY][DIMX]) {
	srand((unsigned)time(NULL));
	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			board[y][x] = rand() % 2;
		}
	} 
}

void updateBoard(bool(&board)[DIMY][DIMX]) {
	bool newBoard[DIMX][DIMY] = {0};
	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			std::vector<bool*> neighbors;
			if (x > 0) {
				neighbors.push_back(&board[y][x - 1]);
				if (y > 0) neighbors.push_back(&board[y-1][x - 1]);
				if (y < DIMY - 1) neighbors.push_back(&board[y + 1][x - 1]);
			}
			if (x < DIMX - 1) {
				neighbors.push_back(&board[y][x + 1]);
				if (y > 0) neighbors.push_back(&board[y - 1][x + 1]);
				if (y < DIMY - 1) neighbors.push_back(&board[y + 1][x + 1]);
			}

			if (y > 0) neighbors.push_back(&board[y - 1][x]);
			if (y < DIMY - 1) neighbors.push_back(&board[y + 1][x]);

			int count = 0;
			for (bool* i : neighbors) if (*i == 1) count++;

			if (count == 2 && board[y][x]) newBoard[y][x] = 1;
			else if (count == 3) newBoard[y][x] = 1;
			else newBoard[y][x] = 0;
			int a = 0;
		}
	}

	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			board[y][x] = newBoard[y][x];
		}
	}
}

int main() {
	bool board[DIMY][DIMX] = {0};

	popBoard(board);

	while (true) {
		printBoard(board);
		updateBoard(board);
		//system("CLS");
	}
}

