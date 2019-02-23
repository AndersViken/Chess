#include "Chess.h"
#include "piece.h"
#include "GuiSetup.h"
#include <QDebug>
#include <map>
Chess::Chess(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setMinimumSize(minWinSizeLeft, minWinSizeTop);
	setWindowTitle(tr("Chess"));
	setAcceptDrops(true);

	generateBoard();

}

void Chess::hideBoard()
{
	for (auto &row : board) {
		for (auto &square : row) {
			square.label->hide();
		}
	}

	for (auto &coordinate : labelCoordinates) {
		coordinate->hide();
	}

	boarder->hide();
}

void Chess::showBoard()
{
	for (auto &row : board) {
		for (auto &square : row) {
			square.label->show();
		}
	}

	for (auto &coordinate : labelCoordinates) {
		coordinate->show();
	}

	boarder->show();
}

void Chess::generateBoard()
{
	board = {};

	generateBoarder();

	for (int rowNumber = 0; rowNumber < squaresInARow; rowNumber++) {
		board.push_back( generateRow(rowNumber) );
	}

	labelCoordinates = generateLabelCoordinates();
	pieceCoordinates = generateInitialCoordinates();
	pieces = generatePieces();
	printPieceInfo(pieces);
}

std::vector<Square> Chess::generateRow(int const rowNumber)
{
	std::vector<Square> row{ };
	int color{ rowNumber };
	for (int colNumber = 0; colNumber < squaresInARow; colNumber++) {
		color++;
		row.push_back(generateSquare((color) % 2, rowNumber, colNumber));
	}
	return row;
}

Square Chess::generateSquare(int const color, int const rowNumber, int const colNumber)
{
	Square square{};
	square.label = new QLabel(this);

	int const xPos = boardStartLeft + colNumber * squarePixelSize;
	int const yPos = boardStartTop + squarePixelSize * (squaresInARow - (rowNumber + 1));
	square.label->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));

	setLabelBackgroundColor(color, square.label);

	return square;
}

void Chess::printPieceInfo(std::vector<Piece*> &pieceVec)
{
	for (auto &piece : pieceVec) {
		qDebug() << "Piece: " <<
			" type: " << piece->getPieceType() <<
			" color: " << piece->getColor() <<
			" xPos: " << piece->getCoordinate().xPos <<
			" yPos: " << piece->getCoordinate().yPos <<
			" path: " << piece->getImagePath();
	}
}

std::vector<Coordinate> Chess::generateInitialCoordinates()
{
	std::vector<Coordinate> coordinates { };
	for (int row = 0; row < squaresInARow; row++) {
		for (int col = 0; col < squaresInARow; col++) {
			int const xPosition = boardStartLeft + imageOffsetLeft + col * squarePixelSize;
			int const yPosition = boardStartTop + imageOffsetLeft + squarePixelSize * (squaresInARow - (row + 1));
			Coordinate coordinate = { xPosition, yPosition };
			coordinates.push_back(coordinate);
		}
		if (row == 1)
			row = 5;
	}
	return coordinates;
}

Coordinate Chess::getCoordinate(int const pieceID)
{
	if (pieceID < static_cast<int>(pieceCoordinates.size())) {
		return pieceCoordinates.at(static_cast<std::size_t>(pieceID));
	}
	else {
		return { 0, 0 };
	}
}

std::vector<Piece*> Chess::generatePieces()
{
	std::vector<Piece*> piecesVec{ };
	
	int pieceID = 0;
	for (auto &pieceType : pieceTypes) {
		
		Coordinate coordinate = getCoordinate(pieceID++);
		piecesVec.push_back(generatePiece(pieceType, coordinate));
	}
	return piecesVec;
}

Piece* Chess::generatePiece(int const pieceType, Coordinate coordinate)
{

	auto colorSearch = pieceColors.find(pieceType);
	int color{};
	if (colorSearch != pieceColors.end()) {
		color = colorSearch->second;
	}
	else {
		qDebug() << "generatePiece(): no color found.";
		return{};
	}

	auto imageSearch{ imagePaths.find(pieceType) };
	if (imageSearch != imagePaths.end()) {
		QString imagePath = imageSearch->second;
		int tempImageSize = imageSize;
		Piece *piece{ new Piece(coordinate, pieceType, color, imagePath , tempImageSize, this) };
		//piece->setAttribute(Qt::WA_DeleteOnClose);
		return piece;
	}
	return {};
}

void Chess::generateBoarder()
{
	boarder = new QLabel(this);
	int const boarderStartLeft{ boardStartLeft - coordinateOffsetBoarder };
	int const boarderStartTop{ boardStartTop - coordinateOffsetBoarder };
	int const boarderSize{ squarePixelSize * squaresInARow + 2 * coordinateOffsetBoarder };
	boarder->setGeometry(QRect(boarderStartLeft, boarderStartTop, boarderSize, boarderSize));
	boarder->setStyleSheet("QLabel { background-color: rgb(60, 60, 60); }");
	boarder->show();
}

