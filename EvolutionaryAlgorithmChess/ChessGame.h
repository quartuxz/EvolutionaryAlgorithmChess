#pragma once

#include <array>
#include <vector>


enum class chessPiece : size_t {
	empty, blackPawn, blackRook, blackKnight, blackBishop,blackKing,blackQueen, whitePawn, whiteRook,whiteKnight,whiteBishop, whiteKing, whiteQueen
};

enum class colorlessChessPiece : size_t {
	rook, knight, bishop, king, queen
};

typedef std::array<std::array<chessPiece,8>, 8> board;

typedef std::pair<size_t, size_t> boardCoords;


enum class player {
	neither, white, black
};


class ChessGame
{
private:
	board m_current;
	player m_whoToPlay = player::white;
public:

	ChessGame();

	std::vector<board> getPossibleBoards();
	void setCurrentBoard(board brd);
};

