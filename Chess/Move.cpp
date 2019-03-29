#include "Move.h"

Move::Move()
{
}

Move::Move(int fromSquareId, int toSquareId) : fromSquareId(fromSquareId), toSquareId(toSquareId)
{
	moveString = "";
	moveType = MoveType::normal;
}

Move::Move(int fromSquareId, int toSquareId, QString moveString) : fromSquareId(fromSquareId), toSquareId(toSquareId),moveString(moveString)
{
	moveType = MoveType::normal;
}
Move::Move(int fromSquareId, int toSquareId, QString moveString, MoveType moveType) : fromSquareId(fromSquareId), toSquareId(toSquareId),moveString(moveString),moveType(moveType)
{
}


Move::~Move()
{
}

bool Move::operator==(const Move& lhs)
{
	return lhs.fromSquareId == fromSquareId && lhs.toSquareId == toSquareId;
}
