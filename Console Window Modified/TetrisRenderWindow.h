// -----------------------------------------------------------------------------
#ifndef TetrisRenderWindow_H
#define TetrisRenderWindow_H
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <vector>
#include <windows.h>
#include <Winuser.h>

using namespace std;

// -----------------------------------------------------------------------------

class TetrisRenderWindow
{
public:
	TetrisRenderWindow(int pWidth, int pHeight)
		: mConsoleHandle(CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL))
		, mBytesWritten(0)
		, mWindow(pWidth * pHeight)
		, mWidth(pWidth)
		, mHeight(pHeight)
	{
		for (int i = 0; i < mWidth * mHeight; ++i)
		{
			mWindow[i] = L' ';
		}

		SetConsoleActiveScreenBuffer(mConsoleHandle);
	}

	~TetrisRenderWindow()
	{
		CloseHandle(mConsoleHandle);
	}

	void displayFrame()
	{
		WriteConsoleOutputCharacter(mConsoleHandle, &mWindow[0], mWidth * mHeight, { 0,0 }, &mBytesWritten);
	}

	HANDLE mConsoleHandle;
	DWORD mBytesWritten;

	vector<wchar_t> mWindow;
	int mWidth;
	int mHeight;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !TetrisRenderWindow_H
