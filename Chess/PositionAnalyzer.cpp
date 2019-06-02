#include <algorithm>
#include <map>
#include <qdebug.h>
#include "PieceInfo.h"
#include "PositionAnalyzer.h"
#include "PositionController.h"

std::map<PieceType, int> pieceValues{
	{ PieceType::whitePawn,	1	},
	{ PieceType::whiteRook,	5	},
	{ PieceType::whiteKnight,	3	},
	{ PieceType::whiteBishop,	3	},
	{ PieceType::whiteQueen,	9	},
	{ PieceType::whiteKing,	1000},
	{ PieceType::blackPawn,	1	},
	{ PieceType::blackRook,	5	},
	{ PieceType::blackKnight,	3	},
	{ PieceType::blackBishop,	3	},
	{ PieceType::blackQueen,	9	},
	{ PieceType::blackKing,	1000},
};

PositionAnalyzer::PositionAnalyzer()
{
}


PositionAnalyzer::~PositionAnalyzer()
{
}

void PositionAnalyzer::analysePosition(Position &position, std::vector<Piece*>& pieces, int depthRemaining)
{
	// Add some recursive function here. This is where the fun code will be.
	// (for every legal move, analysePosition)
	// should of course jump out after a "depth", to avoid infinite recursion
	// should maybe use the tree/leaf structure here, to store all the paths, along with a position "score".
	// should in future probably keep this tree between each move, to avoid thinking on same position twice.

	PositionController positionController{};
	
	std::vector<Move> validMoves{ positionController.getValidMoves(position, pieces, position.getActiveColorInt(), true) };
	int error{};
	std::for_each(validMoves.begin(), validMoves.end(), [&](Move &move) {
		if (move.fromSquareId < 0 || move.fromSquareId>63 || move.toSquareId < 0 || move.toSquareId>63) {
			error++;
		}
	});

	//bool whiteKingAttacked{ positionController.checkIfKingAttacked(position, pieces, black) };
	//bool blackKingAttacked{ positionController.checkIfKingAttacked(position, pieces, white) };
	qDebug() << "fullmove: " << position.getFullMove();
	//qDebug() << "whiteKingAttacked: " << whiteKingAttacked;
	//qDebug() << "blackKingAttacked: " << blackKingAttacked;

	constexpr bool DEBUG_MODE{ true };
	if constexpr (DEBUG_MODE)
	{
		std::for_each(validMoves.begin(), validMoves.end(), [&positionController, &pieces, &position, this](Move &move) {
			PieceType const pieceType{ getPieceTypeFromSquareID(pieces, move.fromSquareId) };
			positionController.getMoveString(position, move, pieceType);
		});
	}

	pieceValueSum = findPieceValueSum(pieces);
	numOfValidMoves = validMoves.size();
}

PieceType PositionAnalyzer::getPieceTypeFromSquareID(std::vector<Piece*>& pieces, int const squareID)
{
	auto it = std::find_if(pieces.begin(), pieces.end(),
		[&squareID](Piece *piece) { return (piece->getSquareID()==squareID); });
	if (it != pieces.end() && *it) {
		Piece* foundPiece = static_cast<Piece*>(*it);
		return foundPiece->getPieceType();
	}
	else {
		return PieceType::empty;
	}
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

int PositionAnalyzer::getNumberOfValidMoves()
{
	return numOfValidMoves;
}

int PositionAnalyzer::getUpdatedNumberOfValidMoves(Position &t_position, std::vector<Piece*>& t_pieces)
{
	PositionController positionController;
	std::vector<Move> validMoves{ positionController.getValidMoves(t_position, t_pieces, t_position.getActiveColorInt(), true) };
	return validMoves.size();
}