std::vector<QLabel*> Chess::generateLabelCoordinates()
{
	std::vector<QLabel*> coordinates { };
	std::vector<QString> colCoordinateNames { "a","b","c","d","e","f","g","h" };
	int rowNumber{ 0 };
	int colNumber{ 0 };
	for (; colNumber < squaresInARow; colNumber++) {
		QLabel *coordinate = new QLabel(this);

		int const xPos{ boardStartLeft + coordinateOffsetColLeft + colNumber * squarePixelSize };
		int const yPos{ boardStartTop + coordinateOffsetColTop + squarePixelSize * (squaresInARow - (rowNumber + 1)) };
		coordinate->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));
		coordinate->setText(colCoordinateNames.at(static_cast<std::size_t>(colNumber)));
		setLabelColor(colNumber % 2, coordinate);
		coordinates.push_back(coordinate);
	}

	std::vector<QString> rowCoordinateNames { "1","2","3","4","5","6","7","8" };
	colNumber = 0;
	for (rowNumber = 0; rowNumber < squaresInARow; rowNumber++) {
		QLabel *coordinate = new QLabel(this);

		int const xPos{ boardStartLeft + coordinateOffsetRowLeft + colNumber * squarePixelSize };
		int const yPos{ boardStartTop + coordinateOffsetRowTop + squarePixelSize * (squaresInARow - (rowNumber + 1)) };
		coordinate->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));
		coordinate->setText(rowCoordinateNames.at(static_cast<std::size_t>(rowNumber)));
		setLabelColor(rowNumber % 2, coordinate);

		coordinates.push_back(coordinate);
	}
	return coordinates;
}



void Chess::setLabelBackgroundColor(const int &color, QLabel * label)
{
	if (color == darkColor)
		label->setStyleSheet("QLabel { background-color: rgb(181, 135, 99); }");
	else if (color == lightColor)
		label->setStyleSheet("QLabel { background-color: rgb(240, 218, 181); }");
}

void Chess::setLabelColor(const int &color, QLabel * label)
{
	if (color == darkColor)
		label->setStyleSheet("QLabel { color: rgb(181, 135, 99); }");
	else if (color == lightColor)
		label->setStyleSheet("QLabel { color: rgb(240, 218, 181); }");
}


void Chess::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat(chessMimeType())) {
		if (children().contains(event->source())) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else if (event->mimeData()->hasText()) {
		event->acceptProposedAction();
	}
	else {
		event->ignore();
	}
}

void Chess::dragMoveEvent(QDragMoveEvent *event)
{
	if (event->mimeData()->hasFormat(chessMimeType())) {
		if (children().contains(event->source())) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else if (event->mimeData()->hasText()) {
		event->acceptProposedAction();
	}
	else {
		event->ignore();
	}
}

void Chess::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat(chessMimeType())) {
		const QMimeData *mime = event->mimeData();
		QByteArray itemData = mime->data(chessMimeType());
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		Coordinate coordinate{ };
		int pieceType{};
		QPoint offset{};
		dataStream >> pieceType >> offset;

		QPoint newPosition{ event->pos() - offset };
		int posX{ newPosition.rx() };
		int posY{ newPosition.ry() };
		Piece *piece = generatePiece(pieceType, { posX,posY });
		piece->show();
		piece->setAttribute(Qt::WA_DeleteOnClose);
		piece->move(event->pos() - offset);

		/*QLabel *newLabel = new QLabel("", this);
		newLabel->move(event->pos() - offset);
		newLabel->show();
		newLabel->setAttribute(Qt::WA_DeleteOnClose);*/

		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else if (event->mimeData()->hasText()) {

		qDebug() << "else if (event->mimeData()->hasText())";
		QStringList eventText = event->mimeData()->text().split(QRegularExpression(QStringLiteral("\\s+")),
			QString::SkipEmptyParts);
		QPoint position = event->pos();

		for (const QString &piece : eventText) {
			QLabel *newLabel = new QLabel(piece, this);
			newLabel->move(position);
			newLabel->show();
			newLabel->setAttribute(Qt::WA_DeleteOnClose);

			position += QPoint(newLabel->width(), 0);
		}

		event->acceptProposedAction();
	}
	else {
		qDebug() << "else (event->ignore)";
		event->ignore();
	}
}

void Chess::mousePressEvent(QMouseEvent *event)
{
	Piece *piece{ static_cast<Piece*>(childAt(event->pos())) };
	
	if (!piece || !piece->hasTabletTracking()) // TabletTracking used only for Chess Pieces, not board
		return;
	
	QPoint hotSpot{ event->pos() - piece->pos() };

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream	<< piece->getPieceType() << QPoint(hotSpot);

	QMimeData *mimeData{ new QMimeData };
	mimeData->setData(chessMimeType(), itemData);
	/*mimeData->setText(child->text);*/

	QDrag *drag{ new QDrag(this) };
	drag->setMimeData(mimeData);
	drag->setPixmap(*piece->pixmap());
	drag->setHotSpot(hotSpot);

	piece->hide();

	if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
		piece->close();
	else
		piece->show();
	
}
