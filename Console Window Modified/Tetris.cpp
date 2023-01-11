// -----------------------------------------------------------------------------

//--INCLUDES--//
#include "Tetris.h"

#include <thread>

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
	, mLines()
	, mNumTetrominosOnBoard(0)
	, mScore(0)
	, mCurrentTetromino(0)
	, mCurrentRotation(0)
	, mCurrentX(TetrisPrivate::tetrisGridWidth / 2)
	, mCurrentY(0)
	, mPieceCount(0)
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
		}
	}

	// ---- draw current piece
	for (int x = 0; x < 4; ++x)
	{
		for (int y = 0; y < 4; ++y)
		{
			if (mTetrominos[mCurrentTetromino][getTetrominoBlockIndexAfterRotation(x, y, mCurrentRotation)] == L'X')
			{
				pWindow.mWindow[(mCurrentY + y + 2) * screenWidth + (mCurrentX + x + 2)] = mCurrentTetromino + 65;
			}
		}
	}

	// ---- draw score
	swprintf_s(&pWindow.mWindow[2 * screenWidth + tetrisGridWidth + 6], 16, L"SCORE: %8d", mScore);

	// ---- remove line if complete
	if (!mLines.empty())
	{
		// display frame (cheekily to draw lines)
		pWindow.displayFrame();
		this_thread::sleep_for(400ms); // delay a bit

		for (int line : mLines)
		{
			for (int x = 1; x < tetrisGridWidth - 1; ++x)
			{
				for (int y = line; y > 0; --y)
				{
					mTetrisBoard[y * tetrisGridWidth + x] = mTetrisBoard[(y - 1) * tetrisGridWidth + x];
				}

				mTetrisBoard[x] = 0;
			}
		}

		mScore += (1 << mLines.size()) * 100;

		mLines.clear();
	}

	// display frame
	pWindow.displayFrame();
}

// -----------------------------------------------------------------------------

bool Tetris::update(int& pDeltaTime)
{
	using namespace TetrisPrivate;

	mForceTetrominoDown = (pDeltaTime == mTetrominoFallSpeed);

	// ---- input
	int currentKeyPress = mTetrisInputOptions.getKeyPress();

	if (currentKeyPress == mTetrisInputOptions.mLeft)
	{
		if (doesPieceFit(mCurrentTetromino, mCurrentRotation, mCurrentX - 1, mCurrentY))
		{
			mCurrentX = mCurrentX - 1;
		}
	}

	if (currentKeyPress == mTetrisInputOptions.mRight)
	{
		if (doesPieceFit(mCurrentTetromino, mCurrentRotation, mCurrentX + 1, mCurrentY))
		{
			mCurrentX = mCurrentX + 1;
		}
	}

	if (currentKeyPress == mTetrisInputOptions.mDown)
	{
		if (doesPieceFit(mCurrentTetromino, mCurrentRotation, mCurrentX, mCurrentY + 1))
		{
			mCurrentY = mCurrentY + 1;
		}
	}

	if (currentKeyPress == mTetrisInputOptions.mRotate)
	{
		if (!mLockRotate && doesPieceFit(mCurrentTetromino, mCurrentRotation + 1, mCurrentX, mCurrentY))
		{
			mCurrentRotation = mCurrentRotation + 1;
			mLockRotate = true;
		}
	}
	else
	{
		mLockRotate = false;
	}

	if (currentKeyPress == mTetrisInputOptions.mQuit)
	{
		return true;
	}

	if (mForceTetrominoDown)
	{
		if (doesPieceFit(mCurrentTetromino, mCurrentRotation, mCurrentX, mCurrentY + 1))
		{
			++mCurrentY; // it can, so do it
		}
		else
		{
			// lock current piece in main grid
			for (int x = 0; x < 4; ++x)
			{
				for (int y = 0; y < 4; ++y)
				{
					if (mTetrominos[mCurrentTetromino][getTetrominoBlockIndexAfterRotation(x, y, mCurrentRotation)] == L'X')
					{
						mTetrisBoard[(mCurrentY + y) * tetrisGridWidth + (mCurrentX + x)] = mCurrentTetromino + 1;
					}
				}
			}

			++mPieceCount;
			if (mPieceCount % 10 == 0)
			{
				if (mTetrominoFallSpeed >= 10)
				{
					--mTetrominoFallSpeed;
				}
			}

			// check if full horizontal lines
			for (int y = 0; y < 4; ++y)
			{
				if (mCurrentY + y < tetrisGridHeight - 1)
				{
					bool fullLine = true;
					for (int x = 1; x < tetrisGridWidth - 1; ++x)
					{
						fullLine &= (mTetrisBoard[(mCurrentY + y) * tetrisGridWidth + x]) != 0;
					}

					if (fullLine)
					{
						// remove line
						for (int x = 1; x < tetrisGridWidth - 1; ++x)
						{
							mTetrisBoard[(mCurrentY + y) * tetrisGridWidth + x] = 8;
						}

						mLines.push_back(mCurrentY + y);
					}
				}
			}

			mScore += 25;

			// choose next piece
			mCurrentX = tetrisGridWidth / 2;
			mCurrentY = 0;
			mCurrentRotation = 0;
			mCurrentTetromino = rand() % 7;
			//currentPiece = 0;

			// if piece does not fit
			pDeltaTime = 0;
			return !doesPieceFit(mCurrentTetromino, mCurrentRotation, mCurrentX, mCurrentY);
		}

		pDeltaTime = 0;
	}

	return false;
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
