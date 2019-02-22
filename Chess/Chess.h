#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Chess.h"
#include <vector>
#include <QLabel>

class Chess : public QMainWindow
{
	Q_OBJECT

public:
	Chess(QWidget *parent = Q_NULLPTR);
	struct Coordinate
	{
		int xPos;
		int yPos;
	};

	struct Square
	{
		QLabel* label;
	};
	struct Piece
	{
		QLabel* label;
		Coordinate coordinate;
		QString imagePath;
		int pieceId;
		int pieceType;
		int color;
	};
private:
	Ui::ChessClass ui;
	



	std::vector< std::vector <Square> > board = {};
	std::vector<QLabel*> labelCoordinates = {};
	std::vector<Coordinate> pieceCoordinates = {};
	std::vector<Piece> pieces = {};
	QLabel *boarder;


	void showBoard();
	void hideBoard();
	std::vector<Chess::Piece> Chess::generatePieces();
	void generateBoard();
	void generateBoarder();
	void generateSquare(Square &square, int const color, int const rowNumber, int const colNumber);
	std::vector<Chess::Coordinate> Chess::generateInitialCoordinates();
	std::vector<QLabel*> Chess::generateLabelCoordinates();
	std::vector<Chess::Square> Chess::generateRow(int const rowNumber);
	
	Chess::Coordinate getCoordinate(int const pieceID);
	void printPieceInfo(std::vector<Piece> &pieces);

	void setLabelBackgroundColor(const int &color, QLabel * label);
	void setLabelColor(const int &color, QLabel * label);

};
