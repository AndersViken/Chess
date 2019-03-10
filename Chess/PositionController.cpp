#include "PositionController.h"
#include "PieceInfo.h"
#include "GuiSetup.h"
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
	if (pieceType == blackRook || pieceType == whiteRook) {
		if (validateRookMove(move) == false) {
			return false;
		};
	}
	if (pieceType == blackKnight || pieceType == whiteKnight) {
		if (validateKnightMove(move) == false) {
			return false;
		}
	}
	if (pieceType == blackBishop || pieceType == whiteBishop) {
		if (validateBishopMove(move) == false) {
			return false;
		}
	}
	if (pieceType == blackQueen || pieceType == whiteQueen) {
		if (validateQueenMove(move) == false) {
			return false;
		}
	}
	if (pieceType == blackKing || pieceType == whiteKing) {
		if (validateKingMove(move) == false) {
			return false;
		}
	}
	if (pieceType == blackPawn || pieceType == whitePawn) {
		if (validatePawnMove(oldPosition, move, newPosition, pieceType) == false) {
			return false;
		}
	}
	if (checkIfMovingToOwnColorPiece(oldPosition, move, newPosition)) {
		return false;
	}
	return true;
}

bool PositionController::validateBishopMove(Move const &move)
{
	if (colsMoved(move) != rowsMoved(move)) {
		return false;
	}
	return true;
}

bool PositionController::validateRookMove(Move const &move)
{
	if (colsMoved(move) > 0 && rowsMoved(move) > 0) {
		return false;
	}
	return true;
}

bool PositionController::validateKnightMove(Move const &move)
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

bool PositionController::validateQueenMove(Move const &move)
{
	if (validateBishopMove(move) || validateRookMove(move)) {
		return true;
	}
	return false;
}

bool PositionController::validateKingMove(Move const &move)
{
	if ((colsMoved(move) > 1) || (rowsMoved(move) > 1)) {
		return false;
	}
	return true;
}



bool PositionController::validatePawnMove(Position &oldPosition, Move const &move, Position &newPosition, int const piecetype)
{
	int sign;
	int startRow;
	if (piecetype == whitePawn) {
		sign = 1;
		startRow = whiteStartRow;
	}
	else {
		sign = -1;
		startRow = blackStartRow;
	}

	int const numberOfColsMoved{ colsMoved(move) };
	int const numberOfRowsMoved{ sign * rowsMovedWithSign(move) };

	bool movedFromStartRow{ false };
	if (move.fromSquareId / squaresInARow == startRow) {
		movedFromStartRow = true;
	}
	if (movedFromStartRow) {
		if ((numberOfRowsMoved < 1) || (numberOfRowsMoved > 2)) {
			return false;
		}
	}
	else if (numberOfRowsMoved !=  1) {
		return false;
	}

	if (numberOfColsMoved > 1) {
		return false;
	}
	else if (numberOfColsMoved == 1) {
		if (checkIfMovingToOppositeColorPiece(oldPosition, move, newPosition) == false) {
			return false;
		}
	}
	else if (numberOfColsMoved == 0) {
		if (checkIfMovingToPiece(oldPosition, move) == true) {
			return false;
		}
	}
	
	return true;
}



bool PositionController::checkIfMovingToOwnColorPiece(Position &oldPosition, Move const &move, Position &newPosition)
{
	// Check if trying to move to a square with piece of own color
	int const colorNotFound{ -1 };
	int const takenColor{  getColorFromType(oldPosition.getPiecePlacement().at(move.toSquareId),colorNotFound) };
	int const activeColor{ getColorFromType(newPosition.getPiecePlacement().at(move.toSquareId),colorNotFound) };
	if (takenColor != colorNotFound && takenColor == activeColor) {
		return true;
	}
	return false;
}bool PositionController::checkIfMovingToOppositeColorPiece(Position &oldPosition, Move const &move, Position &newPosition)
{
	int const colorNotFound{ -1 };
	// Check if trying to move to a square with piece of own color
	int const takenColor{  getColorFromType(oldPosition.getPiecePlacement().at(move.toSquareId),colorNotFound) };
	int const activeColor{ getColorFromType(newPosition.getPiecePlacement().at(move.toSquareId),colorNotFound) };
	if (takenColor != colorNotFound && takenColor != activeColor) {
		return true;
	}
	return false;
}

bool PositionController::checkIfMovingToPiece(Position & oldPosition, Move const & move)
{
	// Check if trying to move to a square with a piece
	int const colorNotFound{ -1 };
	int const takenColor{ getColorFromType(oldPosition.getPiecePlacement().at(move.toSquareId),colorNotFound) };
	if (takenColor != colorNotFound) {
		return true;
	}
	return false;
}

int PositionController::rowsMoved(Move move)
{
	int const fromRow = move.fromSquareId / 8;
	int const toRow = move.toSquareId / 8;
	return std::abs(fromRow - toRow);
}
int PositionController::rowsMovedWithSign(Move move)
{
	int const fromRow = move.fromSquareId / 8;
	int const toRow = move.toSquareId / 8;
	return fromRow - toRow;
}

int PositionController::colsMoved(Move move)
{
	int const fromCol = move.fromSquareId % 8;
	int const toCol = move.toSquareId % 8;
	return std::abs(fromCol - toCol);
}

int PositionController::getColorFromType(int pieceType, int const returnValueIfNotFound)
{
	auto colorSearch{ colorFromType.find(pieceType) };
	if (colorSearch != colorFromType.end()) {
		return colorSearch->second;
	}
	return returnValueIfNotFound;
}



//QChar PositionController::getFenCharFromSquareID(QString fenString, int const squareID)
//{
//	QChar ch{};
//
//
//	return ch;
//}

