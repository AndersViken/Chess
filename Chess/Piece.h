#pragma once
#include <qlabel.h>
#include "PieceInfo.h"
class Piece :
	public QLabel
{
public:
	Piece();
	Piece(QPoint coordinate, int squareID, PieceType pieceType, int color, QString imagePath, int imageSize, QWidget *parent);
	~Piece();

	QPoint	getCoordinate();
	int		getSquareID();
	PieceType		getPieceType();
	void	setPieceType(PieceType t_pieceType) { pieceType = t_pieceType; };
	int		getColor();
	QString	getImagePath();
	int		getImageSize();
	void	makeActive();
	void	makeDisabled();

private:
	QPoint coordinate{};
	int squareID{};
	PieceType pieceType{};
	int color{};
	QString imagePath{};
	int imageSize{};
};

