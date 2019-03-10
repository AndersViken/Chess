#include "Chess.h"
#include "piece.h"
#include "GuiSetup.h"
#include "Position.h"
#include "Move.h"
#include "Leaf.h"
#include "PieceInfo.h"
#include "PositionController.h"
#include <qinputdialog.h>
#include <QDebug>
#include <map>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <QTableView>
#include <QHeaderView>
Chess::Chess(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	setMinimumSize(minWinSizeX, minWinSizeY);
	setWindowTitle(tr("Chess"));
	setAcceptDrops(true);


	QInputDialog positionDialog{};
	QString positionString{};

	positionString = getPositionFromDialog(positionDialog);
 
	Position initialPosition{ positionString };
	position = initialPosition;

	
	//initialPosition = positioncontroller.generateNewPosition(move1, initialPosition);
	//initialPosition = positioncontroller.generateNewPosition(move2, initialPosition);
	//initialPosition = positioncontroller.generateNewPosition(move3, initialPosition);
	//initialPosition = positioncontroller.generateNewPosition(move4, initialPosition);

	generateBoard(position);
	generateFenStringLabel();
	generateAnalysisTable();
	generateMoveTable();

	

	//Leaf leaf{ m,initialPosition };


}


std::map<int, int> const pieceColors = {
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

QString imageDir = ":/Images/";
std::map<int, QString> imagePaths = {
	{ whitePawn,    imageDir + "whitePawn"     },
	{ whiteRook,    imageDir + "whiteRook"     },
	{ whiteKnight,  imageDir + "whiteKnight"   },
	{ whiteBishop,  imageDir + "whiteBishop"	},
	{ whiteQueen,   imageDir + "whiteQueen"    },
	{ whiteKing,    imageDir + "whiteKing"     },
	{ blackPawn,    imageDir + "blackPawn"     },
	{ blackRook,    imageDir + "blackRook"     },
	{ blackKnight,  imageDir + "blackKnight"   },
	{ blackBishop,  imageDir + "blackBishop"   },
	{ blackQueen,   imageDir + "blackQueen"    },
	{ blackKing,    imageDir + "blackKing"     }
};

std::map<int, QChar> pieceChars = {
	{ whiteRook,    'R' },
	{ whiteKnight,  'N' },
	{ whiteBishop,  'B'	},
	{ whiteQueen,   'Q' },
	{ whiteKing,    'K' },
	{ blackRook,    'R' },
	{ blackKnight,  'N' },
	{ blackBishop,  'B' },
	{ blackQueen,   'Q' },
	{ blackKing,    'K' }
};

//std::map<int, QPoint> squareIdToCoordinate = {
//	{ }
//}

void Chess::generateBoard(Position &t_position)
{
	board = {};

	generateBoarder();

	for (int rowNumber = 0; rowNumber < squaresInARow; rowNumber++) {
		board.push_back( generateRow(rowNumber) );
	}

	labelCoordinates = generateLabelCoordinates();
	pieceCoordinates = generateInitialCoordinates();
	generateLegalCoordinates(legalCoordinatesX, legalCoordinatesY);
	pieces = generatePieces(t_position);
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
	std::for_each(board.begin(), board.end(), [](auto &row) {
		std::for_each(row.begin(), row.end(), [](auto &square) {
			square->hide();
		});
	});
	std::for_each(labelCoordinates.begin(), labelCoordinates.end(),[](auto &coordinate) {
		coordinate->hide();
	});

	boarder->hide();
}

void Chess::showBoard()
{
	std::for_each(board.begin(), board.end(), [](auto &row) {
		std::for_each(row.begin(), row.end(), [](auto &square) {
			square->show();
		});
	});
	std::for_each(labelCoordinates.begin(), labelCoordinates.end(), [](auto &coordinate) {
		coordinate->show();
	});

	boarder->show();
}

void Chess::printPieceInfo(std::vector<Piece*> &pieceVec)
{
	std::for_each(pieceVec.begin(), pieceVec.end(), [](auto &piece) {
		qDebug() << "Piece: " <<
			" type: " << piece->getPieceType() <<
			" color: " << piece->getColor() <<
			" xPos: " << piece->getCoordinate().rx() <<
			" yPos: " << piece->getCoordinate().ry() <<
			" path: " << piece->getImagePath();
	});
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
	int const halfSquare = squarePixelSize / 2;
	if (droppedPosition.rx() < boardStartX - halfSquare ||
		droppedPosition.rx() > boardStartX + halfSquare + boardWidth  ||
		droppedPosition.ry() < boardStartY - halfSquare ||
		droppedPosition.ry() > boardStartY + halfSquare + boardHeigth) {
		return origPosition;
	}
	int const posX = getClosestNumber(droppedPosition.rx() - halfSquare, legalCoordinatesX);
	int const posY = getClosestNumber(droppedPosition.ry() - halfSquare, legalCoordinatesY);
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

int getSquareIdFromPoint(QPoint point)
{
	int const col = (point.rx() - boardStartX) / squarePixelSize;
	int const row = (point.ry() - boardStartY) / squarePixelSize;
	return ((8 * row) + (col));
}

QPoint Chess::getPointFromSquareID(int const squareNumber)
{
	int const col = squareNumber % 8;
	int const row = squareNumber / 8;
	return { boardStartX + col * squarePixelSize, boardStartY + row * squarePixelSize };
}

std::vector<Piece*> Chess::generatePieces(Position &t_position)
{
	std::vector<Piece*> piecesVec{ };
	
	int squareID = 0;
	for (auto &pieceType : t_position.getPiecePlacement() ) {
		if (pieceType != empty)
		{
			QPoint coordinate = getPointFromSquareID(squareID);
			piecesVec.push_back(generatePiece(pieceType, coordinate, squareID));
		}
		squareID++;
	}
	return piecesVec;
}

Piece* Chess::generatePiece(int const pieceType, QPoint coordinate, int squareID)
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
		Piece *piece{ new Piece(coordinate, squareID, pieceType, color, imagePath , tempImageSize, this) };
		//piece->setAttribute(Qt::WA_DeleteOnClose);
		return piece;
	}
	return {};
}

