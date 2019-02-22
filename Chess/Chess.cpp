#include "Chess.h"
#include "GuiSetup.h"
#include <QDebug>
#include <map>
Chess::Chess(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setMinimumSize(minWinSizeLeft, minWinSizeTop);
	setWindowTitle(tr("Fridge Magnets"));
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

void Chess::printPieceInfo(std::vector<Piece> &pieces)
{
	for (auto &piece : pieces) {
		qDebug() << "Piece " << piece.pieceId <<
			" type: " << piece.pieceType <<
			" color: " << piece.color <<
			" xPos: " << piece.coordinate.xPos <<
			" yPos: " << piece.coordinate.yPos <<
			" path: " << piece.imagePath
			;
	}
}

std::vector<Chess::Coordinate> Chess::generateInitialCoordinates()
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

Chess::Coordinate Chess::getCoordinate(int const pieceID)
{
	if (pieceID < static_cast<int>(pieceCoordinates.size())) {
		return pieceCoordinates.at(static_cast<std::size_t>(pieceID));
	}
	else {
		return { 0, 0 };
	}


}

std::vector<Chess::Piece> Chess::generatePieces()
{
	std::vector<Piece> pieces{ };
	QString imagePath = ":/Images/";
	std::map<int, QString> imagePaths = {
		{ whitePawn,    imagePath + "whitePawn"     },
		{ whiteRook,    imagePath + "whiteRook"     },
		{ whiteKnight,  imagePath + "whiteKnight"   },
		{ whiteBishop,  imagePath + "whiteBishop"	},
		{ whiteQueen,   imagePath + "whiteQueen"    },
		{ whiteKing,    imagePath + "whiteKing"     },
		{ blackPawn,    imagePath + "blackPawn"     },
		{ blackRook,    imagePath + "blackRook"     },
		{ blackKnight,  imagePath + "blackKnight"   },
		{ blackBishop,  imagePath + "blackBishop"   },
		{ blackQueen,   imagePath + "blackQueen"    },
		{ blackKing,    imagePath + "blackKing"     }
	};
	std::map<int, int> pieceColors = {
		{ whitePawn,    white },
		{ whiteRook,    white },
		{ whiteKnight,  white },
		{ whiteBishop,  white },
		{ whiteQueen,   white },
		{ whiteKing,    white },
		{ blackPawn,    black },
		{ blackRook,    black },
		{ blackKnight,  black },
		{ blackBishop,  black },
		{ blackQueen,   black },
		{ blackKing,    black }
	};
	std::vector<int> pieceTypes = {
		whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing, whiteBishop, whiteKnight, whiteRook,
		whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn,
		blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn,
		blackRook, blackKnight, blackBishop, blackQueen, blackKing, blackBishop, blackKnight, blackRook
	};
	int pieceID = 0;
	for (auto &pieceType : pieceTypes) {
		Piece piece = { };
		piece.pieceType = pieceTypes.at(static_cast<std::size_t>(pieceID));
		piece.pieceId = pieceID++;
		piece.coordinate = getCoordinate(piece.pieceId);

		auto imageSearch = imagePaths.find(piece.pieceType);
		if (imageSearch != imagePaths.end()) {
			piece.label = new QLabel(this);
			piece.label->setGeometry(QRect(piece.coordinate.xPos, piece.coordinate.yPos, imageSize, imageSize));
			piece.label->setTabletTracking(true);
			piece.imagePath = imageSearch->second;
			QPixmap image(piece.imagePath);

			piece.label->setPixmap(image.scaled(piece.label->width(), piece.label->height(), Qt::KeepAspectRatio));
		}
		auto colorSearch = pieceColors.find(pieceType);
		if (colorSearch != pieceColors.end()) {
			piece.color = colorSearch->second;
		}
		pieces.push_back(piece);
	}
	return pieces;
}

void Chess::generateBoarder()
{
	boarder = new QLabel(this);
	int const boarderStartLeft = boardStartLeft - coordinateOffsetBoarder;
	int const boarderStartTop = boardStartTop - coordinateOffsetBoarder;
	int const boarderSize = squarePixelSize * squaresInARow + 2 * coordinateOffsetBoarder;
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

		int const xPos = boardStartLeft + coordinateOffsetColLeft + colNumber * squarePixelSize;
		int const yPos = boardStartTop + coordinateOffsetColTop + squarePixelSize * (squaresInARow - (rowNumber + 1));
		coordinate->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));
		coordinate->setText(colCoordinateNames.at(static_cast<std::size_t>(colNumber)));
		setLabelColor(colNumber % 2, coordinate);
		coordinates.push_back(coordinate);
	}

	std::vector<QString> rowCoordinateNames { "1","2","3","4","5","6","7","8" };
	colNumber = 0;
	for (rowNumber = 0; rowNumber < squaresInARow; rowNumber++) {
		QLabel *coordinate = new QLabel(this);

		int const xPos = boardStartLeft + coordinateOffsetRowLeft + colNumber * squarePixelSize;
		int const yPos = boardStartTop + coordinateOffsetRowTop + squarePixelSize * (squaresInARow - (rowNumber + 1));
		coordinate->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));
		coordinate->setText(rowCoordinateNames.at(static_cast<std::size_t>(rowNumber)));
		setLabelColor(rowNumber % 2, coordinate);

		coordinates.push_back(coordinate);
	}
	return coordinates;
}

std::vector<Chess::Square> Chess::generateRow(int const rowNumber)
{
	std::vector<Chess::Square> row { };
	int color { rowNumber };
	for (int colNumber = 0; colNumber < squaresInARow; colNumber++) {
		color++;
		row.push_back(generateSquare((color) % 2, rowNumber, colNumber));
	}
	return row;
}

Chess::Square Chess::generateSquare(int const color, int const rowNumber, int const colNumber)
{
	Square square{};
	square.label = new QLabel(this);

	int const xPos = boardStartLeft + colNumber * squarePixelSize;
	int const yPos = boardStartTop + squarePixelSize * (squaresInARow - (rowNumber + 1));
	square.label->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));

	setLabelBackgroundColor(color, square.label);

	return square;
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

		QString text;
		QPoint offset;
		dataStream >> text >> offset;
		QLabel *newLabel = new QLabel(text, this);
		newLabel->move(event->pos() - offset);
		newLabel->show();
		newLabel->setAttribute(Qt::WA_DeleteOnClose);

		if (event->source() == this) {
			event->setDropAction(Qt::MoveAction);
			event->accept();
		}
		else {
			event->acceptProposedAction();
		}
	}
	else if (event->mimeData()->hasText()) {
		QStringList pieces = event->mimeData()->text().split(QRegularExpression(QStringLiteral("\\s+")),
			QString::SkipEmptyParts);
		QPoint position = event->pos();

		for (const QString &piece : pieces) {
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
	QLabel *child = static_cast<QLabel*>(childAt(event->pos()));
	
	if (!child || !child->hasTabletTracking()) // TabletTracking used only for Chess Pieces, not board
		return;
	
	QPoint hotSpot = event->pos() - child->pos();

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream << /*child->labelText() <<*/ QPoint(hotSpot);

	QMimeData *mimeData = new QMimeData;
	mimeData->setData(chessMimeType(), itemData);
	/*mimeData->setText(child->text);*/

	QDrag *drag = new QDrag(this);
	drag->setMimeData(mimeData);
	drag->setPixmap(*child->pixmap());
	drag->setHotSpot(hotSpot);

	child->hide();

	if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
		child->close();
	else
		child->show();
	
}
