#pragma once
#include <qstring.h>
#include "PieceInfo.h"

enum class MoveType {
	normal,
	castleKingsideWhite,
	castleQueensideWhite,
	castleKingsideBlack,
	castleQueensideBlack,
	enPassant,
	promotion,
	pawnMove,
	capture,
	captureAndPromotion
};

class Move
{
public:
	
	Move::Move(){}

	Move::Move(int fromSquareId, int toSquareId) :
		fromSquareId(fromSquareId),
		toSquareId(toSquareId),
		moveString(""),
		moveType(MoveType::normal){}

	Move::Move(int fromSquareId, int toSquareId, QString moveString) :
		fromSquareId(fromSquareId),
		toSquareId(toSquareId),
		moveString(moveString),
		moveType(MoveType::normal){}

	Move::Move(int fromSquareId, int toSquareId, QString moveString, MoveType moveType, PieceType promotionToPieceType) :
		fromSquareId(fromSquareId),
		toSquareId(toSquareId),
		moveString(moveString),
		moveType(moveType),
		promotionToPieceType(promotionToPieceType){}
	
	Move::~Move() {}

	bool operator==(const Move & lhs);
	Move& operator=(const Move& other);
	int fromSquareId;
	int toSquareId;
	QString moveString;
	MoveType moveType;
	PieceType promotionToPieceType;
};


