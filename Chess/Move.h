#pragma once
#include <qstring.h>
class Move
{
public:
	Move();
	~Move();
	int fromSquareId;
	int toSquareId;
	QString moveString;

};

