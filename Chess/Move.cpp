#include "Move.h"



bool Move::operator==(const Move& lhs)
{
	return lhs.fromSquareId == fromSquareId && lhs.toSquareId == toSquareId;
}

Move & Move::operator=(const Move & other)
{
	if (this != &other) {
		fromSquareId = other.fromSquareId;
		toSquareId = other.toSquareId;
		moveString = other.moveString;
		moveType = other.moveType;
		promotionToPieceType = other.promotionToPieceType;
	}
	return *this;
}
