
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

#include "Tetris.h"
#include "TetrisRenderWindow.h"

using namespace std;

// -----------------------------------------------------------------------------

void playTetris()
{
	TetrisRenderWindow renderWindow(80, 30);
	Tetris tetrisGame;

	bool gameover = false;
	int gameTickCounter = 0;

	vector<int> lines;

	while (!gameover)
	{
		// ---- game timing
		this_thread::sleep_for(50ms);
		++gameTickCounter;
	
		// ---- UPDATE ---- //
		
		// ---- input
	
		// ---- game logic

		gameover = tetrisGame.update(gameTickCounter);


		// ---- RENDER ---- //

		// ---- draw game grid
	
		// ---- draw current piece

		// ---- draw score
		 
		// display frame

		tetrisGame.render(renderWindow);	
	}

	cout << "!!No more moves!!";
	cout << "SCORE: " << tetrisGame.getPlayerScore() << '\n';

	Sleep(1000);
}

// -----------------------------------------------------------------------------

int main()
{
	playTetris();

	//_CrtDumpMemoryLeaks();
	return 0;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
