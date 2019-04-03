#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "PromotionImage.h"


class PromotionDialog : public QDialog
{
	Q_OBJECT

public:
	PromotionDialog();
	int getPieceTypeSelected() { return pieceTypeSelected; };
	void setPieceTypeSelected(int t_pieceTypeSelected) { pieceTypeSelected = t_pieceTypeSelected; };

private:
	enum { numPieces = 4 };
	std::vector<PromotionImage*> pieces{};
	void mousePressEvent(QMouseEvent *event) override;
	int pieceTypeSelected;
};

#endif // DIALOG_H