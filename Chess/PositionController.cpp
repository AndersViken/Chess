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

std::map<int, int> colorFromType{
	{ whitePawn,  white	},
	{ whiteRook,  white	},
	{ whiteKnight,white	},
	{ whiteBishop,white	},
	{ whiteQueen, white	},
	{ whiteKing,  white },
	{ blackPawn,  black },
	{ blackRook,  black },
	{ blackKnight,black },
	{ blackBishop,black },
	{ blackQueen, black },
	{ blackKing,  black }
};

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

bool PositionController::validateMove(Position newPosition, Position oldPosition, Move move, int pieceType)
{
	if (move.fromSquareId == move.toSquareId) {
		return false;
	}
	// TODO: obviously needs more logic
	if (pieceType == blackKnight || pieceType == whiteKnight) {
		if (ValidateKnightMove(move) == false) {
			return false;
		}
	}
	if (pieceType == blackRook || pieceType == whiteRook) {
		if (validateRookMove(move) == false) {
			return false;
		};
	}
	if (checkIfMovingToOwnColorPiece(oldPosition, move, newPosition) == false) {
		return false;
	}
	return true;
}

bool PositionController::validateRookMove(Move &move)
{
	if (colsMoved(move) > 0 && rowsMoved(move) > 0) {
		return false;
	}
	return true;
}

bool PositionController::ValidateKnightMove(Move &move)
{
	if (colsMoved(move) == 2) {
		if (rowsMoved(move) != 1) {
			return false;
		}
	}
	else if (colsMoved(move) == 1) {
		if (rowsMoved(move) != 2) {
			return false;
		}
	}
	else {
		return false;
	}
	return true;
}

bool PositionController::checkIfMovingToOwnColorPiece(Position &oldPosition, Move &move, Position &newPosition)
{
	// Check if trying to move to a square with piece of own color
	int takenType = oldPosition.getPiecePlacement().at(move.toSquareId);
	int activeType = newPosition.getPiecePlacement().at(move.toSquareId);
	int takenColor{ getColorFromType(takenType) };
	int activeColor{ getColorFromType(activeType) };

	if (takenColor != 0 && takenColor == activeColor) {
		return false;
	}
	return true;
}

int getColorFromType(int pieceType)
{
	auto colorSearch{ colorFromType.find(pieceType) };
	if (colorSearch != colorFromType.end()) {
		return colorSearch->second;
	}
	return{};
}

int PositionController::rowsMoved(Move move)
{
	int const fromRow = move.fromSquareId / 8;
	int const toRow = move.toSquareId / 8;
	return std::abs(fromRow - toRow);
}

int PositionController::colsMoved(Move move)
{
	int const fromCol = move.fromSquareId % 8;
	int const toCol = move.toSquareId % 8;
	return std::abs(fromCol - toCol);
}



//QChar PositionController::getFenCharFromSquareID(QString fenString, int const squareID)
//{
//	QChar ch{};
//
//
//	return ch;
//}

