
// -----------------------------------------------------------------------------
// https://lptcp.blogspot.com/
// 
/*
	Code-It-Yourself! Tetris
	
	Following along with the video by javidx9 here:
	https://www.youtube.com/watch?v=8OK8_tHeCIA&t=4s&ab_channel=javidx9

	His original code is here:
	https://github.com/OneLoneCoder/Javidx9/blob/master/SimplyCode/OneLoneCoder_Tetris.cpp

	I've modified it quite a bit

*/
// https://github.com/l-paz91/
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <conio.h>
#include <iostream>
#include <thread>
#include <windows.h>
#include <vector>
#include <string>

using namespace std;
//#include "std_lib_facilities.h"

// -----------------------------------------------------------------------------

//--GLOBALS--//
int fieldWidth = 12;
int fieldHeight = 18;
int screenWidth = 120;
int screenHeight = 30;

vector<wstring> gTetromino(7);
vector<char> mainGrid;
vector<wchar_t> screen;
unsigned char* pField = nullptr;

// -----------------------------------------------------------------------------

// pX, pY - original index values for tetromino pieces
// r	  - rotation of shape
int getIndexFromRotation(int pX, int pY, int pR)
{
	switch (pR % 4)
	{
	case 0:
		return (pY * 4) + pX;		// 0 degrees
	case 1:
		return 12 + pY - (pX * 4);	// 90 degrees
	case 2:
		return 15 - (pY * 4) - pX;	// 180 degrees
	case 3:
		return 3 - pY + (pX * 4);	// 270 degrees
	default:
		cout << "::rotate() How did you end up here?\n";
		break;
	}

	return 0;
}

// -----------------------------------------------------------------------------

// create tetris board
void f()
{
	// create assets
	gTetromino[0].append(L"..X.");
	gTetromino[0].append(L"..X.");
	gTetromino[0].append(L"..X.");
	gTetromino[0].append(L"..X.");

	gTetromino[1].append(L"..X.");
	gTetromino[1].append(L".XX.");
	gTetromino[1].append(L"..X.");
	gTetromino[1].append(L"....");

	gTetromino[2].append(L"....");
	gTetromino[2].append(L".XX.");
	gTetromino[2].append(L".XX.");
	gTetromino[2].append(L"....");

	gTetromino[3].append(L"..X.");
	gTetromino[3].append(L".XX.");
	gTetromino[3].append(L".X..");
	gTetromino[3].append(L"....");

	gTetromino[4].append(L".X..");
	gTetromino[4].append(L".XX.");
	gTetromino[4].append(L"..X.");
	gTetromino[4].append(L"....");

	gTetromino[5].append(L".X..");
	gTetromino[5].append(L".X..");
	gTetromino[5].append(L".XX.");
	gTetromino[5].append(L"....");

	gTetromino[6].append(L"..X.");
	gTetromino[6].append(L"..X.");
	gTetromino[6].append(L".XX.");
	gTetromino[6].append(L"....");

	for (int x = 0; x < fieldWidth; ++x)
	{
		for (int y = 0; y < fieldHeight; ++y)
		{
			// set all grid spaces to 0 unless it's grid boundaries, then set it to 9
			mainGrid[y * fieldWidth + x] = (x == 0 || x == fieldWidth - 1 || y == fieldHeight - 1) ? 9 : 0;
		}
	}

}

// -----------------------------------------------------------------------------

// simple collision to check if shape fits
bool doesPieceFit(int pTetromino, int pRotation, int pPosX, int pPosY)
{
	for (int x = 0 ; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			// get index into piece
			int tetrominoIndex = getIndexFromRotation(x, y, pRotation);

			// get index into main grid
			int maingridIndex = (pPosY + y) * fieldWidth + (pPosX + x);

			if (pPosX + x >= 0 && pPosX + x < fieldWidth)
			{
				if (pPosY + y >= 0 && pPosY + y < fieldHeight)
				{
					if (gTetromino[pTetromino][tetrominoIndex] == L'X' && mainGrid[maingridIndex] != 0)
					{
						return false;	// fail on first hit
					}
				}
			}
		}
	}
	return true;
}

// -----------------------------------------------------------------------------

