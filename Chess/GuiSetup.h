#pragma once
#include <map>
#include <vector>
int const minWinSizeX		{ 840 };
int const minWinSizeY		{ 840 };
int const squaresInARow		{ 8 };
int const maxNumOfPieces	{ 32 };
int const squarePixelSize	{ 80 };
int const imageSize			{ squarePixelSize };
int const lightColor		{ 0 };
int const darkColor			{ 1 };
int const boardWidth		{ squaresInARow *squarePixelSize };
int const boardHeigth		{ boardWidth };
int const boardStartX		{ 100 };
int const boardStartY		{ 115 };


int const coordinateOffsetColTop	{ squarePixelSize / 2 - squarePixelSize / 8 };
int const coordinateOffsetColLeft	{ squarePixelSize - squarePixelSize / 7 };
int const coordinateOffsetRowTop	{ squarePixelSize / 8 - squarePixelSize / 2 };
int const coordinateOffsetRowLeft	{ squarePixelSize / 15 };
int const coordinateOffsetBoarder	{ squarePixelSize / 40 };

QString const darkColorString	{ "background - color: rgb(181, 135, 99);"	};
QString const lightColorString	{ "background - color: rgb(240, 218, 181);" };

