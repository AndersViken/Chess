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

enum class GameStatus { notStarted, inProgress, stalemate, draw, whiteWon, blackWon };

class Position
{
public:
	Position();
	Position(QString fenString);
	~Position();

	QString getFenString() { return fenString; };
	void setFenString(QString t_fenString) { fenString = t_fenString; };
	QChar getActiveColor() { return activeColor; };
	void updateActiveColor();
	int getActiveColorInt();
	void setActiveColor(QChar t_activeColor) { activeColor = t_activeColor; };
	bool getCastleLegalWhiteKingside() { return castleLegalWhiteKingside; };
	bool getCastleLegalWhiteQueenside() { return castleLegalWhiteQueenside; };
	bool getCastleLegalBlackKingside() { return castleLegalBlackKingside; };
	bool getCastleLegalBlackQueenside() { return castleLegalBlackQueenside; };
	void setCastleLegalWhiteKingside(bool t_castleLegalWhiteKingside) {
		castleLegalWhiteKingside = t_castleLegalWhiteKingside;
	};
	void setCastleLegalWhiteQueenside(bool t_castleLegalWhiteQueenside) {
		castleLegalWhiteQueenside = t_castleLegalWhiteQueenside;
	};
	void setCastleLegalBlackKingside(bool t_castleLegalBlackKingside) {
		castleLegalBlackKingside = t_castleLegalBlackKingside;
	};
	void setCastleLegalBlackQueenside(bool t_castleLegalBlackQueenside) {
		castleLegalBlackQueenside = t_castleLegalBlackQueenside;
	};
	QString getEnPassantSquare() { return enPassantSquare; };
	int getHalfMoveClock() { return halfMoveClock; };
	int getFullMove() { return fullMove; };
	void setFullMove(int t_fullMove) { fullMove = t_fullMove; };
	void incrementFullMove() { fullMove++; };
	void resetHalfMoveClock() { halfMoveClock = 0; };
	void incrementHalfMoveClock() { halfMoveClock++; };
	std::vector<int> getPiecePlacement() { return piecePlacement; };
	void insertNewMove(Move const &move);
	//rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR w KQkq - 0 1
	QString createPiecePlacementFenString();
	QString createCastleFenString();
	QString createEnPassantFenString();
	QString createHalfMoveFenString();
	QString createFullMoveFenString();
	bool getActiveKingAttacked() { return activeKingAttacked; };
	void setActiveKingAttacked(bool t_activeKingAttacked) { activeKingAttacked = t_activeKingAttacked; };
	bool getGameFinished() { return gameFinished; };
	void setGameFinished(bool t_gameFinished) { gameFinished = t_gameFinished; };
	GameStatus getGameStatus() { return gameStatus; };
	void setGameStatus(GameStatus t_gameStatus) { gameStatus = t_gameStatus; };

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

	QString fenString;
	std::vector<int> piecePlacement;
	QChar activeColor;
	bool	castleLegalWhiteKingside;
	bool	castleLegalWhiteQueenside;
	bool	castleLegalBlackKingside;
	bool	castleLegalBlackQueenside;
	QString enPassantSquare;
	int		halfMoveClock;
	int		fullMove;
	bool	activeKingAttacked;
	bool	gameFinished;
	GameStatus gameStatus;
};

