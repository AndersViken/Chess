#include "Piece.h"



Piece::Piece()
{
}

Piece::Piece(Coordinate coordinate, int pieceId, int pieceType, int color, QString imagePath) :
	coordinate(coordinate),
	pieceId(pieceId),
	pieceType(pieceType),
	color(color),
	imagePath(imagePath)
{
}


Piece::~Piece()
{
}
