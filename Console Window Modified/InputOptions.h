// -----------------------------------------------------------------------------
#ifndef InputOptions_H
#define InputOptions_H
// -----------------------------------------------------------------------------

//--INCLUDES--//


// -----------------------------------------------------------------------------

class InputOptions
{
public:
	InputOptions() {}

	InputOptions(int pLeft, int pRight, int pDown, int pRotate, int pQuit, int pPause)
		: mLeft(pLeft), mRight(pRight), mDown(pRight), mRotate(pRotate), mQuit(pQuit), mPause(pPause)
	{}

	void setLeft(int pLeft) { mLeft = pLeft; }
	void setRight(int pRight) { pRight = pRight; }
	void setDown(int pDown) { mDown = pDown; }
	void setRotate(int pRotate) { mRotate = pRotate; }
	void setQuit(int pQuit) { mQuit = pQuit; }
	void setPause(int pPause) { mPause = pPause; }

private:
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
