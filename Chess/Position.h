#pragma once
#include <qstring.h>
#include <vector>
#include <map>
#include "PieceInfo.h"
#include "Move.h"

std::map<QChar, int> const pieceLetters = {
	{ 'p', blackPawn },
	{ 'r', blackRook },
	{ 'n', blackKnight },
	{ 'b', blackBishop },
	{ 'q', blackQueen },
	{ 'k', blackKing },
	{ 'P', whitePawn },
	{ 'R', whiteRook },
	{ 'N', whiteKnight },
	{ 'B', whiteBishop },
	{ 'Q', whiteQueen },
	{ 'K', whiteKing },
	{ '-', empty } // using '-' when need for empty square
};

std::map<int, QChar> const pieceLettersReverse = {
	{ blackPawn,'p' },
	{ blackRook, 'r' },
	{ blackKnight, 'n'  },
	{ blackBishop, 'b'  },
	{ blackQueen, 'q'  },
	{ blackKing, 'k'  },
	{ whitePawn, 'P'  },
	{ whiteRook, 'R'  },
	{ whiteKnight, 'N'  },
	{ whiteBishop, 'B'  },
	{ whiteQueen, 'Q'  },
	{ whiteKing, 'K'  }
};

class Position
{
public:
	Position();
	Position(QString fenString);
	~Position();

	QString getFenString() { return fenString; };
	void setFenString(QString t_fenString) { fenString = t_fenString; };
	QChar getActiveColor() { return activeColor; };
	int getActiveColorInt();
	void setActiveColor(QChar t_activeColor) { activeColor = t_activeColor; };
	bool canWhiteCastleKingside() { return whiteCanCastleKingside; };
	bool canWhiteCastleQueenside() { return whiteCanCastleQueenside; };
	bool canBlackCastleKingside() { return blackCanCastleKingside; };
	bool canBlackCastleQueenside() { return blackCanCastleQueenside; };
	QString getEnPassantSquare() { return enPassantSquare; };
	int getHalfMoveClock() { return halfMoveClock; };
	int getFullMove() { return fullMove; };
	std::vector<int> getPiecePlacement() { return piecePlacement; };
	void insertNewMove(Move const &move);

private:
	void processFenString(QString const text);
	void getHalfMoveClock(QChar * &it, QString &text, const QChar &space);
	void findFullMove(QChar *& it, QString & text);
	int	findIntFromString(QChar *& it, QString & text, const QChar & space);
	void findEnPassantSquareValue(QChar * &it, QString &text, const QChar &space);
	void findColorValue(QChar * &it);
	void setCastleValues(std::vector<QChar> &castleVector);
	void findCastleValues(QChar * &it, QString &text, const QChar &space);
	void findPiecePlacement(QChar * &it, QString &text, const QChar &space);
	void pushBackEmptySquares(int const n);
	void pushBackPieceInSquare(QChar const pieceChar);
	void pushBackLetterInFenString(int pieceNumber, QString &inputString);
	void pushBackNumberInFenString(int &number, QString &inputString);
	QString createFenStringFromPiecePlacement();

	QString fenString;
	std::vector<int> piecePlacement;
	QChar activeColor;
	bool	whiteCanCastleKingside;
	bool	whiteCanCastleQueenside;
	bool	blackCanCastleKingside;
	bool	blackCanCastleQueenside;
	QString enPassantSquare;
	int		halfMoveClock;
	int		fullMove;
};

