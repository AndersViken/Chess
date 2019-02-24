#include "Leaf.h"
#include <qdebug.h>


Leaf::Leaf(Move move, Position previousPosition) : move(move), previousPosition(previousPosition)
{
	qDebug() << "created leaf with move " << move.fromSquareId << " " << move.toSquareId << " " << move.moveString;

	// set position, based on previous position and move
	position = generateCurrentPosition(move, previousPosition);
}


Leaf::~Leaf()
{
	qDebug() << "deleted leaf with move " << move.fromSquareId << " " << move.toSquareId << " " << move.moveString;
}

Position Leaf::generateCurrentPosition(Move mov, Position pos)
{
	QString fenString;
	return Position(fenString);
}
