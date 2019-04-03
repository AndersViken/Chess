#include <QtWidgets>
#include <qlabel.h>
#include "qmimedata.h"
#include "PieceInfo.h"
#include "PromotionDialog.h"
#include "GuiSetup.h"

PromotionDialog::PromotionDialog()
{
	setCursor(Qt::PointingHandCursor);
	QVBoxLayout *mainLayout = new QVBoxLayout;

	std::vector<QString> imageDirs{};
	imageDirs.push_back(":/Images/whiteQueen");
	imageDirs.push_back(":/Images/whiteKnight");
	imageDirs.push_back(":/Images/whiteRook");
	imageDirs.push_back(":/Images/whiteBishop");

	std::vector<int> pieceTypes{};
	pieceTypes.push_back(whiteQueen);
	pieceTypes.push_back(whiteKnight);
	pieceTypes.push_back(whiteRook);
	pieceTypes.push_back(whiteBishop);

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
	
	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << piece->getPieceType();

	pieceTypeSelected = piece->getPieceType();

	delete piece;
	close();
}


