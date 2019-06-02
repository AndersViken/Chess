#pragma once
#include "Move.h"
#include "Position.h"
#include "Piece.h"
#include "PieceView.h"
#include "PositionAnalyzer.h"
struct specialMove
{
	int square;
	PieceType pieceOnSquareAfterMove;
	QString moveString;
};


enum class Direction { left, right, up, down, diagonalLeftUp, diagonalRightUp, diagonalLeftDown, diagonalRightDown};

struct Location {
	int row;
	int col;
};

class PositionController
{
public:
	PositionController();
	~PositionController();

	Position generateNewPosition(Move &mov, Position &oldPos);
	void updateFenString(Position &position);
	bool validateMove(Position & position, std::vector<Piece*> pieces, Move & move);
	bool validateMove(Position & newPosition, Position & oldPosition, Move move, PieceType pieceType, std::vector<specialMove>& specialMoves);
	//QChar getFenCharFromSquareID(QString fenString, int const squareID);
	bool validateBishopMove(Move const &move);
	bool validateRookMove(Move const &move);
	bool validateKnightMove(Move const &move);
	bool validateQueenMove(Move const &move);
	bool validateKingMove(Move const & move, Position &oldPosition, Position &newPosition, PieceType pieceType, std::vector<specialMove> &specialMoves);
	void moveRookWhenCastling(Position & newPosition, std::vector<specialMove>& specialMoves, int const rookOrigSquare, int const rookNewSquare, PieceType const rookType, QString castleString);
	bool validatePawnMove(Position &oldPosition, Move const &move, Position &newPosition, PieceType const piecetype);
	bool checkIfMovingToOwnColorPiece(Position &oldPosition, int const newSquareID, Position &newPosition);
	bool checkIfMovingToOwnColorPiece(Position & position, Location const activeLocation, Location const newLocation);
	bool checkIfMovingToOppositeColorPiece(Position &oldPosition, int const newSquareID, Position &newPosition);
	bool checkIfMovingToOppositeColorPiece(Position & position, int const activeSquareID, int const newSquareID);
	bool checkIfMovingToPiece(Position & oldPosition, int const newSquareID);
	bool checkIfMovingToPiece(Position & oldPosition, Location newLocation);
	bool checkIfKingAttacked(Position & position, std::vector<Piece*> pieces, int const activeColor);
	bool checkIfSquareAttacked(Position & position, std::vector<Piece*> pieces, int const activeColor, int const squareID);
	bool checkIfPiecesInSquares(Position const position, std::vector<int> const squares);
	int rowsMoved(Move move);
	int rowsMovedWithSign(Move move);
	int colsMoved(Move move);
	int colsMovedWithSign(Move move);
	int getColorFromType(PieceType pieceType, int const returnValueIfNotFound);

	std::vector<Move> getValidMoves(Position & position, std::vector<Piece*>& pieces, int const activeColor, bool isActualMove);

	void getValidMovesForPiece(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);

	void getValidMovesForPawn(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);

	void checkIfValidMovePawnForward(Location const & location, Location const & origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, bool isActualMove);

	void checkIfValidMovePawnDiagonal(Location const & location, Location const & origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, bool isActualMove);

	void getValidKnightMovesInDirections(Location const origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, std::vector<std::vector<Direction>> directionsVector, bool isActualMove);

	void getValidKingMovesInDirections(Location const origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, std::vector<Direction> directions, bool isActualMove);

	void getValidKingMovesCastling(Location const origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, int const pieceColor, bool canCastleKingSide, bool canCastleQueenSide, bool isActualMove);

	void getValidMovesInDirections(Location const origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, std::vector<Direction> directions, bool isActualMove);

	void getValidMovesInDirection(Location const origLocation, Position & position, std::vector<Piece*>& pieces, std::vector<Move>& moves, Direction direction, bool isActualMove);

	void checkIfValidMove(Location const & newLocation, Location const & origLocation, Position & position, std::vector<Piece*> pieces, std::vector<Move>& moves, bool & continueSearch, bool isActualMove);

	bool checkIfKingAttackedAfterMove(Position & position, std::vector<Piece*>& pieces, int const inactiveColor, Move move);

	
	void checkIfListOfSquaresAttackedAfterMove(Position & position, Move move, std::vector<int> squares);
	void getValidMovesForRook(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);

	void getValidMovesForBishop(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);

	void getValidMovesForKnight(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);

	void getValidMovesForQueen(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);

	void getValidMovesForKing(Position & position, std::vector<Piece*>& pieces, Piece *& piece, std::vector<Move>& moves, bool isActualMove);
	
	void getMoveString(Position & origPosition, Move & move, PieceType const pieceType);

	void getSquareString(int const & squareID, QString & squareString);

	QChar getPieceChar(PieceType const pieceType);

	bool checkIfSquare(Location location);
	Location getLocationFromSquareID(int const squareID);
	int getSquareIDFromLocation(Location location);
	void moveLocation(Location& location, Direction const direction);
	void handleValidMove(Position & position, std::vector<Move>& moves, std::vector<Piece*>& pieces, int const fromSquareID, int const toSquareID, MoveType moveType, bool isActualMove);
	void insertValidMove(Position & position, std::vector<Move>& moves, std::vector<Piece*>& pieces, int const fromSquareID, int const toSquareID, MoveType const moveType, bool const isActualMove, QString moveString);
	void handleValidMove(Position & position, std::vector<Move>& moves, std::vector<Piece*>& pieces, int const fromSquareID, int const toSquareID, bool isActualMove);
	void handleValidMove(Position & position, std::vector<Move>& moves, std::vector<Piece*>& pieces, Location const & origLocation, Location const & location, bool isActualMove);
	void handleValidMove(Position & position, std::vector<Move>& moves, std::vector<Piece*>& pieces, Location const & origLocation, Location const & location, MoveType moveType, bool isActualMove);

	void removePiece(std::vector<Piece*>& pieceVec, int squareID);

	MoveType checkIfPromotion(Location origLocation, Location newLocation);
	void handlePromotion(Position &position, std::vector<Move>& moves, std::vector<Piece*> &pieces, int const fromSquareID, int const toSquareID, MoveType moveType, int const pieceColor, bool const isActualMove, QString moveString);
	std::vector<PieceType> getPromotionOptions(int const pieceColor);
	
	void erasePieceFromVector(std::vector<Piece*>& t_pieces, int squareID);

	
	
};

