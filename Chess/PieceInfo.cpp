#include <qstring.h>
#include <qlabel.h>
#include <map>
#include <PieceInfo.h>
namespace PieceTypes {
	enum PieceNames {
		empty = 0, whitePawn, whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing,
		blackPawn, blackRook, blackKnight, blackBishop, blackQueen, blackKing
	};
	enum PieceColors { white = 0, black = 1 };

	//rnbqkbnr / pppppppp / 8 / 8 / 8 / 8 / PPPPPPPP / RNBQKBNR

	std::vector<int> const pieceTypesVec = {
		whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing, whiteBishop, whiteKnight, whiteRook,
		whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn,
		blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn,
		blackRook, blackKnight, blackBishop, blackQueen, blackKing, blackBishop, blackKnight, blackRook
	};
}

std::vector<int> const getPieceTypes()
{
	return PieceTypes::pieceTypesVec;
}
