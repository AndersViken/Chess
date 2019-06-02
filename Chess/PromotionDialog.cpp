#include <QtWidgets>
#include <qlabel.h>
#include "qmimedata.h"
#include "PieceInfo.h"
#include "PromotionDialog.h"
#include "GuiSetup.h"

PromotionDialog::PromotionDialog(int const playerColor)
{
	setCursor(Qt::PointingHandCursor);
	QVBoxLayout *mainLayout = new QVBoxLayout;

	std::vector<QString> imageDirs{};
	std::vector<PieceType> pieceTypes{};
	if (playerColor == white) {
		imageDirs.push_back(":/Images/whiteQueen");
		imageDirs.push_back(":/Images/whiteKnight");
		imageDirs.push_back(":/Images/whiteRook");
		imageDirs.push_back(":/Images/whiteBishop");

		pieceTypes.push_back(PieceType::whiteQueen);
		pieceTypes.push_back(PieceType::whiteKnight);
		pieceTypes.push_back(PieceType::whiteRook);
		pieceTypes.push_back(PieceType::whiteBishop);
	}
	else {
		imageDirs.push_back(":/Images/blackQueen");
		imageDirs.push_back(":/Images/blackKnight");
		imageDirs.push_back(":/Images/blackRook");
		imageDirs.push_back(":/Images/blackBishop");

		pieceTypes.push_back(PieceType::blackQueen);
		pieceTypes.push_back(PieceType::blackKnight);
		pieceTypes.push_back(PieceType::blackRook);
		pieceTypes.push_back(PieceType::blackBishop);
	}
	int pieceNum{ 0 };
	std::for_each(imageDirs.begin(), imageDirs.end(), [&mainLayout, &pieceTypes, &pieceNum, this](QString imageDir) {
		PromotionImage* piece = new PromotionImage();
		QPixmap image(imageDir);
		piece->setPixmap(image.scaled(squarePixelSize, squarePixelSize, Qt::KeepAspectRatio));
		piece->setPieceType(pieceTypes.at(pieceNum));
		mainLayout->addWidget(piece);
		pieces.push_back(piece);
		pieceNum++;
	});
	setLayout(mainLayout);

	setWindowTitle(tr(""));
	
}

void PromotionDialog::mousePressEvent(QMouseEvent * event)
{
	PromotionImage *piece{ static_cast<PromotionImage*>(childAt(event->pos())) };
	if (!piece)
		return;

	setPieceTypeSelected(piece->getPieceType());

	delete piece;
	close();
}