void Chess::removePiece(std::vector<Piece*>& pieceVec, int squareID)
{
	auto it = std::find_if(pieceVec.begin(), pieceVec.end(),
		[&squareID](Piece *piece) {return (piece->getSquareID() == squareID); });
	if (it != pieceVec.end())
	{
		if (*it)
		{
			Piece* pieceToErase = static_cast<Piece*>(*it);
			pieceToErase->hide();
			pieceVec.erase(it);
		}
	}
}

void Chess::showAllPieces(std::vector<Piece*>& pieceVec)
{
	std::for_each(pieceVec.begin(), pieceVec.end(),
		[](Piece* piece) {piece->show(); });
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

void Chess::generateFenStringLabel()
{
	fenStringLabel = new QLabel(this);
	int const startX{ boardStartX + boardWidth/6 };
	int const startY{ boardStartY + boardHeigth };
	int const labelWidth{ boardWidth - boardWidth / 6 };
	int const labelHeight{ 30 };
	fenStringLabel->setGeometry(QRect(startX, startY, labelWidth, labelHeight));
	//fenStringLabel->setStyleSheet("QLabel { background-color: rgb(60, 60, 60); }");
	updateFenString();
	fenStringLabel->show();
}

void Chess::generateMoveTable()
{
	int const rows = 0;
	int const cols = 2;
	moveTableModel = new QStandardItemModel(rows, cols, this);

	moveTableView = new QTableView(this);
	int const startX{ boardStartX + boardWidth + moveTableOffsetFromBoardX };
	int const startY{ boardStartY + moveTableOffsetFromBoardY };
	moveTableView->setGeometry(QRect(startX, startY, moveTableWidth, moveTableHeight));
	moveTableView->horizontalHeader()->hide();

	moveTableView->setModel(moveTableModel);
}

void Chess::generateAnalysisTable()
{
	analysisTableModel = new QStandardItemModel(this);

	analysisTableView = new QTableView(this);
	int const startX{ boardStartX + boardWidth + analysisTableOffsetFromBoardX };
	int const startY{ boardStartY + analysisTableOffsetFromBoardY };
	analysisTableView->setGeometry(QRect(startX, startY, analysisTableWidth, analysisTableHeight));
	analysisTableView->horizontalHeader()->hide();

	analysisTableView->setModel(analysisTableModel);
}

void Chess::insertMoveInMoveTable(QStandardItemModel *& model, int fullMove, QString move, int activeColor)
{
	if (activeColor == white) {
		QStandardItem* itm = new QStandardItem(QString(""));
		model->appendRow(itm);
	}
	int const row{ (fullMove -1) }; // -1 because fullmove starts at one, but model starts at 0
	int const col{ activeColor };
	qDebug() << "fullMove=" << fullMove << " row=" << row << " col=" << col;


	QModelIndex index = model->index(row, col, QModelIndex());
	// 0 for all data
	model->setData(index, move);
	
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

QString Chess::getPositionFromDialog(QInputDialog & dialog)
{
	QString const windowTitle{ "Input FEN String" };
	QString const positionSuggestion{ "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1" };
	dialog.setWindowTitle(windowTitle);
	dialog.setInputMode(QInputDialog::TextInput);
	dialog.setLabelText(windowTitle);
	dialog.resize(500, 108);
	dialog.setTextValue(positionSuggestion);
	dialog.exec();
	return dialog.textValue();
}


void Chess::updateFenString()
{
	fenStringLabel->setText("FEN:   "+position.getFenString());
}

void Chess::updateBoard()
{
	//showAllPieces(pieces);
	updateFenString();
	updateColorRules();
}

void Chess::updateColorRules()
{

	int activeColor = position.getActiveColorInt();
	std::for_each(pieces.begin(), pieces.end(), [&activeColor](Piece*& piece) {
		if (piece && piece->getSquareID() >= 0 && piece->getColor() == activeColor) {
			piece->makeActive();
		}
		else if (piece && piece->getSquareID() >= 0) {
			piece->makeDisabled();
		}
	});
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
		int origSquareID{};
		QPoint offset{};
		QPoint origPoint{};
		dataStream >> pieceType >> origSquareID >> origPoint.rx() >> origPoint.ry() >> offset;

		QPoint droppedPoint{ event->pos() - offset };
		QPoint newPoint = giveCoordinateToDroppedPiece(droppedPoint, origPoint);
		int newSquareID = getSquareIdFromPoint(newPoint);

		Move move{ origSquareID, newSquareID, "MoveStringNA" };
		Position origPosition = position;
	
		position = positioncontroller.generateNewPosition(move, position);
		
		Piece *piece;
		std::vector<specialMove> specialMoves{};
		bool legalMove = positioncontroller.validateMove(position, origPosition, move, pieceType, specialMoves);
		if (legalMove)
		{
			handleLegalMove(piece, pieceType, newPoint, move, origPosition, position, specialMoves);
		}
		else
		{
			piece = generatePiece(pieceType, origPoint, origSquareID);
			position = origPosition;
		}
			
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
		QPoint point = event->pos();

		std::for_each(eventText.begin(), eventText.end(), [this,&point](QString &piece) {
			QLabel *newLabel = new QLabel(piece, this);
			newLabel->move(point);
			newLabel->show();
			newLabel->setAttribute(Qt::WA_DeleteOnClose);

			point += QPoint(newLabel->width(), 0);
		});

		event->acceptProposedAction();
	}
	else {
		event->ignore();
	}
}

void Chess::handleLegalMove(Piece * &piece, int const pieceType, QPoint const &newPoint, Move const &move,
	Position &origPosition, Position &newPosition, std::vector<specialMove> &specialMoves)
{
	std::for_each(specialMoves.begin(), specialMoves.end(), [this](specialMove move) mutable {
		removePiece(pieces, move.square);
		if (move.pieceOnSquareAfterMove != empty) {
			Piece *newRook = generatePiece(move.pieceOnSquareAfterMove, getPointFromSquareID(move.square), move.square);
			pieces.push_back(newRook);
			newRook->show();
			newRook->setAttribute(Qt::WA_DeleteOnClose);
		}
	});

	// TODO: handle castle here: move rook.
	piece = generatePiece(pieceType, newPoint, move.toSquareId);
	removePiece(pieces, move.toSquareId);
	removePiece(pieces, move.fromSquareId);
	pieces.push_back(piece);
	QString moveString{};
	getMoveString(origPosition, newPosition, moveString, move, pieceType);
	insertMoveInMoveTable(moveTableModel, origPosition.getFullMove(), moveString, origPosition.getActiveColorInt());
	updateBoard();
}

void Chess::getMoveString(Position &origPosition, Position &newPosition, QString &moveString, Move const &move, int const pieceType)
{
	QString origSquareString{};
	QString newSquareString{};
	getSquareString(move.fromSquareId, origSquareString);
	getSquareString(move.toSquareId, newSquareString);
	
	moveString.append(getPieceChar(pieceType));

	if (positioncontroller.checkIfMovingToOppositeColorPiece(origPosition, move, newPosition)) {
		if (pieceType == blackPawn || pieceType == whitePawn) {
			moveString.append(origSquareString.at(0));
		}
		moveString.append('x');
	}
	
	moveString.append(newSquareString);
}

void Chess::getSquareString(int const &squareID, QString &squareString)
{
	std::vector<QChar> boardLetters { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
	int const origRow = squaresInARow - (squareID / squaresInARow);
	QChar const origCol = boardLetters.at(squareID % squaresInARow);
	squareString.append(origCol);
	squareString.append(QString::number(origRow));
}

QChar Chess::getPieceChar(int const pieceType)
{
	QChar pieceChar{};
	auto pieceCharSearch{ pieceChars.find(pieceType) };
	if (pieceCharSearch != pieceChars.end()) {
		pieceChar = pieceCharSearch->second;
	}
	return pieceChar;
}

void Chess::mousePressEvent(QMouseEvent *event)
{
	Piece *piece{ static_cast<Piece*>(childAt(event->pos())) };
	
	if (!piece || !piece->hasTabletTracking()) // TabletTracking used only for Chess Pieces, not board
		return;
	
	QPoint hotSpot{ event->pos() - piece->pos() };

	QByteArray itemData;
	QDataStream dataStream(&itemData, QIODevice::WriteOnly);
	dataStream	<< piece->getPieceType()
				<< piece->getSquareID()
				<< piece->getCoordinate().rx()
				<< piece->getCoordinate().ry()
				<< QPoint(hotSpot);

	QMimeData *mimeData{ new QMimeData };
	mimeData->setData(chessMimeType(), itemData);

	QDrag *drag{ new QDrag(this) };
	drag->setMimeData(mimeData);
	drag->setPixmap(*piece->pixmap());
	drag->setHotSpot(hotSpot);

	piece->hide();

	if (drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction) == Qt::MoveAction)
		//piece->close();
	{
	}
	else
		piece->show();
	
}
