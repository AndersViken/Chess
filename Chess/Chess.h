#pragma once

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "ui_Chess.h"
#include "ChessTypes.h"
#include "Piece.h"
#include <vector>
#include <QLabel>

static inline QString chessMimeType() { return QStringLiteral("application/x-Chess"); }



class Chess : public QMainWindow
{
	Q_OBJECT

public:
	Chess(QWidget *parent = Q_NULLPTR);

	struct ChessPiece
	{
		QLabel* label;
		Coordinate coordinate;
		int pieceId;
		int pieceType;
		int color;
		QString imagePath;
	};

protected:
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dragMoveEvent(QDragMoveEvent *event) override;
	void dropEvent(QDropEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
private:
	Ui::ChessClass ui;
	
	std::vector< std::vector <Square> > board = {};
	std::vector<QLabel*> labelCoordinates = {};
	std::vector<Coordinate> pieceCoordinates = {};
	std::vector<Piece*> pieces = {};
	QLabel *boarder;

	void showBoard();
	void hideBoard();
	Piece* generatePiece(int const pieceType, Coordinate coordinate);
	std::vector<Piece*> generatePieces();
	void generateBoard();
	void generateBoarder();
	Square generateSquare(int const color, int const rowNumber, int const colNumber);
	std::vector<Coordinate> generateInitialCoordinates();
	std::vector<QLabel*> generateLabelCoordinates();
	std::vector<Square> generateRow(int const rowNumber);
	
	Coordinate getCoordinate(int const pieceID);
	void printPieceInfo(std::vector<Piece*> &pieces);

	void setLabelBackgroundColor(const int &color, QLabel * label);
	void setLabelColor(const int &color, QLabel * label);

};
