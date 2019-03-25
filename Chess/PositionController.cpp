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

int const lowestSquareID = 0;
int const highestSquareID = 63;

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
	if (checkIfMovingToOwnColorPiece(oldPosition, move.toSquareId, newPosition)) {
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
		if (checkIfMovingToOppositeColorPiece(oldPosition, move.toSquareId, newPosition) == false) {
			return false;
		}
	}
	else if (numberOfColsMoved == 0) {
		if (checkIfMovingToPiece(oldPosition, move.toSquareId) == true) {
			return false;
		}
	}
	
	return true;
}



bool PositionController::checkIfMovingToOwnColorPiece(Position &oldPosition, int const newSquareID, Position &newPosition)
{
	// Check if trying to move to a square with piece of own color
	int const colorNotFound{ -1 };
	int const takenColor{  getColorFromType(oldPosition.getPiecePlacement().at(newSquareID),colorNotFound) };
	int const activeColor{ getColorFromType(newPosition.getPiecePlacement().at(newSquareID),colorNotFound) };
	if (takenColor != colorNotFound && takenColor == activeColor) {
		return true;
	}
	return false;
}

bool PositionController::checkIfMovingToOppositeColorPiece(Position &oldPosition, int const newSquareID, Position &newPosition)
{
	int const colorNotFound{ -1 };
	// Check if trying to move to a square with piece of opposite color
	int const takenColor{ getColorFromType(oldPosition.getPiecePlacement().at(newSquareID),colorNotFound) };
	int const activeColor{ getColorFromType(newPosition.getPiecePlacement().at(newSquareID),colorNotFound) };
	if (takenColor != colorNotFound && takenColor != activeColor) {
		return true;
	}
	return false;
}
bool PositionController::checkIfMovingToOppositeColorPiece(Position &position, Location newLocation)
{
	int const squareID = getSquareIDFromLocation(newLocation);
	return checkIfMovingToOppositeColorPiece(position, squareID, position);
}

bool PositionController::checkIfMovingToPiece(Position & oldPosition, int const newSquareID)
{
	// Check if trying to move to a square with a piece
	int const colorNotFound{ -1 };
	int const takenColor{ getColorFromType(oldPosition.getPiecePlacement().at(newSquareID),colorNotFound) };
	if (takenColor != colorNotFound) {
		return true;
	}
	return false;
}
bool PositionController::checkIfMovingToPiece(Position & oldPosition, Location newLocation)
{
	int const squareID = getSquareIDFromLocation(newLocation);
	return checkIfMovingToPiece(oldPosition, squareID);
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

std::vector<Move> PositionController::getValidMoves(Position &position, std::vector<Piece*> &pieces)
{
	std::vector<Move> validMoves{};

	int activeColor{ position.getActiveColorInt() };

	std::for_each(pieces.begin(), pieces.end(), [this, activeColor, &position, &validMoves](Piece* piece) mutable {
		if (piece->getColor() != activeColor) {
			getValidMovesForPiece(position, piece, validMoves);
		}
	});

	return validMoves;
}

void PositionController::getValidMovesForPiece(Position & position, Piece *& piece, std::vector<Move> &moves)
{
	switch (piece->getPieceType()) {

	case blackPawn:
	case whitePawn:		getValidMovesForPawn(position, piece, moves);	break;

	case blackRook:
	case whiteRook:		getValidMovesForRook(position, piece, moves);	break;

	case blackBishop:
	case whiteBishop:	getValidMovesForBishop(position, piece, moves); break;

	case blackKnight:
	case whiteKnight:	getValidMovesForKnight(position, piece, moves); break;

	case blackQueen:
	case whiteQueen:	getValidMovesForQueen(position, piece, moves);	break;

	case blackKing:
	case whiteKing:		getValidMovesForKing(position, piece, moves);	break;
		
	}
	return;
}

void PositionController::getValidMovesForPawn(Position& position, Piece*& piece, std::vector<Move>& moves)
{
	// TODO: add logic similar to validatemove for all functions here. remember to consider checks on king before and after move.
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"Pawn" });

	int piecetype = piece->getPieceType();
	Direction forward;
	Direction diagonalForwardLeft;
	Direction diagonalForwardRight;
	int startRow;
	if (piecetype == whitePawn) {
		forward = Direction::up;
		diagonalForwardLeft = Direction::diagonalLeftUp;
		diagonalForwardRight = Direction::diagonalRightUp;
		startRow = whiteStartRow;
	}
	else {
		forward = Direction::down;
		diagonalForwardLeft = Direction::diagonalLeftDown;
		diagonalForwardRight = Direction::diagonalRightDown;
		startRow = blackStartRow;
	}
	Location location = getLocationFromSquareID(piece->getSquareID());
	Location origLocation = location;

	moveLocation(location, forward);
	checkIfValidMovePawnForward(location, position, moves, piece);
	location = origLocation;

	if (location.row == startRow) {
		moveLocation(location, forward);
		moveLocation(location, forward);
		checkIfValidMovePawnForward(location, position, moves, piece);
		location = origLocation;
	}

	moveLocation(location, diagonalForwardLeft);
	checkIfValidMovePawnDiagonal(location, position, moves, piece);
	location = origLocation;

	moveLocation(location, diagonalForwardRight);
	checkIfValidMovePawnDiagonal(location, position, moves, piece);
	location = origLocation;

	/*int const numberOfColsMoved{ colsMoved(move) };
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
	else if (numberOfRowsMoved != 1) {
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

	return true;*/

}

