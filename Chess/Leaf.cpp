#include "Leaf.h"
#include <qdebug.h>


Leaf::Leaf(Move move, Position position) : move(move), position(position)
{
	qDebug() << "created leaf with move " << move.fromSquareId << " " << move.toSquareId << " " << move.moveString;
}


Leaf::~Leaf()
{
	qDebug() << "deleted leaf with move " << move.fromSquareId << " " << move.toSquareId << " " << move.moveString;
}
