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



struct chessMove {
	enum moveTypes{
		doublePawn, castle, normal, capture, promotion
	}moveType;
	boardCoords whereFrom;
	boardCoords whereTo;
	player who;
	chessMove(moveTypes p_moveType, boardCoords p_whereFrom, boardCoords p_whereTo, player p_who);
};

typedef std::pair<board, chessMove> boardAndPreviousMove;


class ChessGame
{
private:
	board m_current;
	//the back is the latest move
	std::vector<chessMove> m_moves;
	player m_whoToPlay = player::white;
public:

	ChessGame();

	std::vector<board> getPossibleBoards();
	void setCurrentBoard(board brd);
};

