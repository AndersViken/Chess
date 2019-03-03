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
	Position newPosition{ oldPos.getFenString() };
	newPosition.insertNewMove(mov);
	updateFenStringForNewPosition(newPosition);
	return newPosition;
}

void PositionController::updateFenStringForNewPosition(Position &position)
{
	QString fenString = position.getFenString();
	QChar space = ' ';
	QChar whiteColor = 'w';
	QChar blackColor = 'b';
	auto it = std::find(fenString.begin(), fenString.end(), space);
	std::advance(it, 1);
	if (it != fenString.end()) {
		QChar activeColor = *(it);
		activeColor = (activeColor == whiteColor) ? blackColor : whiteColor;
		*(it) = activeColor;
		position.setActiveColor(activeColor);
		position.setFenString(fenString);
	}
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

