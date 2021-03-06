#include "Piece.h"


Piece::Piece()
{
}

Piece::Piece(QPoint coordinate, int squareID, PieceType pieceType, int color, QString imagePath, int imageSize, QWidget *parent) :
	coordinate(coordinate),
	squareID(squareID),
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

int Piece::getSquareID()
{
	return squareID;
}

PieceType Piece::getPieceType()
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

void Piece::makeActive()
{
	setTabletTracking(true);
}

void Piece::makeDisabled()
{
	setTabletTracking(false);
}


