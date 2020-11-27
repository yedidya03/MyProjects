#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <time.h> 

using namespace std;

struct squad {
	pair<int, int> pos;
	char body;
};

int fieldWidth = 12;
int fieldHeight = 12;

int screenWidth = 60;
int screenHeight = 30;

vector<squad> snake;

enum direction {
	UP, DOWN, LEFT, RIGHT
};

pair<int, int> newPos(pair<int, int> old, direction dir){
	switch (dir) {
	case UP:
		return make_pair(old.first, old.second - 1);
	case DOWN:
		return make_pair(old.first, old.second + 1);
	case LEFT:
		return make_pair(old.first - 1, old.second);
	case RIGHT:
		return make_pair(old.first + 1, old.second);
	}
}

pair<int, int> newPiece() {
	pair<int, int> temp = make_pair(rand() % (fieldWidth - 2) + 1, rand() % (fieldHeight - 2) + 1);
	for (auto& it : snake) {
		if (it.pos.first == temp.first && it.pos.second == temp.second) {
			return newPiece();
		}
	}
	return temp;
}

bool collition(squad currSquad) {
	if (currSquad.pos.first == 0 || currSquad.pos.second == 0 ||
		currSquad.pos.first == fieldWidth - 1 || currSquad.pos.second == fieldHeight - 1) {
		return true;
	}

	for (auto& it : snake){
		if (currSquad.pos.first == it.pos.first && currSquad.pos.second == it.pos.second) {
			return true;
		}
	}

	return false;
}

int main() {
	srand(time(0));

	bool gameover = false;

	squad currSquad;
	currSquad.body = '*';
	currSquad.pos = make_pair(fieldWidth / 2, fieldHeight / 2);
	snake.push_back(currSquad);
	currSquad.pos = make_pair(fieldWidth / 2, fieldHeight / 2 + 1);
	snake.push_back(currSquad);

	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	unsigned char* field;
	field = new unsigned char[fieldHeight * fieldWidth];
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			field[y * fieldWidth + x] = (x == 0 || x == fieldWidth - 1 || y == (fieldHeight - 1)) || y == 0 ? 9 : 0;
		}
	}
	unsigned char* f = field;
	wchar_t *screen = new wchar_t[screenWidth * screenHeight];
	for (int i = 0; i < screenHeight * screenWidth; i++) screen[i] = L'_';

	direction dir = UP;
	pair<int, int> piecePos = newPiece();

	// Game Loop
	while (!gameover) {

		// Time and tics -------------------------------------
		std::this_thread::sleep_for(std::chrono::milliseconds(150));

		// User input ----------------------------------------
		if (GetAsyncKeyState(VK_ESCAPE))
			gameover = true;

		if (GetAsyncKeyState(VK_UP)){
			if (dir != DOWN) dir = UP;
		}
		else if (GetAsyncKeyState(VK_DOWN)) {
			if (dir != UP) dir = DOWN;
		}
		else if (GetAsyncKeyState(VK_LEFT)){
			if (dir != RIGHT) dir = LEFT;
		}
		else if (GetAsyncKeyState(VK_RIGHT)){
			if (dir != LEFT) dir = RIGHT;
		}

		// Game logic --------------------------------
		currSquad.pos = newPos(snake.begin()->pos, dir);

		if (collition(currSquad)) gameover = true;

		snake.insert(snake.begin(), currSquad);
		if (currSquad.pos.first == piecePos.first && currSquad.pos.second == piecePos.second) {
			snake.begin()->body = '#';

			piecePos = newPiece();
		}

		if (snake.back().body != '#')
			snake.pop_back();
		else snake.back().body = '*';

		// Drew Field ----------------------------------
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				screen[(y + 1) * screenWidth + (x + 1)] = L" 1234567=#"[field[y * fieldWidth + x]];
			}
		}

		// Drew Piece
		screen[(piecePos.second + 1) * screenWidth + (piecePos.first + 1)] = '@';

		// Drew Snake
		for (auto& it : snake) {
			screen[(it.pos.second + 1) * screenWidth + (it.pos.first + 1)] = it.body;
		}

		WriteConsoleOutputCharacter(hConsole, (LPCSTR)screen, screenHeight * screenWidth, { 0, 0 }, &dwBytesWritten);
	}

	wstring gameOverMes = L"GAME OVER!";
	for (int i = 0; i < screenWidth * screenHeight; i++) screen[i] = L' ';
	for (int i = 0; i < gameOverMes.length(); i++) {
		screen[(fieldHeight / 2)* screenWidth + ((fieldWidth - gameOverMes.length()) / 2) + i] = gameOverMes[i];
	}

	WriteConsoleOutputCharacter(hConsole, (LPCSTR)screen, screenWidth * screenHeight, { 0, 0 }, &dwBytesWritten);

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

/**

Problems :
- displays only 15 rows.
- need to figure out how to set windows size

*/