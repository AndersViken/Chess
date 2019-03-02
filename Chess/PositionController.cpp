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
	//QString fenString = oldPos.getFenString();
	//QChar const pieceToMove = getFenCharFromSquareID(oldPos.getFenString(), mov.fromSquareId);
	//fenString.replace(mov.toSquareId,pieceToMove);
	Position newPosition{ oldPos.getFenString() };
	newPosition.insertNewMove(mov);

	return newPosition;
}

bool PositionController::validateMove(Position newPosition, Position oldPosition, Move move)
{
	if (move.fromSquareId == move.toSquareId)
	{
		return false;
	}
	else
	{
		// TODO: obviously needs more logic
		return true;
	}
}


//QChar PositionController::getFenCharFromSquareID(QString fenString, int const squareID)
//{
//	QChar ch{};
//
//
//	return ch;
//}

