#pragma once
#include "Piece.h"

class PositionAnalyzer
{
public:
	PositionAnalyzer();
	~PositionAnalyzer();

	void analysePosition(std::vector<Piece*>& pieces);
	int getPieceValueSum();

private:
	int pieceValueSum{};

	int findPieceValueSum(std::vector<Piece*>& pieces);
	int findPieceValue(Piece*& piece);
};

