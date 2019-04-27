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

std::map<PieceType, int> colorFromType{
	{ PieceType::whitePawn,  white	},
	{ PieceType::whiteRook,  white	},
	{ PieceType::whiteKnight,white	},
	{ PieceType::whiteBishop,white	},
	{ PieceType::whiteQueen, white	},
	{ PieceType::whiteKing,  white },
	{ PieceType::blackPawn,  black },
	{ PieceType::blackRook,  black },
	{ PieceType::blackKnight,black },
	{ PieceType::blackBishop,black },
	{ PieceType::blackQueen, black },
	{ PieceType::blackKing,  black }
};

std::map<PieceType, QChar> pieceChars2 = {
	{ PieceType::whiteRook,    'R' },
	{ PieceType::whiteKnight,  'N' },
	{ PieceType::whiteBishop,  'B'	},
	{ PieceType::whiteQueen,   'Q' },
	{ PieceType::whiteKing,    'K' },
	{ PieceType::blackRook,    'R' },
	{ PieceType::blackKnight,  'N' },
	{ PieceType::blackBishop,  'B' },
	{ PieceType::blackQueen,   'Q' },
	{ PieceType::blackKing,    'K' }
};

int const lowestSquareID = 0;
int const highestSquareID = 63;

std::vector<int> const whiteKingsideCastleSquares{ 61,62 };
std::vector<int> const whiteQueensideCastleSquares{ 59,58,57 };
std::vector<int> const blackKingsideCastleSquares{ 5,6 };
std::vector<int> const blackQueensideCastleSquares{ 1,2,3};

std::vector<PieceType> promotionOptionsWhite{ PieceType::whiteBishop, PieceType::whiteKnight,
												PieceType::whiteQueen, PieceType::whiteRook };
std::vector<PieceType> promotionOptionsBlack{ PieceType::blackBishop, PieceType::blackKnight,
												PieceType::blackQueen, PieceType::blackRook };

QString kingsideCastleString = "0-0";
QString queensideCastleString = "0-0-0";

