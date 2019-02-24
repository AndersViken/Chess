#pragma once
#include "Position.h"
#include "Move.h"
#include <vector>
#include <map>

class Leaf
{
public:
	Leaf::Leaf(Move move, Position previousPosition);
	~Leaf();

	Move move;
	Position previousPosition;
	Position position;
	std::vector<std::map<Move, Leaf>> childLeafs;
	Position generateCurrentPosition(Move mov, Position pos);
};

