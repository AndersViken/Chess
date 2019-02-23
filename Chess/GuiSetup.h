#pragma once
int const minWinSizeLeft	{ 840 };
int const minWinSizeTop		{ 840 };
int const squaresInARow		{ 8 };
int const maxNumOfPieces	{ 32 };
int const squarePixelSize	{ 80 };
int const imageSize			{ squarePixelSize };
int const lightColor		{ 0 };
int const darkColor			{ 1 };
int const boardWidth		{ squaresInARow *squarePixelSize };
int const boardHeigth		{ boardWidth };
int const boardStartTop		{ 115 };
int const boardStartLeft	{ 100 };
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
enum { imageOffsetLeft = 0, imageOffsetTop = 0 };
QString const darkColorString	{ "background - color: rgb(181, 135, 99);"	};
QString const lightColorString	{ "background - color: rgb(240, 218, 181);" };

QString imagePath = ":/Images/";
std::map<int, QString> imagePaths = {
	{ whitePawn,    imagePath + "whitePawn"     },
	{ whiteRook,    imagePath + "whiteRook"     },
	{ whiteKnight,  imagePath + "whiteKnight"   },
	{ whiteBishop,  imagePath + "whiteBishop"	},
	{ whiteQueen,   imagePath + "whiteQueen"    },
	{ whiteKing,    imagePath + "whiteKing"     },
	{ blackPawn,    imagePath + "blackPawn"     },
	{ blackRook,    imagePath + "blackRook"     },
	{ blackKnight,  imagePath + "blackKnight"   },
	{ blackBishop,  imagePath + "blackBishop"   },
	{ blackQueen,   imagePath + "blackQueen"    },
	{ blackKing,    imagePath + "blackKing"     }
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