// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Tetris.h"

// -----------------------------------------------------------------------------

namespace 
{
	namespace TetrisPrivate
	{
		constexpr int tetrisGridWidth = 12;
		constexpr int tetrisGridHeight = 18;
		constexpr int tetrominoFallSpeed = 20;
		constexpr int screenWidth = 120;
		constexpr int screenHeight = 30;
	}
}

// -----------------------------------------------------------------------------

Tetris::Tetris()
	: mTetrisInputOptions(VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP, VK_ESCAPE, VK_SPACE)
	, mTetrisBoard(TetrisPrivate::tetrisGridWidth * TetrisPrivate::tetrisGridHeight)
	, mTetrominos(7)
	, mNumTetrominosOnBoard(0)
	, mScore(0)
	, mCurrentTetromino(0)
	, mCurrentRotation(0)
	, mCurrentX(TetrisPrivate::tetrisGridWidth / 2)
	, mCurrentY(0)
	, mTetrominoFallSpeed(TetrisPrivate::tetrominoFallSpeed)
	, mForceTetrominoDown(false)
	, mLockRotate(false)
{
	setupTetrisBoard();
}

// -----------------------------------------------------------------------------

void Tetris::render(TetrisRenderWindow& pWindow)
{
	using namespace TetrisPrivate;

	// ---- draw game grid
	for (int x = 0; x < tetrisGridWidth; ++x)
	{
		for (int y = 0; y < tetrisGridHeight; ++y)
		{
			pWindow.mWindow[(y + 2) * screenWidth + (x + 2)] = L" ABCDEFG=#"[mTetrisBoard[y * tetrisGridWidth + x]];
			pWindow.displayFrame();
		}
	}

	// ---- draw current piece

	// ---- draw score

	// display frame
	pWindow.displayFrame();
}

// -----------------------------------------------------------------------------

void Tetris::update(const int& pDeltaTime)
{
	using namespace TetrisPrivate;

}

// -----------------------------------------------------------------------------

void Tetris::setupTetrisBoard()
{
	// create assets
	mTetrominos[0].append(L"..X.");
	mTetrominos[0].append(L"..X.");
	mTetrominos[0].append(L"..X.");
	mTetrominos[0].append(L"..X.");

	mTetrominos[1].append(L"..X.");
	mTetrominos[1].append(L".XX.");
	mTetrominos[1].append(L"..X.");
	mTetrominos[1].append(L"....");

	mTetrominos[2].append(L"....");
	mTetrominos[2].append(L".XX.");
	mTetrominos[2].append(L".XX.");
	mTetrominos[2].append(L"....");

	mTetrominos[3].append(L"..X.");
	mTetrominos[3].append(L".XX.");
	mTetrominos[3].append(L".X..");
	mTetrominos[3].append(L"....");

	mTetrominos[4].append(L".X..");
	mTetrominos[4].append(L".XX.");
	mTetrominos[4].append(L"..X.");
	mTetrominos[4].append(L"....");

	mTetrominos[5].append(L".X..");
	mTetrominos[5].append(L".X..");
	mTetrominos[5].append(L".XX.");
	mTetrominos[5].append(L"....");

	mTetrominos[6].append(L"..X.");
	mTetrominos[6].append(L"..X.");
	mTetrominos[6].append(L".XX.");
	mTetrominos[6].append(L"....");

	using namespace TetrisPrivate;

	for (int x = 0; x < tetrisGridWidth; ++x)
	{
		for (int y = 0; y < tetrisGridHeight; ++y)
		{
			// set all grid spaces to 0 unless it's grid boundaries, then set it to 9
			mTetrisBoard[y * tetrisGridWidth + x] = (x == 0 || x == tetrisGridWidth - 1 || y == tetrisGridHeight - 1) ? 9 : 0;
		}
	}
}

// -----------------------------------------------------------------------------

int Tetris::getTetrominoBlockIndexAfterRotation(int pX, int pY, int pR)
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
		return 0;
	}
}

// -----------------------------------------------------------------------------

bool Tetris::doesPieceFit(int pTetromino, int pRotation, int pPosX, int pPosY)
{
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			// get index into piece
			int tetrominoIndex = getTetrominoBlockIndexAfterRotation(x, y, pRotation);

			// get index into main grid
			int maingridIndex = (pPosY + y) * TetrisPrivate::tetrisGridWidth + (pPosX + x);

			if (pPosX + x >= 0 && pPosX + x < TetrisPrivate::tetrisGridWidth)
			{
				if (pPosY + y >= 0 && pPosY + y < TetrisPrivate::tetrisGridHeight)
				{
					if (mTetrominos[pTetromino][tetrominoIndex] == L'X' && mTetrisBoard[maingridIndex] != 0)
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
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
