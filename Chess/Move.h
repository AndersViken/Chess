#pragma once
#include <qstring.h>

enum class MoveType {
	normal,
	castleKingsideWhite,
	castleQueensideWhite,
	castleKingsideBlack,
	castleQueensideBlack,
	enPassant,
	promotion,
	pawnMove,
	capture
};

class Move
{
public:
	Move();
	Move(int fromSquareId, int toSquareId);
	Move(int fromSquareId, int toSquareId, QString moveString);
	Move(int fromSquareId, int toSquareId, QString moveString, MoveType moveType);
	~Move();
	bool operator==(const Move & lhs);
	int fromSquareId;
	int toSquareId;
	QString moveString;
	MoveType moveType;

};


