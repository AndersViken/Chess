#include "Chess.h"
#include "piece.h"
#include "GuiSetup.h"
#include "Position.h"
#include "Move.h"
#include "Leaf.h"
#include "PieceInfo.h"
#include "PositionController.h"
#include "PromotionDialog.h"
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
	setCursor(Qt::PointingHandCursor);
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


	positionAnalyzer.analysePosition(position, pieces, maxDepthWhenAnalysing);
	updateAnalysisTable(analysisTableModel);

	PromotionDialog* dialog{ new PromotionDialog() };
	dialog->exec();

	qDebug() << "piece selected: " << dialog->getPieceTypeSelected();

	//Leaf leaf{ m,initialPosition };
}



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
			piecesVec.push_back(pieceView.generatePiece(pieceType, coordinate, squareID, this));
		}
		squareID++;
	}
	return piecesVec;
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
	analysisTableModel = new QStandardItemModel(0,1,this);

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

	QModelIndex index = model->index(row, col, QModelIndex());
	// 0 for all data
	model->setData(index, move);
	
}

void Chess::updateAnalysisTable(QStandardItemModel *& model)
{
	model->clear();
	if (position.getActiveColorInt() == white) {
		addAnalysisTableRow(model, "White to move", 0);
	}
	if (position.getActiveColorInt() == black) {
		addAnalysisTableRow(model, "Black to move", 0);
	}
	addAnalysisTableRow(model, "Piece Values:", positionAnalyzer.getPieceValueSum());
	addAnalysisTableRow(model, "Possible Moves:", positionAnalyzer.getUpdatedNumberOfValidMoves(position, pieces));

	QString gameStatus{};
	switch(position.getGameStatus()) {
		case GameStatus::notStarted:
		gameStatus.append("Not Started.");
		break;
		case GameStatus::inProgress:
		gameStatus.append("In progress.");
		break;
		case GameStatus::draw:
		gameStatus.append("draw.");
		break;
		case GameStatus::stalemate:
		gameStatus.append("stalemate.");
		break;
		case GameStatus::whiteWon:
		gameStatus.append("White won.");
		break;
		case GameStatus::blackWon:
		gameStatus.append("Black won.");
		break;
		default: break;
	}
	
	addAnalysisTableRow(model, gameStatus, 0);

}

