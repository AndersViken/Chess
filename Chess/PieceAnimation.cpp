#include "pieceAnimation.h"
#include <QPropertyAnimation>



PieceAnimation::PieceAnimation()
{
}


PieceAnimation::~PieceAnimation()
{
}

void PieceAnimation::movePiece(Piece* piece, QPoint newCoordinate)
{
	//int xPosOrig = piece.getCoordinate().rx();
	//int yPosOrig = piece.getCoordinate().ry();


	QPropertyAnimation* mMoveAnimation = new QPropertyAnimation(piece, "moveAnimation");
	mMoveAnimation->setDuration(nMillisecondsToMovePiece);
	mMoveAnimation->setStartValue(piece->getCoordinate());
	mMoveAnimation->setEndValue(newCoordinate);

	mMoveAnimation->start(QAbstractAnimation::DeleteWhenStopped);

	

}
