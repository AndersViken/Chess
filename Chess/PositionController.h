#pragma once
#include "Move.h"
#include "Position.h"
#include "Piece.h"

struct specialMove
{
	int square;
	int pieceOnSquareAfterMove;
	QString moveString;
};


enum class Direction { left, right, up, down, diagonalLeftUp, diagonalRightUp, diagonalLeftDown, diagonalRightDown};

struct Location {
	int row;
	int col;
};

class PositionController
{
public:
	PositionController();
	~PositionController();

	Position generateNewPosition(Move &mov, Position &oldPos);
	void updateFenString(Position &position);
	bool validateMove(Position & newPosition, Position & oldPosition, Move move, int pieceType, std::vector<specialMove> &specialMoves);
	//QChar getFenCharFromSquareID(QString fenString, int const squareID);
	bool validateBishopMove(Move const &move);
	bool validateRookMove(Move const &move);
	bool validateKnightMove(Move const &move);
	bool validateQueenMove(Move const &move);
	bool validateKingMove(Move const & move, Position &oldPosition, Position &newPosition, int pieceType, std::vector<specialMove> &specialMoves);
	void moveRookWhenCastling(Position & newPosition, std::vector<specialMove>& specialMoves, int const rookOrigSquare, int const rookNewSquare, int const rookType, QString castleString);
	bool validatePawnMove(Position &oldPosition, Move const &move, Position &newPosition, int const piecetype);
	bool checkIfMovingToOwnColorPiece(Position &oldPosition, Move const &move, Position &newPosition);
	bool checkIfMovingToOppositeColorPiece(Position & position, int const newSquareID);
	bool checkIfMovingToOppositeColorPiece(Position & position, Location newLocation);
	bool checkIfMovingToPiece(Position & oldPosition, int const newSquareID);
	bool checkIfMovingToPiece(Position & oldPosition, Location newLocation);
	bool checkIfPiecesInSquares(Position const position, std::vector<int> const squares);
	int rowsMoved(Move move);
	int rowsMovedWithSign(Move move);
	int colsMoved(Move move);
	int colsMovedWithSign(Move move);
	int getColorFromType(int pieceType, int const returnValueIfNotFound);

	std::vector<Move> getValidMoves(Position & position, std::vector<Piece*>& pieces);
	void getValidMovesForPiece(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForPawn(Position & position, Piece *& piece, std::vector<Move> &moves);
	void checkIfValidMovePawnForward(const Location &location, Position & position, std::vector<Move> & moves, Piece *& piece);
	void checkIfValidMovePawnDiagonal(Location const & location, Position & position, std::vector<Move>& moves, Piece *& piece);
	void getValidMovesForRook(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForBishop(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForKnight(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForQueen(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForKing(Position & position, Piece *& piece, std::vector<Move> &moves);

	bool checkIfSquare(Location location);
	Location getLocationFromSquareID(int const squareID);
	int getSquareIDFromLocation(Location location);
	void moveLocation(Location& location, Direction const direction);
	void addValidMove(std::vector<Move>& moves, int const fromSquareID, int const toSquareID);

};

