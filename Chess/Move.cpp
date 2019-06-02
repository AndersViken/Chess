#include "Move.h"



bool Move::operator==(const Move& lhs)
{
	return lhs.fromSquareId == fromSquareId && lhs.toSquareId == toSquareId;
}
