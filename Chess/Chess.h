#pragma once

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "ui_Chess.h"
#include "Piece.h"
#include "Square.h"
#include "Position.h"
#include "PositionController.h"
#include "PositionAnalyzer.h"
#include "PieceAnimation.h"
#include "PieceView.h"
#include <vector>
#include <QLabel>

static inline QString chessMimeType() { return QStringLiteral("application/x-Chess"); }



class Chess : public QMainWindow
{
	Q_OBJECT

public:
	Chess(QWidget *parent = Q_NULLPTR);

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
private:
	Ui::ChessClass ui;

	int const maxDepthWhenAnalysing = 5;

	std::vector< std::vector <Square*> > board = {};
	std::vector<QLabel*> labelCoordinates = {};
	std::vector<QPoint> pieceCoordinates = {};
	std::vector<int> legalCoordinatesX{};
	std::vector<int> legalCoordinatesY{};
	std::vector<Piece*> pieces = {};
	Position position = {};
	PositionController positionController = {};
	PositionAnalyzer positionAnalyzer = {};
	PieceAnimation pieceAnimation = {};
	PieceView pieceView = {};
	QLabel *boarder;
	QLabel* fenStringLabel;
	QTableView* moveTableView;
	QTableView* analysisTableView;
	QStandardItemModel* moveTableModel;
	QStandardItemModel* analysisTableModel;
	void showBoard();
	void hideBoard();
	void updateFenString();
	void updateBoard();
	void updateColorRules();
	std::vector<Piece*> generatePieces(Position &position);
	void showAllPieces(std::vector<Piece*>& pieceVec);
	void generateBoard(Position &position);
	void generateBoarder();
	void generateFenStringLabel();
	void generateMoveTable();
	void generateAnalysisTable();
	void insertMoveInMoveTable(QStandardItemModel *& model, int halfMove, QString move, int activeColor);
	void updateAnalysisTable(QStandardItemModel *& model);
	void addAnalysisTableRow(QStandardItemModel *& model, QString title, int value);
	Square* generateSquare(int const color, int const rowNumber, int const colNumber);
	std::vector<QPoint> Chess::generateInitialCoordinates();
	std::vector<QLabel*> generateLabelCoordinates();
	std::vector<Square*> generateRow(int const rowNumber);
	QPoint getPointFromSquareID(int const pieceID);
	void printPieceInfo(std::vector<Piece*> &pieces);
	int Chess::getClosestNumber(int const num, std::vector<int> const &numbers);
	QPoint giveCoordinateToDroppedPiece(QPoint droppedPosition, QPoint origPosition);
	void generateLegalCoordinates(std::vector<int>& coordinatesX, std::vector<int>& coordinatesY);
	void setLabelBackgroundColor(const int &color, QLabel * label);
	void setLabelColor(const int &color, QLabel * label);
	QString Chess::getPositionFromDialog(QInputDialog & dialog);
	void handleMove(Move & move, Position & t_position, std::vector<Piece*>& t_pieces, int const pieceType, QPoint const & newPoint, QPoint const & origPoint);
	void handleLegalMove(Piece *& piece, int const pieceType, QPoint const & newPoint, Move const & move, Position & origPosition, Position & newPosition, std::vector<Piece*>& t_pieces);
	void checkIfCastling(Move & move, Position & newPosition);
	void performCastling(Move move, std::vector<Piece*>& t_pieces, int const pieceType);
	void getMoveString(Position & origPosition, Position & newPosition, std::vector<Piece*>& t_pieces, QString & moveString, Move const & move, int const pieceType);
	void getSquareString(const int & origSquareID, QString &origSquareString);
	QChar getPieceChar(int const pieceType);

};
