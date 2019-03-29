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

std::map<int, QChar> pieceChars2 = {
	{ whiteRook,    'R' },
	{ whiteKnight,  'N' },
	{ whiteBishop,  'B'	},
	{ whiteQueen,   'Q' },
	{ whiteKing,    'K' },
	{ blackRook,    'R' },
	{ blackKnight,  'N' },
	{ blackBishop,  'B' },
	{ blackQueen,   'Q' },
	{ blackKing,    'K' }
};

int const lowestSquareID = 0;
int const highestSquareID = 63;

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

bool PositionController::validateMove(Position &position, std::vector<Piece*> pieces, Move& move)
{
	std::vector<Move> validMoves = getValidMoves(position, pieces);

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
		if (oldPosition.getCastleLegalBlackKingside() && colsMovedWithSign(move) == -2) {
			if (checkIfPiecesInSquares(oldPosition, { 5,6 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 7, 5, blackRook, kingsideCastleString);
			return true;
		}
		if (oldPosition.getCastleLegalBlackQueenside() && colsMovedWithSign(move) == 2) {
			if (checkIfPiecesInSquares(oldPosition, { 1,2,3 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 0, 3, blackRook, queensideCastleString);
			return true;
		}
	}
	if (pieceType == whiteKing) {
		if (oldPosition.getCastleLegalWhiteKingside() && colsMovedWithSign(move) == -2) {
			if (checkIfPiecesInSquares(oldPosition, { 61,62 })) {
				return false;
			}
			moveRookWhenCastling(newPosition, specialMoves, 63, 61, whiteRook, kingsideCastleString);
			return true;
		}
		if (oldPosition.getCastleLegalWhiteQueenside() && colsMovedWithSign(move) == 2) {
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
		if (piece->getColor() == activeColor) {
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
	checkIfValidMovePawnForward(location, origLocation, position, moves);
	location = origLocation;

	if (location.row == startRow) {
		moveLocation(location, forward);
		moveLocation(location, forward);
		checkIfValidMovePawnForward(location, origLocation, position, moves);
		location = origLocation;
	}

	moveLocation(location, diagonalForwardLeft);
	checkIfValidMovePawnDiagonal(location, origLocation, position, moves);
	location = origLocation;

	moveLocation(location, diagonalForwardRight);
	checkIfValidMovePawnDiagonal(location, origLocation, position, moves);
	location = origLocation;

}

void PositionController::checkIfValidMovePawnForward(Location const & location, Location const & origLocation, Position & position, std::vector<Move> & moves)
{
	if (checkIfSquare(location)) {
		if (checkIfMovingToPiece(position, location) == false) {
			addValidMove(moves, origLocation, location, MoveType::pawnMove);
		}
	}
}
void PositionController::checkIfValidMovePawnDiagonal(Location const & location, Location const & origLocation, Position & position, std::vector<Move> & moves)
{
	if (checkIfSquare(location)) {
		if (checkIfMovingToOppositeColorPiece(position, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location))) {
			addValidMove(moves, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location), MoveType::capture);
		}
	}
}
void PositionController::getValidKnightMovesInDirections(Location const origLocation, Position & position, std::vector<Move> & moves, std::vector< std::vector<Direction>> directionsVector)
{
	std::for_each(directionsVector.begin(), directionsVector.end(), [origLocation, &position, &moves, this](std::vector<Direction> directions) {
		bool continueSearch = false;
		Location location = origLocation;
		int const longDirectionPos = 0;
		int const shortDirectionPos = 1;
		moveLocation(location, directions.at(longDirectionPos));
		moveLocation(location, directions.at(longDirectionPos));
		moveLocation(location, directions.at(shortDirectionPos));
		checkIfValidMove(location, origLocation, position, moves, continueSearch);
	});
}
		
void PositionController::getValidKingMovesInDirections(Location const origLocation, Position & position, std::vector<Move> & moves, std::vector<Direction> directions)
{
	std::for_each(directions.begin(), directions.end(), [origLocation, &position, &moves, this](Direction direction) {
		bool continueSearch{ false };
		Location location = origLocation;
		moveLocation(location, direction);
		checkIfValidMove(location, origLocation, position, moves, continueSearch);
	});
}void PositionController::getValidKingMovesCastling(Location const origLocation, std::vector<Move> & moves, int const pieceColor, bool canCastleKingSide, bool canCastleQueenSide)
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
		addValidMove(moves, origLocation, location, moveType);
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
		addValidMove(moves, origLocation, location, moveType);
	}
}
void PositionController::getValidMovesInDirections(Location const origLocation, Position & position, std::vector<Move> & moves, std::vector<Direction> directions)
{
	std::for_each(directions.begin(), directions.end(), [origLocation,&position, &moves, this](Direction direction) {
		getValidMovesInDirection(origLocation, position, moves, direction);
	});
}
void PositionController::getValidMovesInDirection(Location const origLocation, Position & position, std::vector<Move> & moves, Direction direction)
{
	bool continueSearch{ true };
	Location location = origLocation;
	while (continueSearch) {
		moveLocation(location, direction);
		checkIfValidMove(location, origLocation, position, moves, continueSearch);
	}
}
void PositionController::checkIfValidMove(Location const & newLocation, Location const & origLocation, Position & position, std::vector<Move> & moves, bool & continueSearch)
{
	/* backup before changing if else order
	if (checkIfSquare(newLocation)) {
		if (checkIfMovingToOwnColorPiece(position, origLocation, newLocation)) {
			continueSearch = false;
		}
		else {
			addValidMove(moves, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation));
		}
		if (checkIfMovingToOppositeColorPiece(position, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation))) {
			continueSearch = false;
		}
	}
	else {
		continueSearch = false;
	}
	*/
	if (checkIfSquare(newLocation)) {
		if (checkIfMovingToOwnColorPiece(position, origLocation, newLocation)) {
			continueSearch = false;
		}
		else if (checkIfMovingToOppositeColorPiece(position, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation))) {
			continueSearch = false;
			addValidMove(moves, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation), MoveType::capture);
		}
		else {
			addValidMove(moves, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(newLocation));
		}
	}
	else {
		continueSearch = false;
	}
}

