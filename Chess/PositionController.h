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
	bool checkIfMovingToOppositeColorPiece(Position & oldPosition, Move const &move, Position &newPosition);
	bool checkIfMovingToPiece(Position & oldPosition, Move const &move);
	bool checkIfPiecesInSquares(Position const position, std::vector<int> const squares);
	int rowsMoved(Move move);
	int rowsMovedWithSign(Move move);
	int colsMoved(Move move);
	int colsMovedWithSign(Move move);
	int getColorFromType(int pieceType, int const returnValueIfNotFound);

	std::vector<Move> getValidMoves(Position & position, std::vector<Piece*>& pieces);
	void getValidMovesForPiece(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForPawn(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForRook(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForBishop(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForKnight(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForQueen(Position & position, Piece *& piece, std::vector<Move> &moves);
	void getValidMovesForKing(Position & position, Piece *& piece, std::vector<Move> &moves);

};

