#pragma once
#include <qlabel.h>
#include <chessTypes.h>
class Piece :
	public QLabel
{
public:
	Piece();
	Piece(Coordinate coordinate, int pieceId, int pieceType, int color, QString imagePath, int imageSize, QWidget *parent);
	~Piece();

	Coordinate	getCoordinate();
	int			getPieceId();
	int			getPieceType();
	int			getColor();
	QString		getImagePath();

private:
	Coordinate coordinate;
	int pieceId;
	int pieceType;
	int color;
	QString imagePath;
	int imageSize;
};