Position PositionController::generateNewPosition(Move & move, Position & oldPosition)
{
	Position newPosition = oldPosition;// { oldPosition.getFenString() };
	newPosition.insertNewMove(move);

	if (oldPosition.getActiveColorInt() == black) {
		newPosition.incrementFullMove();
	}
	newPosition.updateActiveColor();

	if (move.moveType == MoveType::capture ||
		move.moveType == MoveType::pawnMove ||
		move.moveType == MoveType::promotion) {
		newPosition.resetHalfMoveClock();
	}
	else {
		newPosition.incrementHalfMoveClock();
	}
	

	// need to update all position parameters here.
	// - waiting with en passant

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

bool PositionController::validateMove(Position &position, std::vector<Piece*> pieces, Move& move)
{
	std::vector<Move> validMoves = getValidMoves(position, pieces, position.getActiveColorInt(), true);

	auto it = std::find(validMoves.begin(), validMoves.end(), move);
	if (it != validMoves.end()) {

		// changing the parameter move here (to get info about movestring and movetype)
		move = static_cast<Move> (*it);

		//moveType = move.moveType;
		return true;
	}
	else {
		return false;
	}

}

bool PositionController::validateMove(Position &newPosition, Position &oldPosition, Move move, PieceType pieceType, std::vector<specialMove> &specialMoves)
{
	if (move.fromSquareId == move.toSquareId) {
		return false;
	}
	if (pieceType == PieceType::blackRook || pieceType == PieceType::whiteRook) {
		if (validateRookMove(move) == false) {
			return false;
		};
	}
	if (pieceType == PieceType::blackKnight || pieceType == PieceType::whiteKnight) {
		if (validateKnightMove(move) == false) {
			return false;
		}
	}
	if (pieceType == PieceType::blackBishop || pieceType == PieceType::whiteBishop) {
		if (validateBishopMove(move) == false) {
			return false;
		}
	}
	if (pieceType == PieceType::blackQueen || pieceType == PieceType::whiteQueen) {
		if (validateQueenMove(move) == false) {
			return false;
		}
	}
	if (pieceType == PieceType::blackKing || pieceType == PieceType::whiteKing) {
		if (validateKingMove(move, oldPosition, newPosition, pieceType, specialMoves) == false) {
			return false;
		}
	}
	if (pieceType == PieceType::blackPawn || pieceType == PieceType::whitePawn) {
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

bool PositionController::validateKingMove(Move const &move, Position &oldPosition, Position &newPosition, PieceType pieceType, std::vector<specialMove> &specialMoves)
{
	if (pieceType == PieceType::blackKing) {
		if (oldPosition.getCastleLegalBlackKingside() && colsMovedWithSign(move) == -2) {
			if (checkIfPiecesInSquares(oldPosition, { 5,6 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 7, 5, PieceType::blackRook, kingsideCastleString);
			return true;
		}
		if (oldPosition.getCastleLegalBlackQueenside() && colsMovedWithSign(move) == 2) {
			if (checkIfPiecesInSquares(oldPosition, { 1,2,3 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 0, 3, PieceType::blackRook, queensideCastleString);
			return true;
		}
	}
	if (pieceType == PieceType::whiteKing) {
		if (oldPosition.getCastleLegalWhiteKingside() && colsMovedWithSign(move) == -2) {
			if (checkIfPiecesInSquares(oldPosition, { 61,62 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 63, 61, PieceType::whiteRook, kingsideCastleString);
			return true;
		}
		if (oldPosition.getCastleLegalWhiteQueenside() && colsMovedWithSign(move) == 2) {
			if (checkIfPiecesInSquares(oldPosition, { 57,58,59 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 56, 59, PieceType::whiteRook, queensideCastleString);
			return true;
		}
	}

	if ((colsMoved(move) > 1) || (rowsMoved(move) > 1)) {
		return false;
	}
	return true;
}

void PositionController::moveRookWhenCastling(Position &newPosition, std::vector<specialMove> &specialMoves,
	int const rookOrigSquare, int const rookNewSquare, PieceType const rookType, QString castleString)
{
	newPosition.insertNewMove({ rookOrigSquare,rookNewSquare,"" });
	specialMoves.push_back({ rookOrigSquare,PieceType::empty,castleString });
	specialMoves.push_back({ rookNewSquare,rookType,castleString });
	updateFenString(newPosition);
}



bool PositionController::validatePawnMove(Position &oldPosition, Move const &move, Position &newPosition, PieceType const piecetype)
{
	int sign;
	int startRow;
	if (piecetype == PieceType::whitePawn) {
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

bool PositionController::checkIfMovingToOwnColorPiece(Position &position, Location const activeLocation, Location const newLocation)
{
	int const newSquareID = getSquareIDFromLocation(newLocation);
	int const activeSquareID = getSquareIDFromLocation(activeLocation);

	// Check if trying to move to a square with piece of own color
	int const colorNotFound{ -1 };
	int const takenColor{ getColorFromType(position.getPiecePlacement().at(newSquareID),colorNotFound) };
	int const activeColor{ getColorFromType(position.getPiecePlacement().at(activeSquareID),colorNotFound) };
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
bool PositionController::checkIfMovingToOppositeColorPiece(Position &position, int const activeSquareID, int const newSquareID)
{
	int const colorNotFound{ -1 };
	// Check if trying to move to a square with piece of opposite color
	int const takenColor{ getColorFromType(position.getPiecePlacement().at(newSquareID),colorNotFound) };
	int const activeColor{ getColorFromType(position.getPiecePlacement().at(activeSquareID),colorNotFound) };
	if (takenColor != colorNotFound && takenColor != activeColor) {
		return true;
	}
	return false;
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

bool PositionController::checkIfKingAttacked(Position & position, std::vector<Piece*> pieces, int const activeColor)
{
	int kingSquareID{};
	PieceType const kingPieceType{ activeColor == white ? PieceType::blackKing : PieceType::whiteKing };
	auto it{ find_if(pieces.begin(), pieces.end(),[kingPieceType](Piece* piece) {
		return piece->getPieceType() == kingPieceType;
	}) };
	if (it != pieces.end()) {
		Piece* piece = static_cast<Piece*>(*it);
		kingSquareID = piece->getSquareID();
	}
	//qDebug() << "checkIfKingAttacked: " << kingSquareID << " activeColor: " << activeColor;
	return checkIfSquareAttacked(position, pieces, activeColor, kingSquareID);
}

bool PositionController::checkIfSquareAttacked(Position & position, std::vector<Piece*> pieces, int const activeColor, int const squareID)
{
	bool squareAttacked = false;
	bool const isActualMove{ false }; // to avoid recursive infinite calls.
	std::vector<Move> validMoves{ getValidMoves(position, pieces, activeColor, isActualMove) };
	std::for_each(validMoves.begin(), validMoves.end(), [&squareAttacked, squareID](Move &move) {
		if (move.toSquareId == squareID) {
			squareAttacked = true;
			qDebug() << "square " << squareID << " is attacked by square " << move.fromSquareId;
		}
	});

	return squareAttacked;
}


bool PositionController::checkIfPiecesInSquares(Position position, std::vector<int> const squares)
{
	int retVal = false;
	std::for_each(squares.begin(), squares.end(), [position, &retVal](int square) mutable {
		//int piece = position.getPiecePlacement().at(square);
		if (position.getPiecePlacement().at(square) != PieceType::empty) {
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

int PositionController::getColorFromType(PieceType pieceType, int const returnValueIfNotFound)
{
	auto colorSearch{ colorFromType.find(pieceType) };
	if (colorSearch != colorFromType.end()) {
		return colorSearch->second;
	}
	return returnValueIfNotFound;
}

std::vector<Move> PositionController::getValidMoves(Position &position, std::vector<Piece*> &pieces, int const activeColor, bool isActualMove)
{
	std::vector<Move> validMoves{};

	std::for_each(pieces.begin(), pieces.end(),
		[this, activeColor, &position, &pieces, &validMoves, isActualMove](Piece* piece) mutable {
		if (piece->getColor() == activeColor) {
			getValidMovesForPiece(position, pieces, piece, validMoves, isActualMove);
		}
	});

	return validMoves;
}

void PositionController::getValidMovesForPiece(Position & position, std::vector<Piece*> &pieces, Piece *& piece, std::vector<Move> &moves, bool isActualMove)
{
	switch (piece->getPieceType()) {

	case PieceType::blackPawn:
	case PieceType::whitePawn:		getValidMovesForPawn(position, pieces, piece, moves, isActualMove);	break;

	case PieceType::blackRook:
	case PieceType::whiteRook:		getValidMovesForRook(position, pieces, piece, moves, isActualMove);	break;

	case PieceType::blackBishop:
	case PieceType::whiteBishop:	getValidMovesForBishop(position, pieces, piece, moves, isActualMove); break;

	case PieceType::blackKnight:
	case PieceType::whiteKnight:	getValidMovesForKnight(position, pieces, piece, moves, isActualMove); break;

	case PieceType::blackQueen:
	case PieceType::whiteQueen:	getValidMovesForQueen(position, pieces, piece, moves, isActualMove);	break;

	case PieceType::blackKing:
	case PieceType::whiteKing:		getValidMovesForKing(position, pieces, piece, moves, isActualMove);	break;
		
	}
	return;
}

void PositionController::getValidMovesForPawn(Position& position, std::vector<Piece*> &pieces, Piece*& piece, std::vector<Move>& moves, bool isActualMove)
{
	// TODO: add logic similar to validatemove for all functions here. remember to consider checks on king before and after move.
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"Pawn" });

	PieceType piecetype = piece->getPieceType();
	Direction forward;
	Direction diagonalForwardLeft;
	Direction diagonalForwardRight;
	int startRow;
	if (piecetype == PieceType::whitePawn) {
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
	checkIfValidMovePawnForward(location, origLocation, position, pieces, moves, isActualMove);
	location = origLocation;

	if (location.row == startRow) {
		moveLocation(location, forward);
		moveLocation(location, forward);
		checkIfValidMovePawnForward(location, origLocation, position, pieces, moves, isActualMove);
		location = origLocation;
	}

	moveLocation(location, diagonalForwardLeft);
	checkIfValidMovePawnDiagonal(location, origLocation, position, pieces, moves, isActualMove);
	location = origLocation;

	moveLocation(location, diagonalForwardRight);
	checkIfValidMovePawnDiagonal(location, origLocation, position, pieces, moves, isActualMove);
	location = origLocation;

}

void PositionController::checkIfValidMovePawnForward(Location const & location, Location const & origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, bool isActualMove)
{
	if (checkIfSquare(location)) {
		if (checkIfMovingToPiece(position, location) == false) {
			addValidMove(position, moves, pieces, origLocation, location, checkIfPromotion(origLocation, location), isActualMove);
		}
	}
}
void PositionController::checkIfValidMovePawnDiagonal(Location const & location, Location const & origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, bool isActualMove)
{
	if (checkIfSquare(location)) {
		if (checkIfMovingToOppositeColorPiece(position, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location))) {
			addValidMove(position, moves, pieces, origLocation, location, checkIfPromotion(origLocation, location), isActualMove);
		}
	}
}
void PositionController::getValidKnightMovesInDirections(Location const origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, std::vector< std::vector<Direction>> directionsVector, bool isActualMove)
{
	std::for_each(directionsVector.begin(), directionsVector.end(),
		[origLocation, &position, &pieces, &moves, isActualMove, this](std::vector<Direction> directions) {
		bool continueSearch = false;
		Location location = origLocation;
		int const longDirectionPos = 0;
		int const shortDirectionPos = 1;
		moveLocation(location, directions.at(longDirectionPos));
		moveLocation(location, directions.at(longDirectionPos));
		moveLocation(location, directions.at(shortDirectionPos));
		checkIfValidMove(location, origLocation, position, pieces, moves, continueSearch, isActualMove);
	});
}
		
void PositionController::getValidKingMovesInDirections(Location const origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, std::vector<Direction> directions, bool isActualMove)
{
	std::for_each(directions.begin(), directions.end(),
		[origLocation, &position, &pieces, &moves, isActualMove, this](Direction direction) {
		bool continueSearch{ false };
		Location location = origLocation;
		moveLocation(location, direction);
		checkIfValidMove(location, origLocation, position, pieces, moves, continueSearch, isActualMove);
	});
}void PositionController::getValidKingMovesCastling(Location const origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, int const pieceColor, bool canCastleKingSide, bool canCastleQueenSide, bool isActualMove)
{
	Location kingRookLocation{};
	Location queenRookLocation{};
	MoveType moveType{};
	if (pieceColor == white) {
		kingRookLocation =	{ 0, squaresInARow-1 };
		queenRookLocation = { 0, 0 };
	}
	else {
		kingRookLocation = { squaresInARow - 1, squaresInARow - 1 };
		queenRookLocation = { squaresInARow - 1, 0 };
	}
	if (canCastleKingSide) {

		if (pieceColor == white) {
			moveType = MoveType::castleKingsideWhite;
		}
		else if (pieceColor == black) {
			moveType = MoveType::castleKingsideBlack;
		}
		Location location = origLocation;
		moveLocation(location, Direction::right);
		moveLocation(location, Direction::right);
		addValidMove(position, moves, pieces, origLocation, location, moveType, isActualMove);
	}
	if (canCastleQueenSide) {
		if (pieceColor == white) {
			moveType = MoveType::castleQueensideWhite;
		}
		else if (pieceColor == black) {
			moveType = MoveType::castleQueensideBlack;
		}
		Location location = origLocation;
		moveLocation(location, Direction::left);
		moveLocation(location, Direction::left);
		addValidMove(position, moves, pieces, origLocation, location, moveType, isActualMove);
	}
}
void PositionController::getValidMovesInDirections(Location const origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, std::vector<Direction> directions, bool isActualMove)
{
	std::for_each(directions.begin(), directions.end(),
		[origLocation,&position, &pieces, &moves, isActualMove, this](Direction direction) {
		getValidMovesInDirection(origLocation, position, pieces, moves, direction, isActualMove);
	});
}
void PositionController::getValidMovesInDirection(Location const origLocation, Position & position, std::vector<Piece*> &pieces, std::vector<Move> & moves, Direction direction, bool isActualMove)
{
	bool continueSearch{ true };
	Location location = origLocation;
	while (continueSearch) {
		moveLocation(location, direction);
		checkIfValidMove(location, origLocation, position, pieces, moves, continueSearch, isActualMove);
	}
}
void PositionController::checkIfValidMove(Location const & newLocation, Location const & origLocation, Position & position, std::vector<Piece*> pieces, std::vector<Move> & moves, bool & continueSearch, bool isActualMove)
{
	if (checkIfSquare(newLocation)) {
		if (checkIfMovingToOwnColorPiece(position, origLocation, newLocation)) {
			continueSearch = false;
		}
		else if (checkIfMovingToOppositeColorPiece(position, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation))) {
			continueSearch = false;
			addValidMove(position, moves, pieces, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation), MoveType::capture, isActualMove);
		}
		else {
			addValidMove(position, moves, pieces, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation), isActualMove);
		}
	}
	else {
		continueSearch = false;
	}

}
bool PositionController::checkIfKingAttackedAfterMove(Position &position, std::vector<Piece*> &pieces, int const inactiveColor, Move move)
{
	PositionAnalyzer positionAnalyzer;
	Position positionAfterMove = position;
	positionAfterMove.insertNewMove(move);

	std::vector<Piece*> origPieceVector = pieces;
	std::vector<Piece*> piecesAfterMove = pieces;
	PieceView pieceView;
	PieceType const pieceType{ positionAnalyzer.getPieceTypeFromSquareID(pieces, move.fromSquareId) };
	Piece* piece =  pieceView.generatePiece(pieceType, { 0,0 }, move.toSquareId, NULL);
	piece->hide();

	removePiece(piecesAfterMove, move.toSquareId);
	removePiece(piecesAfterMove, move.fromSquareId);
	piecesAfterMove.push_back(piece);
	//qDebug() << "CKAAM: from: " << move.fromSquareId << " to: " << move.toSquareId << " type: " << pieceType;
	bool retVal{ checkIfKingAttacked(positionAfterMove, piecesAfterMove, inactiveColor) };

	delete piece;
	//piecesAfterMove.clear();
	pieces = origPieceVector;
	for (auto pieceToShow : pieces) {
		pieceToShow->show();
	}

	return retVal;
}
void PositionController::checkIfListOfSquaresAttackedAfterMove(Position &position, Move move, std::vector<int> squares)
{

}



void PositionController::getValidMovesForRook(Position & position, std::vector<Piece*> &pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove)
{
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidMovesInDirections(origLocation, position, pieces, moves,
		{ Direction::left, Direction::right, Direction::up, Direction::down}, isActualMove);
}

void PositionController::getValidMovesForBishop(Position & position, std::vector<Piece*> &pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove)
{
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidMovesInDirections(origLocation, position, pieces, moves,
		{	Direction::diagonalLeftUp,	Direction::diagonalLeftDown,
			Direction::diagonalRightUp, Direction::diagonalRightDown }, isActualMove);
}

void PositionController::getValidMovesForKnight(Position & position, std::vector<Piece*> &pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove)
{
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidKnightMovesInDirections(origLocation, position, pieces, moves,
		{
			{Direction::up,		Direction::left },
			{Direction::up,		Direction::right },
			{Direction::down,	Direction::left },
			{Direction::down,	Direction::right },
			{Direction::left,	Direction::up },
			{Direction::left,	Direction::down },
			{Direction::right,	Direction::up },
			{Direction::right,	Direction::down },
		}, isActualMove);
}

void PositionController::getValidMovesForQueen(Position & position, std::vector<Piece*> &pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove)
{
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidMovesInDirections(origLocation, position, pieces, moves,
		{	Direction::left,			Direction::right,
			Direction::up,				Direction::down,
			Direction::diagonalLeftUp,	Direction::diagonalLeftDown,
			Direction::diagonalRightUp, Direction::diagonalRightDown }, isActualMove);
}

void PositionController::getValidMovesForKing(Position & position, std::vector<Piece*> &pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove)
{
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidKingMovesInDirections(origLocation, position, pieces, moves,
		{	Direction::left,			Direction::right,
			Direction::up,				Direction::down,
			Direction::diagonalLeftUp,	Direction::diagonalLeftDown,
			Direction::diagonalRightUp, Direction::diagonalRightDown }, isActualMove);

	// check if possible to castle
	PieceType const pieceType = piece->getPieceType();
	bool castlingPossibleKingSide{};
	bool castlingPossibleQueenSide{};
	int pieceColor{};
	switch (pieceType)
	{
		case PieceType::whiteKing:
			castlingPossibleKingSide = position.getCastleLegalWhiteKingside();
			if (checkIfPiecesInSquares(position, whiteKingsideCastleSquares)) {
				castlingPossibleKingSide = false;
			}
			castlingPossibleQueenSide = position.getCastleLegalWhiteQueenside();
			if (checkIfPiecesInSquares(position, whiteQueensideCastleSquares)) {
				castlingPossibleQueenSide = false;
			}
			pieceColor = white;
			break;
		case PieceType::blackKing:
			castlingPossibleKingSide = position.getCastleLegalBlackKingside();
			if (checkIfPiecesInSquares(position, blackKingsideCastleSquares)) {
				castlingPossibleKingSide = false;
			}
			castlingPossibleQueenSide = position.getCastleLegalBlackQueenside();
			if (checkIfPiecesInSquares(position, blackQueensideCastleSquares)) {
				castlingPossibleQueenSide = false;
			}
			pieceColor = black;
			break;
		default:
			break;
	}
	if (!position.getActiveKingAttacked()) {
		getValidKingMovesCastling(origLocation, position, pieces, moves, pieceColor, castlingPossibleKingSide, castlingPossibleQueenSide, isActualMove);
	}
}

void PositionController::getMoveString(Position &origPosition, Move &move, PieceType const pieceType)
{
	QString origSquareString{};
	QString newSquareString{};
	getSquareString(move.fromSquareId, origSquareString);
	getSquareString(move.toSquareId, newSquareString);

	move.moveString.append(getPieceChar(pieceType));

	if (checkIfMovingToOppositeColorPiece(origPosition, move.fromSquareId, move.toSquareId)) {
		if (pieceType == PieceType::blackPawn || pieceType == PieceType::whitePawn) {
			move.moveString.append(origSquareString.at(0));
		}
		move.moveString.append('x');
	}

	move.moveString.append(newSquareString);
}

void PositionController::getSquareString(int const &squareID, QString &squareString)
{
	std::vector<QChar> boardLetters{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	int const origRow = squaresInARow - (squareID / squaresInARow);
	QChar const origCol = boardLetters.at(squareID % squaresInARow);
	squareString.append(origCol);
	squareString.append(QString::number(origRow));
}

QChar PositionController::getPieceChar(PieceType const pieceType)
{
	QChar pieceChar{};
	auto pieceCharSearch{ pieceChars2.find(pieceType) };
	if (pieceCharSearch != pieceChars2.end()) {
		pieceChar = pieceCharSearch->second;
	}
	return pieceChar;
}

bool PositionController::checkIfSquare(Location location)
{
	if (location.row >= 0 && location.row < squaresInARow &&
		location.col >= 0 && location.col < squaresInARow) {
		return true;
	}
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

void PositionController::addValidMove(Position &position, std::vector<Move>& moves, std::vector<Piece*> &pieces, int const fromSquareID, int const toSquareID, MoveType moveType, bool isActualMove)
{
	int error{};
	if (fromSquareID < 0 || fromSquareID>63 || toSquareID < 0 || toSquareID>63) {
		error++;
	}
	QString moveString{};
	switch (moveType) {
		case MoveType::castleKingsideWhite:
		case MoveType::castleKingsideBlack:
			moveString.append("0-0");
			break;
		case MoveType::castleQueensideWhite:
		case MoveType::castleQueensideBlack:
			moveString.append("0-0-0");
			break;
		default: break;
	}

	Move move{ fromSquareID,toSquareID, moveString, moveType };

	int inactiveColor{ position.getActiveColorInt() == white ? black : white };
	
	bool moveIllegal{};
	if (isActualMove) {
		moveIllegal = checkIfKingAttackedAfterMove(position, pieces, inactiveColor, move);
	}
	else {
		moveIllegal = false;
	}
	if (!moveIllegal) {
		moves.push_back(move);
	}
}

void PositionController::addValidMove(Position &position, std::vector<Move>& moves, std::vector<Piece*> &pieces, int const fromSquareID, int const toSquareID, bool isActualMove)
{
	addValidMove(position, moves, pieces, fromSquareID, toSquareID, MoveType::normal, isActualMove);
}
void PositionController::addValidMove(Position &position, std::vector<Move>& moves, std::vector<Piece*> &pieces, Location const & origLocation, Location const & location, bool isActualMove)
{
	addValidMove(position, moves, pieces, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location), MoveType::normal, isActualMove);
}
void PositionController::addValidMove(Position &position, std::vector<Move>& moves, std::vector<Piece*> &pieces, Location const & origLocation, Location const & location, MoveType moveType, bool isActualMove)
{
	addValidMove(position, moves, pieces, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location), moveType, isActualMove);
}

void PositionController::removePiece(std::vector<Piece*>& t_pieces, int squareID)
{
	auto it = std::find_if(t_pieces.begin(), t_pieces.end(),
		[&squareID](Piece *piece) {return (piece->getSquareID() == squareID); });
	if (it != t_pieces.end())
	{
		if (*it)
		{
			Piece* pieceToErase = static_cast<Piece*>(*it);
			pieceToErase->hide();
			t_pieces.erase(it);
			//delete pieceToErase;
		}
	}
}

MoveType PositionController::checkIfPromotion(Location origLocation, Location newLocation)
{
	if (newLocation.row == 0 || newLocation.row == squaresInARow) {
		if (newLocation.col != origLocation.col)
		{
			return MoveType::capture;
		}
		else {
			return MoveType::promotion;
		}
	}
	else {
		return MoveType::pawnMove;
	}
}




