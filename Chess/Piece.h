#pragma once
#include <qlabel.h>
#include <ChessTypes.h>
class Piece :
	public QLabel
{
public:
	Piece();
	Piece(Coordinate coordinate, int pieceType, int color, QString imagePath, int imageSize, QWidget *parent);
	~Piece();

	Coordinate	getCoordinate();
	int			getPieceType();
	int			getColor();
	QString		getImagePath();
	int			getImageSize();

private:
	Coordinate coordinate{};
	int pieceType{};
	int color{};
	QString imagePath{};
	int imageSize{};
};

