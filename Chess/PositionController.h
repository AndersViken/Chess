#pragma once
#include "Move.h"
#include "Position.h"
#include <cmath>
class PositionController
{
public:
	PositionController();
	~PositionController();

	Position generateNewPosition(Move &mov, Position &oldPos);
	void updateFenStringForNewPosition(Position &position);
	//QChar getFenCharFromSquareID(QString fenString, int const squareID);
	bool validateMove(Position newPosition, Position oldPosition, Move move, int pieceType);
	bool validateBishopMove(Move const &move);
	bool validateRookMove(Move const &move);
	bool validateKnightMove(Move const &move);
	bool validateQueenMove(Move const &move);
	bool validateKingMove(Move const &move);
	bool validatePawnMove(Position &oldPosition, Move const &move, Position &newPosition, int const piecetype);
	bool checkIfMovingToOwnColorPiece(Position &oldPosition, Move const &move, Position &newPosition);
	bool checkIfMovingToOppositeColorPiece(Position & oldPosition, Move const &move, Position &newPosition);
	bool checkIfMovingToPiece(Position & oldPosition, Move const &move);
	int rowsMoved(Move move);
	int rowsMovedWithSign(Move move);
	int colsMoved(Move move);
	int getColorFromType(int pieceType, int const returnValueIfNotFound);

};

