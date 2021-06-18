#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <Windows.h>
#include <stdlib.h>
#include <chrono>
#include <thread>

#define DIMX 60
#define DIMY 29
#define frameTime 10

void printBoard(bool (&board)[DIMY][DIMX]) {
	std::string print;

	//iterates through each square in the grid
	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			//makes it a pixel either 2 spaces or 2 squares if it is alive or dead
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
	//fills the board with a 50% chance of each one being alive
	srand((unsigned)time(NULL));
	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			board[y][x] = rand() % 2;
		}
	}
}

void updateBoard(bool(&board)[DIMY][DIMX]) {
	bool newBoard[DIMY][DIMX] = {0};
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
	static HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	bool board[DIMY][DIMX] = {0};

	popBoard(board);
	while (true) {
		printBoard(board);
		updateBoard(board);

		//sleep till next frame
		std::this_thread::sleep_for(std::chrono::milliseconds(frameTime));		
		SetConsoleCursorPosition(hConsole, COORD{0,0});
	}
}