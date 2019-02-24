#include "Chess.h"
#include "piece.h"
#include "GuiSetup.h"
#include "Position.h"
#include <QDebug>
#include <map>
#include <algorithm>
#include <iterator>
Chess::Chess(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setMinimumSize(minWinSizeX, minWinSizeY);
	setWindowTitle(tr("Chess"));
	setAcceptDrops(true);

	generateBoard();

	Position p{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 42 1" };

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
	generateLegalCoordinates(legalCoordinatesX, legalCoordinatesY);
	pieces = generatePieces();
	printPieceInfo(pieces);
}

std::vector<Square*> Chess::generateRow(int const rowNumber)
{
	std::vector<Square*> row{ };
	int color{ rowNumber };
	for (int colNumber = 0; colNumber < squaresInARow; colNumber++) {
		color++;
		row.push_back(generateSquare((color) % 2, rowNumber, colNumber));
	}
	return row;
}

Square* Chess::generateSquare(int const color, int const rowNumber, int const colNumber)
{
	Square *square{ new Square(this) };

	int const xPos{ boardStartX + colNumber * squarePixelSize };
	int const yPos{ boardStartY + squarePixelSize * (squaresInARow - (rowNumber + 1)) };
	square->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));
	setLabelBackgroundColor(color, square);

	return square;
}

void Chess::hideBoard()
{
	for (auto &row : board) {
		for (auto &square : row) {
			square->hide();
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
			square->show();
		}
	}

	for (auto &coordinate : labelCoordinates) {
		coordinate->show();
	}

	boarder->show();
}

void Chess::printPieceInfo(std::vector<Piece*> &pieceVec)
{
	for (auto &piece : pieceVec) {
		qDebug() << "Piece: " <<
			" type: " << piece->getPieceType() <<
			" color: " << piece->getColor() <<
			" xPos: " << piece->getCoordinate().rx() <<
			" yPos: " << piece->getCoordinate().ry() <<
			" path: " << piece->getImagePath();
	}
}

int Chess::getClosestNumber(int const num, std::vector<int> const &numbers)
{
	auto closestNumber = std::upper_bound(numbers.begin(), numbers.end(), num);
	if(closestNumber != numbers.end())
		return static_cast<int>(*closestNumber);
	else return 0; // error, out of bounce. The caller should handle this.
}

QPoint Chess::giveCoordinateToDroppedPiece(QPoint droppedPosition, QPoint origPosition)
{
	if (droppedPosition.rx() < boardStartX ||
		droppedPosition.rx() > boardStartX + boardWidth ||
		droppedPosition.ry() < boardStartY ||
		droppedPosition.ry() > boardStartY + boardHeigth ) {
		return origPosition;
	}
	int const posX = getClosestNumber(droppedPosition.rx() - squarePixelSize / 2, legalCoordinatesX);
	int const posY = getClosestNumber(droppedPosition.ry() - squarePixelSize / 2, legalCoordinatesY);
	QPoint point{ posX, posY };
	
	// piece dropped outside of board
	if(point.rx() == 0 || point.ry() == 0) {
		return origPosition;
	}
	return point;
}

void Chess::generateLegalCoordinates(std::vector<int> &coordinatesX, std::vector<int> &coordinatesY)
{
	for (int col = 0; col < squaresInARow; col++) {
		coordinatesX.push_back(boardStartX + col * squarePixelSize);
	}
	for (int row = 0; row < squaresInARow; row++) {
		coordinatesY.push_back(boardStartY + squarePixelSize * (squaresInARow - (row + 1)));
	}
	std::sort(coordinatesX.begin(), coordinatesX.end());
	std::sort(coordinatesY.begin(), coordinatesY.end());
}

std::vector<QPoint> Chess::generateInitialCoordinates()
{
	std::vector<QPoint> coordinates { };
	for (int row = 0; row < squaresInARow; row++) {
		for (int col = 0; col < squaresInARow; col++) {
			int const xPosition = boardStartX + col * squarePixelSize;
			int const yPosition = boardStartY + squarePixelSize * (squaresInARow - (row + 1));
			QPoint coordinate = { xPosition, yPosition };
			coordinates.push_back(coordinate);
		}
		if (row == 1)
			row = 5;
	}
	return coordinates;
}

QPoint Chess::getCoordinate(int const pieceID)
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
		
		QPoint coordinate = getCoordinate(pieceID++);
		piecesVec.push_back(generatePiece(pieceType, coordinate));
	}
	return piecesVec;
}

Piece* Chess::generatePiece(int const pieceType, QPoint coordinate)
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
	int const boarderStartLeft{ boardStartX - coordinateOffsetBoarder };
	int const boarderStartTop{ boardStartY - coordinateOffsetBoarder };
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

		int const xPos{ boardStartX + coordinateOffsetColLeft + colNumber * squarePixelSize };
		int const yPos{ boardStartY + coordinateOffsetColTop + squarePixelSize * (squaresInARow - (rowNumber + 1)) };
		coordinate->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));
		coordinate->setText(colCoordinateNames.at(static_cast<std::size_t>(colNumber)));
		setLabelColor(colNumber % 2, coordinate);
		coordinates.push_back(coordinate);
	}

	std::vector<QString> rowCoordinateNames { "1","2","3","4","5","6","7","8" };
	colNumber = 0;
	for (rowNumber = 0; rowNumber < squaresInARow; rowNumber++) {
		QLabel *coordinate = new QLabel(this);

		int const xPos{ boardStartX + coordinateOffsetRowLeft + colNumber * squarePixelSize };
		int const yPos{ boardStartY + coordinateOffsetRowTop + squarePixelSize * (squaresInARow - (rowNumber + 1)) };
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

		int pieceType{};
		QPoint offset{};
		QPoint origPosition{};
		dataStream >> pieceType >> origPosition.rx() >> origPosition.ry() >> offset;

		QPoint droppedPosition{ event->pos() - offset };
		QPoint newPosition = giveCoordinateToDroppedPiece(droppedPosition, origPosition);
		Piece *piece = generatePiece(pieceType, newPosition);
		piece->show();
		piece->setAttribute(Qt::WA_DeleteOnClose);

		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else if (event->mimeData()->hasText()) {

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
	dataStream	<< piece->getPieceType() << piece->getCoordinate().rx() << piece->getCoordinate().ry() << QPoint(hotSpot);

	QMimeData *mimeData{ new QMimeData };
	mimeData->setData(chessMimeType(), itemData);

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