void PositionController::getValidMovesForRook(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	qDebug() << "enter function getValidMovesForRook()\n";
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidMovesInDirections(origLocation, position, moves,
		{ Direction::left, Direction::right, Direction::up, Direction::down});
}

void PositionController::getValidMovesForBishop(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	qDebug() << "enter function getValidMovesForBishop()\n";
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidMovesInDirections(origLocation, position, moves,
		{	Direction::diagonalLeftUp,	Direction::diagonalLeftDown,
			Direction::diagonalRightUp, Direction::diagonalRightDown });
}

void PositionController::getValidMovesForKnight(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidKnightMovesInDirections(origLocation, position, moves,
		{
			{Direction::up,		Direction::left },
			{Direction::up,		Direction::right },
			{Direction::down,	Direction::left },
			{Direction::down,	Direction::right },
			{Direction::left,	Direction::up },
			{Direction::left,	Direction::down },
			{Direction::right,	Direction::up },
			{Direction::right,	Direction::down },
		});
}

void PositionController::getValidMovesForQueen(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	qDebug() << "enter function getValidMovesForQueen()\n";
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidMovesInDirections(origLocation, position, moves,
		{	Direction::left,			Direction::right,
			Direction::up,				Direction::down,
			Direction::diagonalLeftUp,	Direction::diagonalLeftDown,
			Direction::diagonalRightUp, Direction::diagonalRightDown });
}

void PositionController::getValidMovesForKing(Position & position, Piece *& piece, std::vector<Move>& moves)
{
	//moves.push_back(Move{ piece->getSquareID(),piece->getSquareID(),"King" });
	qDebug() << "enter function getValidMovesForKing()\n";
	Location origLocation = getLocationFromSquareID(piece->getSquareID());
	getValidKingMovesInDirections(origLocation, position, moves,
		{	Direction::left,			Direction::right,
			Direction::up,				Direction::down,
			Direction::diagonalLeftUp,	Direction::diagonalLeftDown,
			Direction::diagonalRightUp, Direction::diagonalRightDown });

	// check if possible to castle
	int const pieceType = piece->getPieceType();
	bool castlingPossibleKingSide{};
	bool castlingPossibleQueenSide{};
	int pieceColor{};
	switch (pieceType)
	{
		case whiteKing:
			castlingPossibleKingSide = position.getCastleLegalWhiteKingside();
			castlingPossibleQueenSide = position.getCastleLegalWhiteQueenside();
			pieceColor = white;
			break;
		case blackKing:
			castlingPossibleKingSide = position.getCastleLegalBlackKingside();
			castlingPossibleQueenSide = position.getCastleLegalBlackQueenside();
			pieceColor = black;
			break;
		default:
			break;
	}

	getValidKingMovesCastling(origLocation, moves, pieceColor, castlingPossibleKingSide, castlingPossibleQueenSide);
}

void PositionController::getMoveString(Position &origPosition, Move &move, int const pieceType)
{
	QString origSquareString{};
	QString newSquareString{};
	getSquareString(move.fromSquareId, origSquareString);
	getSquareString(move.toSquareId, newSquareString);

	move.moveString.append(getPieceChar(pieceType));

	if (checkIfMovingToOppositeColorPiece(origPosition, move.fromSquareId, move.toSquareId)) {
		if (pieceType == blackPawn || pieceType == whitePawn) {
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

QChar PositionController::getPieceChar(int const pieceType)
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
	int error{};
	if (fromSquareID < 0 || fromSquareID>63 || toSquareID < 0 || toSquareID>63) {
		error++;
	}
	moves.push_back({ fromSquareID,toSquareID});
}
void PositionController::addValidMove(std::vector<Move>& moves, int const fromSquareID, int const toSquareID, MoveType moveType)
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
	moves.push_back({ fromSquareID,toSquareID, moveString, moveType});
}

void PositionController::addValidMove(std::vector<Move>& moves, Location const & origLocation, Location const & location)
{
	addValidMove(moves, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location));
}
void PositionController::addValidMove(std::vector<Move>& moves, Location const & origLocation, Location const & location, MoveType moveType)
{
	addValidMove(moves, getSquareIDFromLocation(origLocation), getSquareIDFromLocation(location), moveType);
}

//void PositionController::moveLocation(Location location, Direction direction)
//{
//	if (direction == Direction::up) {
//		location.row--;
//	}	
//}



