#pragma once
#include <qlabel.h>
#include <chessTypes.h>
class Piece :
	public QLabel
{
public:
	Piece();
	Piece(Coordinate coordinate, int pieceId, int pieceType, int color, QString imagePath);
	~Piece();

private:
	Coordinate coordinate;
	int pieceId;
	int pieceType;
	int color;
	QString imagePath;
};

