#include "Position.h"
#include <qdebug.h>
#include <algorithm>
#include <array>

Position::Position()
{
  
}

Position::Position(QString fenString) : fenString(fenString)
{
	processFenString(fenString);
}

Position::~Position()
{
}

void Position::processFenString(QString text)
{
	QChar const space{ ' ' };
	auto it{ std::find(text.begin(), text.end(), space) }; // start at first space

	getColorValue(it);
	getCastleValues(it, text, space);
	getEnPassantSquareValue(it, text, space);
	getHalfMoveClock(it, text, space);
	getFullMove(it, text);
}

void Position::getHalfMoveClock(QChar * &it, QString &text, const QChar &space)
{
	halfMoveClock = getIntFromString(it, text, space);
	std::advance(it, 1);
}

void Position::getFullMove(QChar * &it, QString &text)
{
	std::vector<QChar> qCharVector;
	std::copy(it, text.end(), back_inserter(qCharVector));
	std::advance(it, qCharVector.size());
	fullMove = (int)(qCharVector.at(0).digitValue()); // TODO: need to handle all digits.
	if (fullMove == -1)
		qDebug() << "Position::getIntFromString: error, no int found in string. " << qCharVector.at(0) << " size: " << qCharVector.size();
}

int Position::getIntFromString(QChar * &it, QString &text, const QChar &space)
{
	std::vector<QChar> qCharVector;
	std::copy(it, std::find(it, text.end(), space), back_inserter(qCharVector));
	std::advance(it, qCharVector.size());
	int retVal = (int)(qCharVector.at(0).digitValue());
	if(retVal == -1)
		qDebug() << "Position::getIntFromString: error, no int found in string." << qCharVector.size();
	return retVal;
}

void Position::getEnPassantSquareValue(QChar * &it, QString &text, QChar const &space)
{
	std::vector<QChar> enPassantVector;
	std::copy(it, std::find(it, text.end(), space), back_inserter(enPassantVector));
	std::advance(it, enPassantVector.size()+1);
	for (auto c : enPassantVector) {
		enPassantSquare.append(c);
	}
}

void Position::getColorValue(QChar * &it)
{
	activeColor = *(std::next(it));
	std::advance(it, 3);
}

void Position::getCastleValues(QChar * &it, QString &text, const QChar &space)
{
	std::vector<QChar> castleVector;
	std::copy_if(it, std::find(it, text.end(), space), back_inserter(castleVector),
		[](QChar const ch) {return (ch == 'K' || ch == 'k' || ch == 'Q' || ch == 'q'); });
	std::advance(it, castleVector.size() + 1);

	setCastleValues(castleVector);
}

void Position::setCastleValues(std::vector<QChar> &castleVector)
{
	whiteCanCastleKingside = (std::find(castleVector.begin(), castleVector.end(), 'K') == castleVector.end()) ? false : true;
	whiteCanCastleQueenside = (std::find(castleVector.begin(), castleVector.end(), 'Q') == castleVector.end()) ? false : true;
	blackCanCastleKingside = (std::find(castleVector.begin(), castleVector.end(), 'k') == castleVector.end()) ? false : true;
	blackCanCastleQueenside = (std::find(castleVector.begin(), castleVector.end(), 'q') == castleVector.end()) ? false : true;
}
