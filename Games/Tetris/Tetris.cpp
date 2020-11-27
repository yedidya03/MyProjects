#include <Windows.h>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

wstring tetrisPiecces[7];

void createTetrisPieces() {

	tetrisPiecces[0].append(L"--X-");
	tetrisPiecces[0].append(L"--X-");
	tetrisPiecces[0].append(L"--X-");
	tetrisPiecces[0].append(L"--X-");

	tetrisPiecces[1].append(L"-X--");
	tetrisPiecces[1].append(L"-XX-");
	tetrisPiecces[1].append(L"--X-");
	tetrisPiecces[1].append(L"----");

	tetrisPiecces[2].append(L"-XXX");
	tetrisPiecces[2].append(L"--X-");
	tetrisPiecces[2].append(L"----");
	tetrisPiecces[2].append(L"----");

	tetrisPiecces[3].append(L"--X-");
	tetrisPiecces[3].append(L"--X-");
	tetrisPiecces[3].append(L"--X-");
	tetrisPiecces[3].append(L"--X-");

	tetrisPiecces[4].append(L"--X-");
	tetrisPiecces[4].append(L"--X-");
	tetrisPiecces[4].append(L"--X-");
	tetrisPiecces[4].append(L"--X-");

	tetrisPiecces[5].append(L"--X-");
	tetrisPiecces[5].append(L"--X-");
	tetrisPiecces[5].append(L"--X-");
	tetrisPiecces[5].append(L"--X-");

	tetrisPiecces[6].append(L"--X-");
	tetrisPiecces[6].append(L"--X-");
	tetrisPiecces[6].append(L"--X-");
	tetrisPiecces[6].append(L"--X-");
}

unsigned char* field;

int rotate(int px, int py, int rotate) {
	switch (rotate % 4) {
	case 0: return py * 4 + px;
	case 1: return 12 + py - (px * 4);
	case 2: return 15 - (py * 4) - px;
	case 3: return 3 - py + (px * 4);
	}
	return 0;
}

int fieldWidth = 12;

bool doesPieceFit(int piece, int posX, int posY, int rotation) {
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++) {
			if (field[(posY + py) * fieldWidth + (posX + px)] != 0 && tetrisPiecces[piece][rotate(px, py, rotation)] != L'-') {
				return false;
			}
		}

	return true;
}

