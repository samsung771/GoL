#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <Windows.h>
#include <stdlib.h>
#include <chrono>


#define DIMX 60
#define DIMY 29
#define SPEEDMAX 5


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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
	//outputs as one chunk as it is more efficient
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
			//finds all the cells neighbours and adds them as pointers
			std::vector<bool*> neighbours;
			if (x > 0) {
				neighbours.push_back(&board[y][x - 1]);
				if (y > 0) neighbours.push_back(&board[y-1][x - 1]);
				if (y < DIMY - 1) neighbours.push_back(&board[y + 1][x - 1]);
			}
			if (x < DIMX - 1) {
				neighbours.push_back(&board[y][x + 1]);
				if (y > 0) neighbours.push_back(&board[y - 1][x + 1]);
				if (y < DIMY - 1) neighbours.push_back(&board[y + 1][x + 1]);
			}

			if (y > 0) neighbours.push_back(&board[y - 1][x]);
			if (y < DIMY - 1) neighbours.push_back(&board[y + 1][x]);

			//counts neighbours
			int count = 0;
			for (bool* i : neighbours) if (*i == 1) count++;

			//sets cells life based on the GoL rules :
			//--------------------------------------------------------------------------
			//1. if there are 2 or 3 neighbors and the cell is alive it stays alive
			//2. if there are less than 2 neighbors the cell dies due to underpopulation
			//3. if there are more than 3 neighbors the cell dies due to overpopulation
			//4. if there are 3 neighbors and the cell is dead it becomes alive
			//--------------------------------------------------------------------------
			if (count == 2 && board[y][x]) newBoard[y][x] = 1;
			else if (count == 3) newBoard[y][x] = 1;
			else newBoard[y][x] = 0;
		}
	}

	//updates the board
	for (int y = 0; y < DIMY; y++) {
		for (int x = 0; x < DIMX; x++) {
			board[y][x] = newBoard[y][x];
		}
	}
}

int main() {
	bool board[DIMY][DIMX] = {0};

	int speeds[5] = {
		200,
		100,
		70,
		35,
		10
	};

	int curSpeed = 2;

	RECT rectClient, rectWindow;

	POINT p;
	int pos[2];

	bool running = true;
	bool pause = false;
	bool newFrame = true;

	auto start = std::chrono::steady_clock::now();
	int frameTime = speeds[curSpeed];

	bool spacekey = false;
	bool lmb = false;
	bool arUp = false;
	bool arDown = false;

	while (running) {
		//sets pause if you press space
		if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
			if (spacekey == false && !pause) pause = true;
			else if (spacekey == false && pause) pause = false;
			//sets spacekey true to stop looping
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

		//raises speed if you press right arrow
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && curSpeed < SPEEDMAX - 1) {
			if (!arUp) curSpeed++;
			arUp = true;
		}
		else arUp = false;

		//lowers speed if you press left arrow
		if (GetAsyncKeyState(VK_LEFT) & 0x8000 && curSpeed > 0) {
			if (!arDown) curSpeed--;
			arDown = true;
		}
		else arDown = false;

		//update frame speed
		frameTime = speeds[curSpeed];

		//create random board if you press x
		if (GetAsyncKeyState('X') & 0x8000) popBoard(board);

		//creates or erases square when you click
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
			//get mouse position
			GetClientRect(hWnd, &rectClient);
			GetWindowRect(hWnd, &rectWindow);
			GetCursorPos(&p);

			//find the square it you are hovering over
			pos[0] = (p.x - rectWindow.left) / 15;
			pos[1] = (p.y - rectWindow.top - 30) / 16;

			//clamp mouse to board
			if (pos[0] <= DIMX && pos[0] >= 0 && pos[1] <= DIMY && pos[1] >= 0) {
				if (lmb == false && !board[pos[1]][pos[0]]) board[pos[1]][pos[0]] = 1;
				else if (lmb == false && board[pos[1]][pos[0]]) board[pos[1]][pos[0]] = 0;
			}
			//sets lmb to stop looping while held
			lmb = true;
		}
		else lmb = false;

		if (newFrame) {
			printBoard(board);
			if (!pause) updateBoard(board);
			std::cout << "                                                                                                                      ";
			SetConsoleCursorPosition(hConsole, COORD{ 0,29 });
			std::cout << "[esc] Quit	  [space] Pause	     [c] Clear Board	   [x] Create New Random Board	    [arrows] Speed";
			for (int i = 0; i < curSpeed + 1; i++) std::cout << " -";

			SetConsoleCursorPosition(hConsole, COORD{ 0,0 });
			newFrame = false;
		}

		//only print a new frame every 70 ms
		auto end = std::chrono::steady_clock::now();

		if (std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() > frameTime) {
			start = std::chrono::steady_clock::now();
			newFrame = true;
		}
	}
}