#include "Piece.h"
#include "chessTypes.h"

Piece::Piece()
{
}

Piece::Piece(QPoint coordinate, int pieceType, int color, QString imagePath, int imageSize, QWidget *parent) :
	coordinate(coordinate),
	pieceType(pieceType),
	color(color),
	imagePath(imagePath),
	imageSize(imageSize),
	QLabel(parent)
{
	setGeometry(QRect(coordinate.rx(), coordinate.ry(), imageSize, imageSize));
	setTabletTracking(true);
	QPixmap image(imagePath);
	setPixmap(image.scaled(width(), height(), Qt::KeepAspectRatio));
}

Piece::~Piece()
{
}

QPoint Piece::getCoordinate()
{
	return coordinate;
}

int Piece::getPieceType()
{
	return pieceType;
}

int Piece::getColor()
{
	return color;
}

QString Piece::getImagePath()
{
	return imagePath;
}

int Piece::getImageSize()
{
	return imageSize;
}


