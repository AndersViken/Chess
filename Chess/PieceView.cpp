#include "PieceView.h"
#include "PieceInfo.h"
#include "GuiSetup.h"
#include "qdebug.h"

std::map<int, int> const pieceColors = {
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


PieceView::PieceView()
{
}


PieceView::~PieceView()
{
}

Piece* PieceView::generatePiece(int const pieceType, QPoint coordinate, int squareID, QWidget *parent)
{

	auto colorSearch = pieceColors.find(pieceType);
	int color{};
	if (colorSearch != pieceColors.end()) {
		color = colorSearch->second;
	}
	else {
		qDebug() << "generatePiece(): no color found.";
		return{};
	}

	auto imageSearch{ imagePaths.find(pieceType) };
	if (imageSearch != imagePaths.end()) {
		QString imagePath = imageSearch->second;
		int tempImageSize = imageSize;
		Piece *piece{ new Piece(coordinate, squareID, pieceType, color, imagePath , tempImageSize, parent) };
		//piece->setAttribute(Qt::WA_DeleteOnClose);
		return piece;
	}
	return {};
}
