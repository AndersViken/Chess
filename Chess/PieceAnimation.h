#pragma once

#include "Piece.h"

class PieceAnimation
{
public:
	PieceAnimation();
	~PieceAnimation();

	void movePiece(Piece* piece, QPoint newCoordinate);

private:
	int nMillisecondsToMovePiece = 5000;
};

