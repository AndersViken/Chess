#pragma once

#include <QtWidgets>
#include <QtWidgets/QMainWindow>
#include "ui_Chess.h"
#include "chessTypes.h"
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

	Chess::ChessPiece generatePiece(int const pieceType, int const pieceID);

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
	std::vector<ChessPiece> pieces = {};
	QLabel *boarder;


	void showBoard();
	void hideBoard();
	std::vector<Chess::ChessPiece> Chess::generatePieces();
	void generateBoard();
	void generateBoarder();
	Chess::Square generateSquare(int const color, int const rowNumber, int const colNumber);
	std::vector<Coordinate> Chess::generateInitialCoordinates();
	std::vector<QLabel*> Chess::generateLabelCoordinates();
	std::vector<Square> Chess::generateRow(int const rowNumber);
	
	Chess::Coordinate getCoordinate(int const pieceID);
	void printPieceInfo(std::vector<ChessPiece> &pieces);

	void setLabelBackgroundColor(const int &color, QLabel * label);
	void setLabelColor(const int &color, QLabel * label);

};
