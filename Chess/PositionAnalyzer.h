#pragma once
#include "Piece.h"
#include "Move.h"
#include "Position.h"

class PositionAnalyzer
{
public:
	PositionAnalyzer();
	~PositionAnalyzer();

	void analysePosition(Position & position, std::vector<Piece*>& pieces, int depthRemaining);
	int getPieceValueSum();

private:
	int pieceValueSum{};

	int findPieceValueSum(std::vector<Piece*>& pieces);
	int findPieceValue(Piece*& piece);
};

