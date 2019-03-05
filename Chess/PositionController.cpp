#include "PositionController.h"
#include "PieceInfo.h"
#include "qchar.h"
#include <qdebug.h>


PositionController::PositionController()
{
}


PositionController::~PositionController()
{
}

Position PositionController::generateNewPosition(Move & mov, Position & oldPosition)
{
	Position newPosition{ oldPosition.getFenString() };
	newPosition.insertNewMove(mov);

	if (oldPosition.getActiveColorInt() == black) {
		newPosition.setFullMove(oldPosition.getFullMove() + 1);
	}
	newPosition.updateActiveColor();

	// need to update all position parameters here.
	// - waiting with castle
	// - waiting with en passant
	// - waiting with half move

	qDebug() << " new position. fullmove=" << newPosition.getFullMove();

	QChar space = ' ';
	QString fenString = newPosition.createPiecePlacementFenString();
	fenString.append(space);
	fenString.append(newPosition.getActiveColor());
	fenString.append(space);
	fenString.append(newPosition.createCastleFenString());
	fenString.append(space);
	fenString.append(newPosition.getEnPassantSquare());
	fenString.append(space);
	fenString.append(newPosition.createHalfMoveFenString());
	fenString.append(space);
	fenString.append(newPosition.createFullMoveFenString());

	newPosition.setFenString(fenString);


	return newPosition;
}

void PositionController::updateFenStringForNewPosition(Position &position)
{
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