int main() {

	int fieldHeight = 15;

	int screenWidth = 340;
	int screenHeight = 350;

	int screenRows;
	int screenColumns;

	bool gameover = false;

	createTetrisPieces();

	
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, screenWidth, screenHeight, TRUE);
	
	
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);

	screenColumns = info.dwSize.X / 2;
	screenRows = info.dwSize.Y;

	fieldHeight = screenRows / 2 - 2;
	fieldWidth = screenColumns - 4;

	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;


	field = new unsigned char[fieldHeight * fieldWidth];
	for (int x = 0; x < fieldWidth; x++) {
		for (int y = 0; y < fieldHeight; y++) {
			field[y * fieldWidth + x] = (x == 0 || x == fieldWidth - 1 || y == (fieldHeight - 1)) ? 9 : 0;
		}
	}
	unsigned char* f = field;
	wchar_t *screen = new wchar_t[screenColumns * screenRows];
	for (int i = 0; i < screenColumns * screenRows; i++) screen[i] = L'_';

	int currPiece = 0;
	int currX = fieldWidth / 2;
	int currY = 0;
	int rotation = 0;

	int ticss2down = 20;
	int tics = 0;

	bool lockPiece = false;
	bool spacePress = false;

	// Game Loop
	while (!gameover) {

		// Time and tics -------------------------------------
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
		tics++;

		// User input ----------------------------------------
		if (GetAsyncKeyState(VK_ESCAPE)) 
			gameover = true;


		if (GetAsyncKeyState(VK_SPACE)) {
			if (!spacePress && doesPieceFit(currPiece, currX, currY, rotation + 1)) rotation++;
			spacePress = true;
		}
		else {
			spacePress = false;
		}

		if (GetAsyncKeyState(VK_RIGHT))
			if (doesPieceFit(currPiece, currX + 1, currY, rotation))
				currX++;

		if (GetAsyncKeyState(VK_LEFT))
			if (doesPieceFit(currPiece, currX - 1, currY, rotation))
				currX--;

		if (GetAsyncKeyState(VK_DOWN))
			if (doesPieceFit(currPiece, currX, currY + 1, rotation))
				currY++;
			else
				lockPiece = true;

		// Game logic --------------------------------

		// moving down the piece by time
		if (tics >= ticss2down) {
			if (doesPieceFit(currPiece, currX, currY + 1, rotation))
				currY++;
			else
				lockPiece = true;
			tics = 0;
		}

		// locking the piece to the field
		if (lockPiece) {
			for (int px = 0; px < 4; px++) {
				for (int py = 0; py < 4; py++) {
					if (tetrisPiecces[currPiece][rotate(px, py, rotation)] != L'-')
						field[(py + currY) * fieldWidth + (px + currX)] = currPiece + 1;
				}
			}

			currPiece = (currPiece + 1) % 7;
			currX = fieldWidth / 2;
			currY = 0;

			if (!doesPieceFit(currPiece, currX, currY, rotation))
				gameover = true;

			lockPiece = false;
		}

		// Erasing a full row - it will erase rows that got full at the previus tic
		for (int y = 0; y < fieldHeight - 1; y++) {
			if (field[y * fieldWidth + 1] == 8) {
				for (int ny = y; ny > 0; ny--) {
					for (int x = 1; x < fieldWidth - 1; x++) {
						field[ny * fieldWidth + x] = field[(ny - 1) * fieldWidth + x];
					}
				}

				for (int x = 1; x < fieldWidth - 1; x++) {
					field[x] = 0;
				}

				ticss2down -= ticss2down / 5;
			}
		}

		// Checking for full rows
		for (int y = 0; y < fieldHeight - 1; y++) {
			bool all = true;

			for (int x = 1; x < fieldWidth - 1; x++) {
				if (field[y * fieldWidth + x] == 0){
					all = false;
					break;
				}
			}

			if (all) {
				for (int x = 1; x < fieldWidth - 1; x++) {
					field[y * fieldWidth + x] = 8;
				}
			}
		}


		// Drew Field ----------------------------------
		for (int x = 0; x < fieldWidth; x++) {
			for (int y = 0; y < fieldHeight; y++) {
				screen[(y + 2) * screenColumns + (x + 2)] = L" 1234567=#"[field[y * fieldWidth + x]];
			}
		}

		// Drew Current Piece
		for (int px = 0; px < 4; px++) {
			for (int py = 0; py < 4; py++) {
				if (tetrisPiecces[currPiece][rotate(px, py, rotation)] != L'-')
					screen[(py + currY + 2) * screenColumns + (px + currX + 2)] = L'X';
			}
		}

		WriteConsoleOutputCharacter(hConsole, (LPCSTR)screen, screenColumns * screenRows, { 0, 0 }, &dwBytesWritten);
 	}

	wstring gameOverMes = L"GAME OVER!";
	for (int i = 0; i < screenColumns * screenRows; i++) screen[i] = L' ';
	for (int i = 0; i < gameOverMes.length(); i++) {
		screen[(fieldHeight / 2)* screenColumns + ((fieldWidth - gameOverMes.length()) / 2) + i] = gameOverMes[i];
	}

	WriteConsoleOutputCharacter(hConsole, (LPCSTR)screen, screenColumns * screenRows, { 0, 0 }, &dwBytesWritten);

	std::this_thread::sleep_for(std::chrono::milliseconds(1500));
}

/**

	Problems :
		- displays only 15 rows.
		- need to figure out how to set windows size

*/