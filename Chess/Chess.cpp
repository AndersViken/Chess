#include "Chess.h"
#include "GuiSetup.h"
#include <QDebug>
#include <map>
Chess::Chess(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
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
		std::vector<Square> row;
		generateRow(row, rowNumber);
		board.push_back(row);
	}

	generateLabelCoordinates(labelCoordinates);
	generateInitialCoordinates(pieceCoordinates);
	generatePieces(pieces);
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

void Chess::generateInitialCoordinates(std::vector<Coordinate> &coordinates)
{
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
}

void Chess::getCoordinate(Coordinate &coordinate, int const pieceID)
{
	if (pieceID < static_cast<int>(pieceCoordinates.size())) {
		coordinate = pieceCoordinates.at(static_cast<std::size_t>(pieceID));
	}
	else {
		coordinate = { 0, 0 };
	}


}

void Chess::generatePieces(std::vector<Piece> &pieces)
{
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
		getCoordinate(piece.coordinate, piece.pieceId);

		auto imageSearch = imagePaths.find(piece.pieceType);
		if (imageSearch != imagePaths.end()) {
			piece.label = new QLabel(this);
			piece.label->setGeometry(QRect(piece.coordinate.xPos, piece.coordinate.yPos, imageSize, imageSize));
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

void Chess::generateLabelCoordinates(std::vector<QLabel*> &coordinates)
{
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
}

void Chess::generateRow(std::vector<Square> &row, int const rowNumber)
{
	int color{ rowNumber };
	for (int colNumber = 0; colNumber < squaresInARow; colNumber++) {
		color++;
		Square square;
		generateSquare(square, (color) % 2, rowNumber, colNumber);
		row.push_back(square);
	}
}

void Chess::generateSquare(Square &square, int const color, int const rowNumber, int const colNumber)
{
	QLabel *label = new QLabel(this);

	int const xPos = boardStartLeft + colNumber * squarePixelSize;
	int const yPos = boardStartTop + squarePixelSize * (squaresInARow - (rowNumber + 1));
	label->setGeometry(QRect(xPos, yPos, squarePixelSize, squarePixelSize));

	setLabelBackgroundColor(color, label);

	square = { label };
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
