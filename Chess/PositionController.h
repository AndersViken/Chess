#pragma once
#include "Move.h"
#include "Position.h"
class PositionController
{
public:
	PositionController();
	~PositionController();

	Position generateNewPosition(Move const &mov, Position &pos);
};

