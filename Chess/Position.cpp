#include "Position.h"
#include <qdebug.h>
#include <algorithm>
#include <array>
#include <map>
#include <QString>
#include "PieceInfo.h"
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
	auto it{ text.begin() };

	findPiecePlacement(it, text, space);
	findColorValue(it);
	findCastleValues(it, text, space);
	findEnPassantSquareValue(it, text, space);
	getHalfMoveClock(it, text, space);
	findFullMove(it, text);
}

void Position::getHalfMoveClock(QChar * &it, QString &text, const QChar &space)
{
	halfMoveClock = findIntFromString(it, text, space);
	std::advance(it, 1);
}

void Position::findFullMove(QChar * &it, QString &text)
{
	std::vector<QChar> qCharVector;
	std::copy(it, text.end(), back_inserter(qCharVector));
	std::advance(it, qCharVector.size());
	fullMove = (int)(qCharVector.at(0).digitValue()); // TODO: need to handle all digits.
	if (fullMove == -1)
		qDebug() << "Position::getIntFromString: error, no int found in string. " << qCharVector.at(0) << " size: " << qCharVector.size();
}

int Position::findIntFromString(QChar * &it, QString &text, const QChar &space)
{
	std::vector<QChar> qCharVector;
	std::copy(it, std::find(it, text.end(), space), back_inserter(qCharVector));
	std::advance(it, qCharVector.size());
	QString numberString{};
	for (QChar c : qCharVector) {
		numberString.append(c);
	}
	return numberString.toInt();
}

void Position::findEnPassantSquareValue(QChar * &it, QString &text, QChar const &space)
{
	std::vector<QChar> enPassantVector;
	std::copy(it, std::find(it, text.end(), space), back_inserter(enPassantVector));
	std::advance(it, enPassantVector.size()+1);
	for (auto c : enPassantVector) {
		enPassantSquare.append(c);
	}
}

void Position::findColorValue(QChar * &it)
{
	activeColor = *(std::next(it));
	std::advance(it, 3);
}

void Position::findCastleValues(QChar * &it, QString &text, const QChar &space)
{
	std::vector<QChar> castleVector;
	std::copy_if(it, std::find(it, text.end(), space), back_inserter(castleVector),
		[](QChar const ch) {return (ch == 'K' || ch == 'k' || ch == 'Q' || ch == 'q'); });
	std::advance(it, castleVector.size() + 1);

	setCastleValues(castleVector);
}

void Position::findPiecePlacement(QChar *& it, QString & text, const QChar & space)
{
	std::vector<QChar> pieceFenString;
	std::copy(it, std::find(it, text.end(), space), back_inserter(pieceFenString));
	std::advance(it, pieceFenString.size() + 1);

	int i = 0;

	piecePlacement = {};
	for (QChar c : pieceFenString) {
		if (c.isDigit()) {
			pushBackEmptySquares(c.digitValue());
		}
		else if (c != '/') {
			pushBackPieceInSquare(c);
		}

		i++;
	}
	
}
void Position::pushBackEmptySquares(int n)
{
	while (n--) {
		pushBackPieceInSquare('-');
	}
}

void Position::pushBackPieceInSquare(QChar const pieceChar)
{
	auto search = pieceLetters.find(pieceChar);
	if (search != pieceLetters.end()) {
		piecePlacement.push_back(search->second);
	}
	else {
		pushBackPieceInSquare('-');
	}

}


void Position::setCastleValues(std::vector<QChar> &castleVector)
{
	whiteCanCastleKingside = (std::find(castleVector.begin(), castleVector.end(), 'K') == castleVector.end()) ? false : true;
	whiteCanCastleQueenside = (std::find(castleVector.begin(), castleVector.end(), 'Q') == castleVector.end()) ? false : true;
	blackCanCastleKingside = (std::find(castleVector.begin(), castleVector.end(), 'k') == castleVector.end()) ? false : true;
	blackCanCastleQueenside = (std::find(castleVector.begin(), castleVector.end(), 'q') == castleVector.end()) ? false : true;
}
