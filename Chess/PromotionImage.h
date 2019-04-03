#pragma once
#include <qlabel.h>

class PromotionImage :
	public QLabel
{
public:
	PromotionImage();
	~PromotionImage();
	int getPieceType() { return pieceType; };
	void setPieceType(int t_pieceType) { pieceType = t_pieceType; };
private:
	int pieceType;
};



