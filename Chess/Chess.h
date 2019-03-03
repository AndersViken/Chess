#pragma once

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "ui_Chess.h"
#include "Piece.h"
#include "Square.h"
#include "Position.h"
#include "PositionController.h"
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

	std::vector< std::vector <Square*> > board = {};
	std::vector<QLabel*> labelCoordinates = {};
	std::vector<QPoint> pieceCoordinates = {};
	std::vector<int> legalCoordinatesX{};
	std::vector<int> legalCoordinatesY{};
	std::vector<Piece*> pieces = {};
	Position position = {};
	PositionController positioncontroller = {};
	QLabel *boarder;
	QLabel* fenStringLabel;
	void showBoard();
	void hideBoard();
	void updateFenString();
	void updateBoard();
	void updateColorRules();
	Piece* generatePiece(int const pieceType, QPoint coordinate, int squareID);
	std::vector<Piece*> generatePieces(Position &position);
	void removePiece(std::vector<Piece*> &pieceVec, int squareID);
	void showAllPieces(std::vector<Piece*>& pieceVec);
	void generateBoard(Position &position);
	void generateBoarder();
	void generateFenStringLabel();
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

};