void PositionController::checkIfValidMovePawnForward(Location const & location, Position & position, std::vector<Move> & moves, Piece *& piece)
{
	if (checkIfSquare(location)) {
		if (checkIfMovingToPiece(position, location) == false) {
			addValidMove(moves, piece->getSquareID(), getSquareIDFromLocation(location));
		}
	}
}
void PositionController::checkIfValidMovePawnDiagonal(Location const & location, Position & position, std::vector<Move> & moves, Piece *& piece)
{
	if (checkIfSquare(location)) {
		if (checkIfMovingToOppositeColorPiece(position, location)) {
			addValidMove(moves, piece->getSquareID(), getSquareIDFromLocation(location));
		}
	}
}

void PositionController::getValidMovesForRook(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"Rook" });
}

void PositionController::getValidMovesForBishop(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"Bishop" });
}

void PositionController::getValidMovesForKnight(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"Knight" });
}

void PositionController::getValidMovesForQueen(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"Queen" });
}

void PositionController::getValidMovesForKing(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"King" });
}

bool PositionController::checkIfSquare(Location location)
{
	
	if (location.row >= 0 && location.row < squaresInARow &&
		location.col >= 0 && location.col < squaresInARow) {
		qDebug() << "checkIfSquare(): square: with row: " << location.row << " col: " << location.col << "is a square";
		return true;
	}
	qDebug() << "checkIfSquare(): square: with row: " << location.row << " col: " << location.col << "is NOT a square";

	return false;
}

Location PositionController::getLocationFromSquareID(int const squareID)
{
	return{ squareID / squaresInARow, squareID % squaresInARow };
}

int PositionController::getSquareIDFromLocation(Location location)
{
	return { location.row*squaresInARow + location.col };
}

void PositionController::moveLocation(Location& location, Direction const direction)
{
	switch (direction)
	{
	case Direction::left:
		location.col--;
		break;
	case Direction::right:
		location.col++;
		break;
	case Direction::up:
		location.row--;
		break;
	case Direction::down:
		location.row++;
		break;
	case Direction::diagonalLeftUp:
		location.col--;
		location.row--;
		break;
	case Direction::diagonalRightUp:
		location.col++;
		location.row--;
		break;
	case Direction::diagonalLeftDown:
		location.col--;
		location.row++;
		break;
	case Direction::diagonalRightDown:
		location.col++;
		location.row++;
		break;
	default:
		break;
	}
	return;
}

void PositionController::addValidMove(std::vector<Move>& moves, int const fromSquareID, int const toSquareID)
{
	moves.push_back({ fromSquareID,toSquareID,"" });
}



//void PositionController::moveLocation(Location location, Direction direction)
//{
//	if (direction == Direction::up) {
//		location.row--;
//	}	
//}



