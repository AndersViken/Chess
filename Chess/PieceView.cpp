#include "PieceView.h"
#include "PieceInfo.h"
#include "GuiSetup.h"
#include "qdebug.h"

std::map<PieceType, int> const pieceColors = {
	{ PieceType::whitePawn,    white },
	{ PieceType::whiteRook,    white },
	{ PieceType::whiteKnight,  white },
	{ PieceType::whiteBishop,  white },
	{ PieceType::whiteQueen,   white },
	{ PieceType::whiteKing,    white },
	{ PieceType::blackPawn,    black },
	{ PieceType::blackRook,    black },
	{ PieceType::blackKnight,  black },
	{ PieceType::blackBishop,  black },
	{ PieceType::blackQueen,   black },
	{ PieceType::blackKing,    black }
};

QString imageDir = ":/Images/";
std::map<PieceType, QString> imagePaths = {
	{ PieceType::whitePawn,    imageDir + "whitePawn"     },
	{ PieceType::whiteRook,    imageDir + "whiteRook"     },
	{ PieceType::whiteKnight,  imageDir + "whiteKnight"   },
	{ PieceType::whiteBishop,  imageDir + "whiteBishop"	},
	{ PieceType::whiteQueen,   imageDir + "whiteQueen"    },
	{ PieceType::whiteKing,    imageDir + "whiteKing"     },
	{ PieceType::blackPawn,    imageDir + "blackPawn"     },
	{ PieceType::blackRook,    imageDir + "blackRook"     },
	{ PieceType::blackKnight,  imageDir + "blackKnight"   },
	{ PieceType::blackBishop,  imageDir + "blackBishop"   },
	{ PieceType::blackQueen,   imageDir + "blackQueen"    },
	{ PieceType::blackKing,    imageDir + "blackKing"     }
};


PieceView::PieceView()
{
}


PieceView::~PieceView()
{
}

Piece* PieceView::generatePiece(PieceType const pieceType, QPoint coordinate, int squareID, QWidget *parent)
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
