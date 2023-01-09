// -----------------------------------------------------------------------------
#ifndef Tetromino_H
#define Tetromino_H
// -----------------------------------------------------------------------------

//--INCLUDES--//
#include <string>

using namespace std;

// -----------------------------------------------------------------------------

enum class TetrominoShapeType
{
	e_I,
	e_J,
	e_L,
	e_O,
	e_S,
	e_T,
	e_Z
};

// -----------------------------------------------------------------------------

class TetrominoShape
{
	TetrominoShape(TetrominoShapeType pShapeType) 
	: mShapeType(pShapeType)
	{}

private:
	TetrominoShapeType mShapeType;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
#endif // !Tetromino_H
