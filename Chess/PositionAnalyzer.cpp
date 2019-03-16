#include <algorithm>
#include <map>
#include "PieceInfo.h"
#include "PositionAnalyzer.h"

std::map<int, int> pieceValues{
	{ whitePawn,	1	},
	{ whiteRook,	5	},
	{ whiteKnight,	3	},
	{ whiteBishop,	3	},
	{ whiteQueen,	9	},
	{ whiteKing,	1000},
	{ blackPawn,	1	},
	{ blackRook,	5	},
	{ blackKnight,	3	},
	{ blackBishop,	3	},
	{ blackQueen,	9	},
	{ blackKing,	1000},
};

PositionAnalyzer::PositionAnalyzer()
{
}


PositionAnalyzer::~PositionAnalyzer()
{
}

void PositionAnalyzer::analysePosition(std::vector<Piece*>& pieces)
{
	pieceValueSum = findPieceValueSum(pieces);
}

int PositionAnalyzer::findPieceValueSum(std::vector<Piece*>& pieces)
{
	int whiteSum{};
	int blackSum{};

	std::for_each(pieces.begin(), pieces.end(), [this,&whiteSum, &blackSum](Piece*& piece) {
		if (piece->getColor() == white) {
			whiteSum += findPieceValue(piece);
		}
		else if (piece->getColor() == black) {
			blackSum += findPieceValue(piece);
		}
	});

	return (whiteSum - blackSum);
}

int PositionAnalyzer::findPieceValue(Piece*& piece)
{
	int pieceValue{};
	auto pieceValueSearch{ pieceValues.find(piece->getPieceType()) };
	if (pieceValueSearch != pieceValues.end()) {
		pieceValue = pieceValueSearch->second;
	}
	return pieceValue;
}

int PositionAnalyzer::getPieceValueSum()
{
	return pieceValueSum;
}

