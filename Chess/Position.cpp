#include "Position.h"
#include <qdebug.h>
#include <algorithm>
#include <array>
#include <map>
#include <QString>
#include "PieceInfo.h"
Position::Position()
{
	gameFinished = false;
	gameStatus = GameStatus::notStarted;
}

Position::Position(QString fenString) : fenString(fenString)
{
	processFenString(fenString);
	gameFinished = false;
	gameStatus = GameStatus::notStarted;
}

Position::~Position()
{
}

void Position::updateActiveColor()
{
	activeColor = (activeColor == whiteColorChar) ? blackColorChar : whiteColorChar;
}

int Position::getActiveColorInt()
{
	if (activeColor == 'w') {
		return white;
	}
	else {
		return black;
	}
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
	qCharVector.push_back('\0');
	//fullMove = (int)(qCharVector.at(0).digitValue()); // TODO: need to handle all digits.

	QString s(&qCharVector[0]);
	fullMove = s.toInt();

}

int Position::findIntFromString(QChar * &it, QString &text, const QChar &space)
{
	std::vector<QChar> qCharVector;
	std::copy(it, std::find(it, text.end(), space), back_inserter(qCharVector));
	std::advance(it, qCharVector.size());
	QString numberString{};
	std::for_each(qCharVector.begin(), qCharVector.end(), [&numberString](QChar ch) {
		numberString.append(ch);
	});
	return numberString.toInt();
}

void Position::findEnPassantSquareValue(QChar * &it, QString &text, QChar const &space)
{
	std::vector<QChar> enPassantVector;
	std::copy(it, std::find(it, text.end(), space), back_inserter(enPassantVector));
	std::advance(it, enPassantVector.size()+1);
	std::for_each(enPassantVector.begin(), enPassantVector.end(), [&enPassantSquare = enPassantSquare](auto ch) {
		enPassantSquare.append(ch);
	});
}

void Position::findColorValue(QChar * &it)
{
	activeColor = *(it);
	std::advance(it, 2);
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

QString Position::createPiecePlacementFenString()
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

		if (piece != empty) {
			pushBackLetterInFenString(piece, pieceFenString);
		}

		if (index++ % 8 == 0) {
			if (emptySquares > 0) {
				pushBackNumberInFenString(emptySquares, pieceFenString);
			}
			pieceFenString.push_back('/');
		}
	}
	QString tempFenString{};
	
	std::copy(pieceFenString.begin(), pieceFenString.end(), std::back_inserter(tempFenString));

	//auto it = fenString.begin();
	//QChar space{ ' ' };
	 // first space is after piece letters
	// copy new piece placement to fenString
	//std::copy(std::find(it, fenString.end(), space), fenString.end(), std::back_inserter(tempFenString));


	return tempFenString;
}

QString Position::createCastleFenString()
{
	QChar whiteCastleKingSide  = 'K';
	QChar whiteCastleQueenSide = 'Q';
	QChar blackCastleKingSide  = 'k';
	QChar blackCastleQueenSide = 'q';
	QChar noCastling = '-';
	QString castleFenString{};
	if (getCastleLegalWhiteKingside()) {
		castleFenString.append(whiteCastleKingSide);
	}
	if (getCastleLegalWhiteQueenside()) {
		castleFenString.append(whiteCastleQueenSide);
	}
	if (getCastleLegalBlackKingside()) {
		castleFenString.append(blackCastleKingSide);
	}
	if (getCastleLegalBlackQueenside()) {
		castleFenString.append(blackCastleQueenSide);
	}
	if (std::size(castleFenString) == 0) {
		castleFenString.append(noCastling);
	}
	return castleFenString;
}

QString Position::createEnPassantFenString()
{
	//TODO: write this function
	QChar noEnPassant = '-';
	return noEnPassant;
}

QString Position::createHalfMoveFenString()
{
	return QString::number(halfMoveClock);
}

QString Position::createFullMoveFenString()
{
	return QString::number(fullMove);
}

void Position::insertNewMove(Move const &move)
{
	piecePlacement.at(move.toSquareId) = piecePlacement.at(move.fromSquareId);
	piecePlacement.at(move.fromSquareId) = 0;
}


void Position::setCastleValues(std::vector<QChar> &castleVector)
{
	castleLegalWhiteKingside = (std::find(castleVector.begin(), castleVector.end(), 'K') == castleVector.end()) ? false : true;
	castleLegalWhiteQueenside = (std::find(castleVector.begin(), castleVector.end(), 'Q') == castleVector.end()) ? false : true;
	castleLegalBlackKingside = (std::find(castleVector.begin(), castleVector.end(), 'k') == castleVector.end()) ? false : true;
	castleLegalBlackQueenside = (std::find(castleVector.begin(), castleVector.end(), 'q') == castleVector.end()) ? false : true;
}
