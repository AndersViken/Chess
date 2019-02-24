#pragma once
#include <qstring.h>
#include <vector>
class Position
{
public:
	Position();
	Position(QString fenString);
	~Position();

	QString getFenString()			{ return fenString; };
	QChar getActiveColor()			{ return activeColor; };
	bool canWhiteCastleKingside()	{ return whiteCanCastleKingside; };
	bool canWhiteCastleQueenside()	{ return whiteCanCastleQueenside; };
	bool canBlackCastleKingside()	{ return blackCanCastleKingside; };
	bool canBlackCastleQueenside()	{ return blackCanCastleQueenside; };
	QString getEnPassantSquare()	{ return enPassantSquare; };
	int getHalfMoveClock()			{ return halfMoveClock; };
	int getFullMove()				{ return fullMove; };

	void processFenString(QString const text);
	void getHalfMoveClock(QChar * &it, QString &text, const QChar &space);
	void getFullMove(QChar *& it, QString & text);
	int	getIntFromString(QChar *& it, QString & text, const QChar & space);
	void getEnPassantSquareValue(QChar * &it, QString &text, const QChar &space);
	void getColorValue(QChar * &it);
	void setCastleValues(std::vector<QChar> &castleVector);
	void getCastleValues(QChar * &it, QString &text, const QChar &space);

private:
	QString fenString;
	QChar activeColor;
	bool	whiteCanCastleKingside;
	bool	whiteCanCastleQueenside;
	bool	blackCanCastleKingside;
	bool	blackCanCastleQueenside;
	QString enPassantSquare;
	int		halfMoveClock;
	int		fullMove;
};

