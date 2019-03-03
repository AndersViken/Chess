#pragma once
#include "Move.h"
#include "Position.h"
class PositionController
{
public:
	PositionController();
	~PositionController();

	Position generateNewPosition(Move &mov, Position &oldPos);
	void updateFenStringForNewPosition(Position &position);
	//QChar getFenCharFromSquareID(QString fenString, int const squareID);
	bool validateMove(Position newPosition, Position oldPosition, Move move);

};

