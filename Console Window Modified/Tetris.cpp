// -----------------------------------------------------------------------------
#ifndef Tetris_H
#define Tetris_H
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <vector>
#include <string>

#include "InputOptions.h"
#include "ObjectUpdateInterface.h"
#include "TetrisRenderWindow.h"

using namespace std;

// -----------------------------------------------------------------------------

class Tetris
	: public ObjectUpdateInterface<TetrisRenderWindow, int>
{
public:
	Tetris();

	virtual void render(TetrisRenderWindow& pWindow);
	virtual bool update(int& pDeltaTime);

	const int getPlayerScore() const { return mScore; }

private:
	void setupTetrisBoard();
	int getTetrominoBlockIndexAfterRotation(int pX, int pY, int pR);
	bool doesPieceFit(int pTetromino, int pRotation, int pPosX, int pPosY);

	InputOptions mTetrisInputOptions;
	vector<char> mTetrisBoard;
	vector<wstring> mTetrominos;
	vector<int> mLines;

	int mNumTetrominosOnBoard;
	int mScore;

	int mCurrentTetromino;
	int mCurrentRotation;
	int mCurrentX;
	int mCurrentY;
	int mPieceCount;

	int mTetrominoFallSpeed;
	bool mForceTetrominoDown;
	bool mLockRotate;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !Tetris_H
