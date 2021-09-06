#pragma once

#include <array>
#include <vector>
#include <string>





enum class gameCondition {
	playing, tieByStalemate, tieBy50Moves, tieByThreeFoldRepetition, whiteVictory, blackVictory
};

std::string getGameConditionString(gameCondition condition);

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


player flipColor(player who);

struct chessMove {
	enum moveTypes{
		doublePawn, castle, normal, capture, promotion, captureAndPromotion, enPassant, notAMove
	}moveType;

	enum promotionTypes{
		notPromotion, toRook, toKnight, toBishop, toQueen
	}promotionTo = notPromotion;

	colorlessChessPiece initalPiece;

	boardCoords whereFrom;
	boardCoords whereTo;
	player who;
	chessMove(moveTypes p_moveType, boardCoords p_whereFrom, boardCoords p_whereTo, player p_who, colorlessChessPiece piece);
	chessMove();
	std::string getStringRepresentation()const;
};

typedef std::pair<board, chessMove> boardAndPreviousMove;


class ChessGame
{
private:
	board m_current;

	std::vector<board> m_pastBoards;

	//the back is the latest move
	std::vector<chessMove> m_moves = {chessMove()};
	player m_whoToPlay = player::white;

	size_t m_movesWithoutCaptureOrPawnMove = 0;
	bool m_repeatedPosition = false;

	std::vector<boardAndPreviousMove> m_getPossibleMovesForBoard(const boardAndPreviousMove &brd, player whoToPlay);

	bool m_checkWouldCaptureKing(const boardAndPreviousMove &brd);

public:



	bool getRepeatedPosition()const noexcept;

	size_t getMovesWithoutCaptureOrPawnMove()const noexcept;




	ChessGame();

	std::vector<boardAndPreviousMove> getPossibleBoards(gameCondition *condition = nullptr);
	void setNext(boardAndPreviousMove brdMove);

	board getCurrentBoard()const noexcept;
};



board flipBoard(const board &brd);

std::vector<double> getNumericRepresentationOfBoard(board brd, player whoToPlay);


void addExtraInputs(std::vector<double> &pastInputs, ChessGame *game);




