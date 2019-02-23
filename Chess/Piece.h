#pragma once
#include <qlabel.h>
#include <ChessTypes.h>
class Piece :
	public QLabel
{
public:
	Piece();
	Piece(QPoint coordinate, int pieceType, int color, QString imagePath, int imageSize, QWidget *parent);
	~Piece();

	QPoint	getCoordinate();
	int			getPieceType();
	int			getColor();
	QString		getImagePath();
	int			getImageSize();

private:
	QPoint coordinate{};
	int pieceType{};
	int color{};
	QString imagePath{};
	int imageSize{};
};

