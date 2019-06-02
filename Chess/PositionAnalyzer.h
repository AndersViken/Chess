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
	PieceType getPieceTypeFromSquareID(std::vector<Piece*>& pieces, int const squareID);
	int getPieceValueSum();
	int getNumberOfValidMoves();
	int getUpdatedNumberOfValidMoves(Position & t_position, std::vector<Piece*>& t_pieces);

private:
	int pieceValueSum{};
	int numOfValidMoves{};

	int findPieceValueSum(std::vector<Piece*>& pieces);
	int findPieceValue(Piece*& piece);
};

