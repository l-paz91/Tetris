// -----------------------------------------------------------------------------
#ifndef InputOptions_H
#define InputOptions_H
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <windows.h>

// -----------------------------------------------------------------------------

class InputOptions
{
public:
	InputOptions() {}

	InputOptions(int pLeft, int pRight, int pDown, int pRotate, int pQuit, int pPause)
		: mLeft(pLeft), mRight(pRight), mDown(pDown), mRotate(pRotate), mQuit(pQuit), mPause(pPause)
	{}

	int getKeyPress()
	{
		if (0x8000 & GetAsyncKeyState(mLeft))
			return mLeft;
		if (0x8000 & GetAsyncKeyState(mRight))
			return mRight;
		if (0x8000 & GetAsyncKeyState(mDown))
			return mDown;
		if (0x8000 & GetAsyncKeyState(mRotate))
			return mRotate;
		if (0x8000 & GetAsyncKeyState(mQuit))
			return mQuit;
		if (0x8000 & GetAsyncKeyState(mPause))
			return mPause;

		return 0;
	}

	void setLeft(int pLeft) { mLeft = pLeft; }
	void setRight(int pRight) { pRight = pRight; }
	void setDown(int pDown) { mDown = pDown; }
	void setRotate(int pRotate) { mRotate = pRotate; }
	void setQuit(int pQuit) { mQuit = pQuit; }
	void setPause(int pPause) { mPause = pPause; }

//private:
	int mLeft;
	int mRight;
	int mDown;
	int mRotate;
	int mQuit;
	int mPause;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !InputOptions_H
