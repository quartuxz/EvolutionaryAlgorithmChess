#pragma once

#include <array>
#include <vector>
#include <string>


enum class chessPiece : size_t {
	empty, blackPawn, blackRook, blackKnight, blackBishop,blackKing,blackQueen, whitePawn, whiteRook,whiteKnight,whiteBishop, whiteKing, whiteQueen
};

enum class colorlessChessPiece : size_t {
	pawn ,rook, knight, bishop, king, queen
};

typedef std::array<std::array<chessPiece,8>, 8> board;

typedef std::pair<size_t, size_t> boardCoords;


enum class player {
	neither, white, black
};



struct chessMove {
	enum moveTypes{
		doublePawn, castle, normal, capture, promotion, captureAndPromotion, enPassant
	}moveType;

	enum promotionTypes{
		notPromotion, toRook, toKnight, toBishop, toQueen
	}promotionTo = notPromotion;

	colorlessChessPiece initalPiece;

	boardCoords whereFrom;
	boardCoords whereTo;
	player who;
	chessMove(moveTypes p_moveType, boardCoords p_whereFrom, boardCoords p_whereTo, player p_who, colorlessChessPiece piece);

	std::string getStringRepresentation()const;
};

typedef std::pair<board, chessMove> boardAndPreviousMove;


class ChessGame
{
private:
	board m_current;
	//the back is the latest move
	std::vector<chessMove> m_moves;
	player m_whoToPlay = player::white;

	std::vector<boardAndPreviousMove> m_getPossibleMovesForBoard(const board &brd, player whoToPlay);

	bool m_checkWouldCaptureKing(board brd);

public:

	ChessGame();

	std::vector<boardAndPreviousMove> getPossibleBoards();
	void setNext(boardAndPreviousMove brdMove);

	board getCurrentBoard()const noexcept;
};

