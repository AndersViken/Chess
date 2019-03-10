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

QString kingsideCastleString = "0-0";
QString queensideCastleString = "0-0-0";

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

	updateFenString(newPosition);

	return newPosition;
}

void PositionController::updateFenString(Position &position)
{
	QChar space = ' ';
	QString fenString = position.createPiecePlacementFenString();
	fenString.append(space);
	fenString.append(position.getActiveColor());
	fenString.append(space);
	fenString.append(position.createCastleFenString());
	fenString.append(space);
	fenString.append(position.getEnPassantSquare());
	fenString.append(space);
	fenString.append(position.createHalfMoveFenString());
	fenString.append(space);
	fenString.append(position.createFullMoveFenString());

	position.setFenString(fenString);
}

void PositionController::updateFenStringForNewPosition(Position &position)
{
}

bool PositionController::validateMove(Position &newPosition, Position &oldPosition, Move move, int pieceType, std::vector<specialMove> &specialMoves)
{
	if (move.fromSquareId == move.toSquareId) {
		return false;
	}
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
		if (validateKingMove(move, oldPosition, newPosition, pieceType, specialMoves) == false) {
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

bool PositionController::validateKingMove(Move const &move, Position &oldPosition, Position &newPosition, int pieceType, std::vector<specialMove> &specialMoves)
{
	if (pieceType == blackKing) {
		if (oldPosition.canBlackCastleKingside() && colsMovedWithSign(move) == -2) {
			if (checkIfPiecesInSquares(oldPosition, { 5,6 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 7, 5, blackRook, kingsideCastleString);
			return true;
		}
		if (oldPosition.canBlackCastleQueenside() && colsMovedWithSign(move) == 2) {
			if (checkIfPiecesInSquares(oldPosition, { 1,2,3 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 0, 3, blackRook, queensideCastleString);
			return true;
		}
	}
	if (pieceType == whiteKing) {
		if (oldPosition.canWhiteCastleKingside() && colsMovedWithSign(move) == -2) {
			if (checkIfPiecesInSquares(oldPosition, { 61,62 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 63, 61, whiteRook, kingsideCastleString);
			return true;
		}
		if (oldPosition.canWhiteCastleQueenside() && colsMovedWithSign(move) == 2) {
			if (checkIfPiecesInSquares(oldPosition, { 57,58,59 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 56, 59, whiteRook, queensideCastleString);
			return true;
		}
	}

	if ((colsMoved(move) > 1) || (rowsMoved(move) > 1)) {
		return false;
	}
	return true;
}

void PositionController::moveRookWhenCastling(Position &newPosition, std::vector<specialMove> &specialMoves,
	int const rookOrigSquare, int const rookNewSquare, int const rookType, QString castleString)
{
	newPosition.insertNewMove({ rookOrigSquare,rookNewSquare,"" });
	specialMoves.push_back({ rookOrigSquare,empty,castleString });
	specialMoves.push_back({ rookNewSquare,rookType,castleString });
	updateFenString(newPosition);
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

bool PositionController::checkIfPiecesInSquares(Position position, std::vector<int> const squares)
{
	int retVal = false;
	std::for_each(squares.begin(), squares.end(), [position, &retVal](int square) mutable {
		//int piece = position.getPiecePlacement().at(square);
		if (position.getPiecePlacement().at(square) != empty) {
			retVal = true;
		}
	});
	return retVal;
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

int PositionController::colsMovedWithSign(Move move)
{
	int const fromCol = move.fromSquareId % 8;
	int const toCol = move.toSquareId % 8;
	return fromCol - toCol;
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

