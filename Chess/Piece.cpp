#include "Piece.h"
#include "chessTypes.h"

Piece::Piece()
{
}

Piece::Piece(Coordinate coordinate, int pieceId, int pieceType, int color, QString imagePath, int imageSize, QWidget *parent) :
	coordinate(coordinate),
	pieceId(pieceId),
	pieceType(pieceType),
	color(color),
	imagePath(imagePath),
	QLabel(parent)
{
	setGeometry(QRect(coordinate.xPos, coordinate.yPos, imageSize, imageSize));
	setTabletTracking(true);
	QPixmap image(imagePath);
	setPixmap(image.scaled(width(), height(), Qt::KeepAspectRatio));
}

Piece::~Piece()
{
}

Coordinate Piece::getCoordinate()
{
	return coordinate;
}

int Piece::getPieceId()
{
	return pieceId;
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

