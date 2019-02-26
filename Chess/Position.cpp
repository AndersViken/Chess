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

	piecePlacement = {};
	for (QChar c : pieceFenString) {
		if (c.isDigit()) {
			pushBackEmptySquares(c.digitValue());
		}
		else if (c != '/') {
			pushBackPieceInSquare(c);
		}
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
	auto search{ pieceLetters.find(pieceChar) };
	if (search != pieceLetters.end()) {
		piecePlacement.push_back(search->second);
	}
	else {
		pushBackPieceInSquare('-');
	}

}

void Position::pushBackLetterInFenString(int pieceNumber, QString &inputString)
{
	auto search{ pieceLettersReverse.find(pieceNumber) };
	if (search != pieceLettersReverse.end()) {
		inputString.append(search->second);
	}
	else {
		qDebug() << "Position::pushBackLetterInFenString: error. wrong input to function.";
	}

}

void Position::pushBackNumberInFenString(int &number, QString &inputString)
{
	inputString.append(QString::number(number));
	number = 0;
}

QString Position::createFenStringFromPiecePlacement()
{
	QString pieceFenString{};
	int emptySquares{ 0 };
	int index = 1;
	for(int piece : piecePlacement) {
		if (piece == 0) {
			emptySquares++;
		}
		else if (emptySquares > 0) {
			pushBackNumberInFenString(emptySquares, pieceFenString);
		}
		if (emptySquares == 8) {
			pushBackNumberInFenString(emptySquares, pieceFenString);
		}

		pushBackLetterInFenString(piece, pieceFenString);

		if (index++ % 8 == 0) {
			if (emptySquares > 0) {
				pushBackNumberInFenString(emptySquares, pieceFenString);
			}
			pieceFenString.push_back('/');
		}
	}
	QString tempFenString{};
	
	std::copy(pieceFenString.begin(), pieceFenString.end(), std::back_inserter(tempFenString));

	auto it = fenString.begin();
	QChar space{ ' ' };
	 // first space is after piece letters
	std::copy(std::find(it, fenString.end(), space), fenString.end(), std::back_inserter(tempFenString));

	return tempFenString;
}

void Position::insertNewMove(Move const &move)
{
	piecePlacement.at(move.toSquareId) = piecePlacement.at(move.fromSquareId);
	piecePlacement.at(move.fromSquareId) = 0;

	fenString = createFenStringFromPiecePlacement();
}


void Position::setCastleValues(std::vector<QChar> &castleVector)
{
	whiteCanCastleKingside = (std::find(castleVector.begin(), castleVector.end(), 'K') == castleVector.end()) ? false : true;
	whiteCanCastleQueenside = (std::find(castleVector.begin(), castleVector.end(), 'Q') == castleVector.end()) ? false : true;
	blackCanCastleKingside = (std::find(castleVector.begin(), castleVector.end(), 'k') == castleVector.end()) ? false : true;
	blackCanCastleQueenside = (std::find(castleVector.begin(), castleVector.end(), 'q') == castleVector.end()) ? false : true;
}
