#pragma once
int const squaresInARow		{ 8 };
int const maxNumOfPieces	{ 32 };
int const squarePixelSize	{ 60 };
int const imageSize			{ squarePixelSize };
int const lightColor		{ 0 };
int const darkColor			{ 1 };
enum { white = 0, black = 1 };
enum { boardStartTop = 115, boardStartLeft = 100 };
enum {
	whitePawn = 0, whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing,
	blackPawn, blackRook, blackKnight, blackBishop, blackQueen, blackKing
};
int const coordinateOffsetColTop	{ squarePixelSize / 2 - squarePixelSize / 8 };
int const coordinateOffsetColLeft	{ squarePixelSize - squarePixelSize / 7 };
int const coordinateOffsetRowTop	{ squarePixelSize / 8 - squarePixelSize / 2 };
int const coordinateOffsetRowLeft	{ squarePixelSize / 15 };
int const coordinateOffsetBoarder	{ squarePixelSize / 40 };
enum { imageOffsetLeft = 0, imageOffsetTop = 0 };
QString const darkColorString	{ "background - color: rgb(181, 135, 99);"	};
QString const lightColorString	{ "background - color: rgb(240, 218, 181);" };