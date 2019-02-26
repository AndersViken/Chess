#include "PositionController.h"
#include "PieceInfo.h"
#include "qchar.h"


PositionController::PositionController()
{
}


PositionController::~PositionController()
{
}

Position PositionController::generateNewPosition(Move & mov, Position & oldPos)
{
	QString fenString = oldPos.getFenString();
	QChar const pieceToMove = oldPos.getFenString().at(mov.fromSquareId);
	fenString.replace(mov.toSquareId,pieceToMove);
	Position newPosition{ oldPos.getFenString() };
	newPosition.insertNewMove(mov);

	return newPosition;
}
