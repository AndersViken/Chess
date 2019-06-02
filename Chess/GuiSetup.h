#pragma once
#include <map>
#include <vector>

constexpr int squaresInARow		{ 8 };
constexpr int maxNumOfPieces	{ 32 };
constexpr int squarePixelSize	{ 80 };
constexpr int imageSize			{ squarePixelSize };
constexpr int lightColor		{ 0 };
constexpr int darkColor			{ 1 };
constexpr int boardWidth		{ squaresInARow *squarePixelSize };
constexpr int boardHeigth		{ boardWidth };
constexpr int boardStartX		{ 100 };
constexpr int boardStartY		{ 25 };
constexpr int whiteStartRow		{ 6 };
constexpr int blackStartRow		{ 1 };

constexpr int coordinateOffsetColTop{ squarePixelSize / 2 - squarePixelSize / 8 };
constexpr int coordinateOffsetColLeft{ squarePixelSize - squarePixelSize / 7 };
constexpr int coordinateOffsetRowTop{ squarePixelSize / 8 - squarePixelSize / 2 };
constexpr int coordinateOffsetRowLeft{ squarePixelSize / 15 };
constexpr int coordinateOffsetBoarder{ squarePixelSize / 40 };

constexpr int analysisTableOffsetFromBoardX{ 10 };
constexpr int analysisTableOffsetFromBoardY{ -coordinateOffsetBoarder };
constexpr int analysisTableWidth{ 300 };
constexpr int analysisTableHeight{ 150 };

constexpr int moveTableOffsetFromBoardX	{ analysisTableOffsetFromBoardX };
constexpr int moveTableOffsetFromAnalysisBoard{ -5 };
constexpr int moveTableOffsetFromBoardY	{ analysisTableOffsetFromBoardY + analysisTableHeight + moveTableOffsetFromAnalysisBoard };
constexpr int moveTableWidth			{ analysisTableWidth };
constexpr int moveTableHeight			{ 2*coordinateOffsetBoarder - analysisTableHeight + boardHeigth - moveTableOffsetFromAnalysisBoard};

constexpr int minWinSizeX{ boardStartX + boardWidth + moveTableOffsetFromBoardX + moveTableWidth + boardStartX};
constexpr int minWinSizeY{ boardStartY + boardWidth + boardStartY };

QString const darkColorString	{ "background - color: rgb(181, 135, 99);"	};
QString const lightColorString	{ "background - color: rgb(240, 218, 181);" };