void Chess::addAnalysisTableRow(QStandardItemModel *& model, QString title, int value)
{
	QList<QStandardItem*> row{};
	QString text{};
	if (value > 0) {
		text.append('+');
	}
	text.append(QString::number(value));
	row.append(new QStandardItem(title));
	row.append(new QStandardItem(text));

	model->appendRow(row);
	row.clear();
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

void Chess::handleMove(Move & move, Position & t_position, std::vector<Piece*>& t_pieces, int const pieceType, QPoint const &newPoint, QPoint const &origPoint)
{
	Position origPosition = t_position;
	bool legalMove = positionController.validateMove(t_position, t_pieces, move);

	Piece *piece;
	if (legalMove)
	{
		// TODO: should here also detect a rook move, to loose castling rights with that rook
		checkIfCastling(move, t_position);

		t_position = positionController.generateNewPosition(move, t_position);
		handleLegalMove(piece, pieceType, newPoint, move, origPosition, t_position, t_pieces);

	}
	else
	{
		//piece = pieceView.generatePiece(pieceType, origPoint, move.fromSquareId, this);
		t_position = origPosition;
	}

	
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

	//setCursor(Qt::ClosedHandCursor);
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

	//setCursor(Qt::ClosedHandCursor);
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
	//unsetCursor();
	setCursor(Qt::PointingHandCursor);

	if (event->mimeData()->hasFormat(chessMimeType())) {
		const QMimeData *mime = event->mimeData();
		QByteArray itemData = mime->data(chessMimeType());
		QDataStream dataStream(&itemData, QIODevice::ReadOnly);

		int pieceType{};
		int origSquareID{};
		QPoint origPoint{};
		QPoint offset{};
		dataStream >> pieceType >> origSquareID >> origPoint.rx() >> origPoint.ry() >> offset;

		QPoint droppedPoint{ event->pos() - offset };
		QPoint newPoint = giveCoordinateToDroppedPiece(droppedPoint, origPoint);
		int newSquareID = getSquareIdFromPoint(newPoint);

		Move move{ origSquareID, newSquareID };
		handleMove(move, position, pieces, pieceType, newPoint, origPoint);
		
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
	Position &origPosition, Position &newPosition, std::vector<Piece*>& t_pieces)
{
	QString moveString = move.moveString;

	piece = pieceView.generatePiece(pieceType, newPoint, move.toSquareId, this);
	positionController.removePiece(pieces, move.toSquareId);
	positionController.removePiece(pieces, move.fromSquareId);

	piece->show();
	piece->setAttribute(Qt::WA_DeleteOnClose);
	pieces.push_back(piece);
	
	if (!moveString.compare("")){ // move string not filled yet
		getMoveString(origPosition, newPosition, t_pieces, moveString, move, pieceType);
	}
	insertMoveInMoveTable(moveTableModel, origPosition.getFullMove(), moveString, origPosition.getActiveColorInt());
	updateBoard();
	positionAnalyzer.analysePosition(newPosition, pieces, maxDepthWhenAnalysing);
	updateAnalysisTable(analysisTableModel);
}

void Chess::checkIfCastling(Move& move, Position& newPosition)
{

	switch (move.moveType) {
	case MoveType::castleKingsideWhite:
		performCastling(newPosition, { 63,61 }, pieces, whiteRook);
		newPosition.setCastleLegalWhiteKingside(false);
		newPosition.setCastleLegalWhiteQueenside(false);
		break;
	case MoveType::castleQueensideWhite:
		performCastling(newPosition, { 56,59 }, pieces, whiteRook);
		newPosition.setCastleLegalWhiteKingside(false);
		newPosition.setCastleLegalWhiteQueenside(false);
		break;
	case MoveType::castleKingsideBlack:
		performCastling(newPosition, { 7,5 }, pieces, blackRook);
		newPosition.setCastleLegalBlackKingside(false);
		newPosition.setCastleLegalBlackQueenside(false);
		break;
	case MoveType::castleQueensideBlack:
		performCastling(newPosition, { 0,3 }, pieces, blackRook);
		newPosition.setCastleLegalBlackKingside(false);
		newPosition.setCastleLegalBlackQueenside(false);
		break;
	default: break;
	}
}

void Chess::performCastling(Position &t_position, Move move,std::vector<Piece*>& t_pieces, int const pieceType)
{
	positionController.removePiece(t_pieces, move.fromSquareId);
	Piece *newRook = pieceView.generatePiece(pieceType, getPointFromSquareID(move.toSquareId),move.toSquareId, this);
	t_pieces.push_back(newRook);
	t_position.insertNewMove(move);
	newRook->show();
	newRook->setAttribute(Qt::WA_DeleteOnClose);
}

void Chess::getMoveString(Position &origPosition, Position &newPosition, std::vector<Piece*>& t_pieces, QString &moveString, Move const &move, int const pieceType)
{
	QString origSquareString{};
	QString newSquareString{};
	getSquareString(move.fromSquareId, origSquareString);
	getSquareString(move.toSquareId, newSquareString);
	
	moveString.append(getPieceChar(pieceType));

	if (positionController.checkIfMovingToOppositeColorPiece(origPosition, move.toSquareId, newPosition)) {
		if (pieceType == blackPawn || pieceType == whitePawn) {
			moveString.append(origSquareString.at(0));
		}
		moveString.append('x');
	}
	
	moveString.append(newSquareString);
	int const activeColor{ origPosition.getActiveColorInt() };
	bool const kingAttacked{ positionController.checkIfKingAttacked(newPosition, t_pieces, activeColor) };
	newPosition.setActiveKingAttacked(kingAttacked);
	int const numberOfPossibleMoves{ positionAnalyzer.getUpdatedNumberOfValidMoves(newPosition, t_pieces) };
	if (kingAttacked) {
		if (numberOfPossibleMoves == 0) {
			moveString.append('#');
			newPosition.setGameFinished(true);
			GameStatus gameStatus{ activeColor == white ? GameStatus::whiteWon : GameStatus::blackWon };
			newPosition.setGameStatus(gameStatus);
		}
		else {
			moveString.append('+');
			newPosition.setGameStatus(GameStatus::inProgress);
		}
	}
	else if (numberOfPossibleMoves == 0) {
		moveString.append('=');
		newPosition.setGameFinished(true);
		newPosition.setGameStatus(GameStatus::stalemate);
	}
	else {
		newPosition.setGameStatus(GameStatus::inProgress);
	}
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

	//setCursor(Qt::ClosedHandCursor);
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
		//setCursor(Qt::ClosedHandCursor);
	}
	else {
		piece->show();
	}
	setCursor(Qt::PointingHandCursor);
}
