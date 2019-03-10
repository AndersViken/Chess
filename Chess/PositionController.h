#pragma once
#include "Move.h"
#include "Position.h"
#include <cmath>
class PositionController
{
public:
	PositionController();
	~PositionController();

	Position generateNewPosition(Move &mov, Position &oldPos);
	void updateFenStringForNewPosition(Position &position);
	//QChar getFenCharFromSquareID(QString fenString, int const squareID);
	bool validateMove(Position newPosition, Position oldPosition, Move move, int pieceType);
	bool validateRookMove(Move &move);
	bool ValidateKnightMove(Move &move);
	bool checkIfMovingToOwnColorPiece(Position &oldPosition, Move &move, Position &newPosition);
	int rowsMoved(Move move);
	int colsMoved(Move move);

};

int getColorFromType(int pieceType);
