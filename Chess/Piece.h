#pragma once
#include <qlabel.h>
class Piece :
	public QLabel
{
public:
	Piece();
	Piece(QPoint coordinate, int squareID, int pieceType, int color, QString imagePath, int imageSize, QWidget *parent);
	~Piece();

	QPoint	getCoordinate();
	int		getSquareID();
	int		getPieceType();
	int		getColor();
	QString	getImagePath();
	int		getImageSize();
	void	makeActive();
	void	makeDisabled();

private:
	QPoint coordinate{};
	int squareID{};
	int pieceType{};
	int color{};
	QString imagePath{};
	int imageSize{};
};

