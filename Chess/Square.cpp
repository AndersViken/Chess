#include "Square.h"
#include <memory>
#include "GuiSetup.h"

QString createLabelStyleSheet(QString colorString);

void generateSquare(QWidget *parent, Square &square, int const color, int const rowNumber, int const colNumber)
{
	QLabel * label = new QLabel(parent);
	int const xPos = boardStartLeft + colNumber * squarePixelSize;
	int const yPos = boardStartTop + squarePixelSize * (squaresInARow - (rowNumber + 1));
	label->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));

	QString colorString;
	if (color == darkColor)
		colorString = darkColorString;
	else if (color == lightColor)
		colorString = lightColorString;
	//square->setStyleSheet(createLabelStyleSheet(colorString));
	label->setStyleSheet("QLabel { background - color: rgb(240, 218, 181) }");
	square.label = label;
	return;
}

QString createLabelStyleSheet(QString colorString)
{
	QString styleSheet = "QLabel { ";
	styleSheet.append(colorString);
	styleSheet.append(" }");
	return styleSheet;
}