int main()
{
	mainGrid.resize(fieldWidth * fieldHeight);
	screen.resize(screenWidth * screenHeight);

	for (int i = 0; i < screenWidth*screenHeight; ++i)
	{
		screen[i] = L' ';
	}

	HANDLE consoleHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(consoleHandle);
	DWORD bytesWritten = 0;

	bool gameover = false;

	// set up tetris shapes
	f();

	// game logic stuff
	int currentPiece = 0;
	int currentRotation = 0;
	int currentX = fieldWidth / 2;
	int currentY = 0;

	// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
	vector<bool> keys(4);
	bool lockRotate = false;
	// 0x25 - ARROW LEFT
	// 0x27 - ARROW RIGHT
	// 0x28 - ARROW DOWN
	// 0x26 - ARROW UP (rotate 90 clockwise)

	int shapeFallSpeed = 20;
	int gameTickCounter = 0;
	bool forceShapeDown = false;
	int pieceCount = 0;
	int playerScore = 0;

	vector<int> lines;

	while (!gameover)
	{
		// ---- game timing
		this_thread::sleep_for(50ms);
		++gameTickCounter;
		forceShapeDown = (gameTickCounter == shapeFallSpeed);

		// ---- input
		for (int k = 0; k < 4; ++k)
		{
			keys[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x27\x28\x26"[k]))) != 0;
		}

		// ---- game logic

		// want to move left
		if (keys[0])
		{
			if (doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY))
			{
				currentX = currentX - 1;
			}
		}

		// want to move right
		if (keys[1])
		{
			if (doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY))
			{
				currentX = currentX + 1;
			}
		}

		// want to move down
		if (keys[2])
		{
			if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1))
			{
				currentY = currentY + 1;
			}
		}

		// want to rotate shape
		if (keys[3])
		{
			if (!lockRotate && doesPieceFit(currentPiece, currentRotation + 1, currentX, currentY))
			{
				currentRotation = currentRotation + 1;
				lockRotate = true;
			}
		}
		else
		{
			lockRotate = false;
		}

		if (forceShapeDown)
		{
			if (doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1))
			{
				++currentY; // it can, so do it
			}
			else
			{
				// lock current piece in main grid
				for (int x = 0; x < 4; ++x)
				{
					for (int y = 0; y < 4; ++y)
					{
						if (gTetromino[currentPiece][getIndexFromRotation(x, y, currentRotation)] == L'X')
						{
							mainGrid[(currentY + y) * fieldWidth + (currentX + x)] = currentPiece + 1;
						}
					}
				}

				++pieceCount;
				if (pieceCount % 10 == 0)
				{
					if (shapeFallSpeed >= 10)
					{
						--shapeFallSpeed;
					}
				}

				// check if full horizontal lines
				for (int y = 0; y < 4; ++y)
				{
					if (currentY + y < fieldHeight - 1)
					{
						bool fullLine = true;
						for (int x = 1; x < fieldWidth - 1; ++x)
						{
							fullLine &= (mainGrid[(currentY + y) * fieldWidth + x]) != 0;
						}

						if (fullLine)
						{
							// remove line
							for (int x = 1; x < fieldWidth - 1; ++x)
							{
								mainGrid[(currentY + y) * fieldWidth + x] = 8;
							}

							lines.push_back(currentY + y);
						}
					}
				}

				playerScore += 25;

				// choose next piece
				currentX = fieldWidth / 2;
				currentY = 0;
				currentRotation = 0;
				currentPiece = rand() % 7;
				//currentPiece = 0;

				// if piece does not fit
				gameover = !doesPieceFit(currentPiece, currentRotation, currentX, currentY);
			}

			gameTickCounter = 0;
		}

		// ---- render output

		// ---- draw game grid
		for (int x = 0; x < fieldWidth; ++x)
		{
			for (int y = 0; y < fieldHeight; ++y)
			{
				screen[(y + 2) * screenWidth + (x + 2)] = L" ABCDEFG=#"[mainGrid[y * fieldWidth + x]];
			}
		}

		// ---- draw current piece
		for (int x = 0; x < 4; ++x)
		{
			for (int y = 0; y < 4; ++y)
			{
				if (gTetromino[currentPiece][getIndexFromRotation(x, y, currentRotation)] == L'X')
				{
					screen[(currentY + y + 2) * screenWidth + (currentX + x + 2)] = currentPiece + 65;
				}
			}
		}

		// ---- draw score
		swprintf_s(&screen[2*screenWidth + fieldWidth + 6], 16, L"SCORE: %8d", playerScore);

		if (!lines.empty())
		{
			// display frame (cheekily to draw lines)
			WriteConsoleOutputCharacter(consoleHandle, &screen[0], screenWidth* screenHeight, { 0,0 }, & bytesWritten);
			this_thread::sleep_for(400ms); // delay a bit

			for (int line : lines)
			{
				for (int x = 1; x < fieldWidth - 1; ++x)
				{
					for (int y = line; y > 0; --y)
					{
						mainGrid[y * fieldWidth + x] = mainGrid[(y - 1) * fieldWidth + x];
					}

					mainGrid[x] = 0;
				}
			}

			playerScore += (1 << lines.size()) * 100;

			lines.clear();
		}
		 
		// display frame
		WriteConsoleOutputCharacter(consoleHandle, &screen[0], screenWidth * screenHeight, { 0,0 }, &bytesWritten);
	}

	CloseHandle(consoleHandle);

	cout << "!!No more moves!!";
	cout << "SCORE: " << playerScore << '\n';

	system("pause");

	//_CrtDumpMemoryLeaks();
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
