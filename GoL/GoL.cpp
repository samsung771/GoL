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
#define frameTime 70
#define pauseTime 100


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
RECT rectClient, rectWindow;
HWND hWnd = GetConsoleWindow();

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
	bool board[DIMY][DIMX] = {0};

	POINT p;
	int pos[2];

	bool running = true;
	bool pause = false;
	bool newFrame = true;

	auto start = std::chrono::steady_clock::now();

	bool spacekey = false;
	bool lmb = false;

	while (running) {
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			if (spacekey == false && !pause) pause = true;
			else if (spacekey == false && pause) pause = false;
			spacekey = true;
		}
		else spacekey = false;

		//stops running loop if you press escape
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;

		//clears board if you press c
		if (GetAsyncKeyState('C') & 0x8000) {
			for (int y = 0; y < DIMY; y++) {
				for (int x = 0; x < DIMX; x++) {
					board[y][x] = 0;
				}
			}
		}

		//create random board if you press x
		if (GetAsyncKeyState('X') & 0x8000) {
			popBoard(board);
		}

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			GetClientRect(hWnd, &rectClient);
			GetWindowRect(hWnd, &rectWindow);
			GetCursorPos(&p);
			pos[0] = (p.x - rectWindow.left) / 15;
			pos[1] = (p.y - rectWindow.top - 30) / 16;
			if (pos[0] <= DIMX && pos[0] >= 0 && pos[1] <= DIMY && pos[1] >= 0) {
				if (lmb == false && !board[pos[1]][pos[0]]) board[pos[1]][pos[0]] = 1;
				else if (lmb == false && board[pos[1]][pos[0]]) board[pos[1]][pos[0]] = 0;
			}
			lmb = true;
		}
		else lmb = false;

		if (newFrame) {
			printBoard(board);
			if (!pause) updateBoard(board);
			std::cout << "[esc] Quit		[space] Pause		  [c] Clear Board  		[x] Create New Random Board";

			SetConsoleCursorPosition(hConsole, COORD{ 0,0 });
			newFrame = false;
		}

		//create new frame every 70 ms
		auto end = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > frameTime) {
			start = std::chrono::steady_clock::now();
			newFrame = true;
		}
	}
}