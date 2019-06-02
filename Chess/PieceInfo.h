#pragma once
#include <qstring.h>
#include <qlabel.h>

enum class PieceType {
	empty = 0, noChange, whitePawn, whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing,
	blackPawn, blackRook, blackKnight, blackBishop, blackQueen, blackKing
};
enum { white = 0, black = 1 };

enum {whiteColorChar = 'w', blackColorChar='b' };


