#pragma once

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
enum { white = 0, black = 1 };
enum {
	whitePawn = 0, whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing,
	blackPawn, blackRook, blackKnight, blackBishop, blackQueen, blackKing
};
int const coordinateOffsetColTop	{ squarePixelSize / 2 - squarePixelSize / 8 };
int const coordinateOffsetColLeft	{ squarePixelSize - squarePixelSize / 7 };
int const coordinateOffsetRowTop	{ squarePixelSize / 8 - squarePixelSize / 2 };
int const coordinateOffsetRowLeft	{ squarePixelSize / 15 };
int const coordinateOffsetBoarder	{ squarePixelSize / 40 };

QString const darkColorString	{ "background - color: rgb(181, 135, 99);"	};
QString const lightColorString	{ "background - color: rgb(240, 218, 181);" };

QString imageDir = ":/Images/";
std::map<int, QString> imagePaths = {
	{ whitePawn,    imageDir + "whitePawn"     },
	{ whiteRook,    imageDir + "whiteRook"     },
	{ whiteKnight,  imageDir + "whiteKnight"   },
	{ whiteBishop,  imageDir + "whiteBishop"	},
	{ whiteQueen,   imageDir + "whiteQueen"    },
	{ whiteKing,    imageDir + "whiteKing"     },
	{ blackPawn,    imageDir + "blackPawn"     },
	{ blackRook,    imageDir + "blackRook"     },
	{ blackKnight,  imageDir + "blackKnight"   },
	{ blackBishop,  imageDir + "blackBishop"   },
	{ blackQueen,   imageDir + "blackQueen"    },
	{ blackKing,    imageDir + "blackKing"     }
};
std::map<int, int> pieceColors = {
	{ whitePawn,    white },
	{ whiteRook,    white },
	{ whiteKnight,  white },
	{ whiteBishop,  white },
	{ whiteQueen,   white },
	{ whiteKing,    white },
	{ blackPawn,    black },
	{ blackRook,    black },
	{ blackKnight,  black },
	{ blackBishop,  black },
	{ blackQueen,   black },
	{ blackKing,    black }
};
std::vector<int> pieceTypes = {
	whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing, whiteBishop, whiteKnight, whiteRook,
	whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn,
	blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn,
	blackRook, blackKnight, blackBishop, blackQueen, blackKing, blackBishop, blackKnight, blackRook
};