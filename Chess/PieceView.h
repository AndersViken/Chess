#pragma once
#include "Piece.h"

class PieceView
{
public:
	PieceView();
	~PieceView();
	Piece * generatePiece(int const pieceType, QPoint coordinate, int squareID, QWidget * parent);
	
};

