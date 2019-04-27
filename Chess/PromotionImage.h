#pragma once
#include <qlabel.h>
#include "PieceInfo.h"

class PromotionImage :
	public QLabel
{
public:
	PromotionImage();
	~PromotionImage();
	PieceType getPieceType() { return pieceType; };
	void setPieceType(PieceType t_pieceType) { pieceType = t_pieceType; };
private:
	PieceType pieceType;
};



