#include <algorithm>
#include <map>
#include "PieceInfo.h"
#include "PositionAnalyzer.h"
#include "PositionController.h"

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

void PositionAnalyzer::analysePosition(Position &position, std::vector<Move> validMoves, std::vector<Piece*>& pieces, int depthRemaining)
{
	// Add some recursive function here. This is where the fun code will be.
	// (for every legal move, analysePosition)
	// should of course jump of after a "depth", to avoid infinite recursion
	// should maybe use the tree/leaf structure here, to store all the paths, along with a position "score".
	// should in future probably keep this tree between each move, to avoid thinking on same position twice.

	PositionController positionController{};
	
	
	positionController.getValidMoves(position, pieces);
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

