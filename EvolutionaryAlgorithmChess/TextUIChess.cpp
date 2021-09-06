#include "TextUIChess.h"
#include "MatchMaker.h"
#include <string>
#include <sstream>
#include <iostream>

TextUIChess::TextUIChess(boardDisplayType brdDsp):
	m_brdDsp(brdDsp)
{

}

TextUIChess::TextUIChess(NeuralNetwork* adversary,player playerColor, boardDisplayType brdDsp):
	m_adversary(adversary),
	m_playingAs(playerColor)
{
}

std::string TextUIChess::getBoardString(board brd, boardDisplayType brdDsp)
{
	std::string finalStr;
	for (auto x : brd) {
		for (auto piece : x) {
			switch (piece)
			{
			case chessPiece::empty:
				finalStr.push_back('.');
				break;
			case chessPiece::blackPawn:
				finalStr.push_back('p');
				break;
			case chessPiece::blackRook:
				finalStr.push_back('r');
				break;
			case chessPiece::blackKnight:
				finalStr.push_back('n');
				break;
			case chessPiece::blackBishop:
				finalStr.push_back('b');
				break;
			case chessPiece::blackKing:
				finalStr.push_back('k');
				break;
			case chessPiece::blackQueen:
				finalStr.push_back('q');
				break;
			case chessPiece::whitePawn:
				finalStr.push_back('P');
				break;
			case chessPiece::whiteRook:
				finalStr.push_back('R');
				break;
			case chessPiece::whiteKnight:
				finalStr.push_back('N');
				break;
			case chessPiece::whiteBishop:
				finalStr.push_back('B');
				break;
			case chessPiece::whiteKing:
				finalStr.push_back('K');
				break;
			case chessPiece::whiteQueen:
				finalStr.push_back('Q');
				break;
			default:
				break;
			}

		}
		finalStr.push_back('\n');
	}

	return finalStr;
}

void TextUIChess::showBoard()
{
	std::cout << getBoardString(m_game.getCurrentBoard(), m_brdDsp) << std::endl;
}

std::string TextUIChess::getShowMovesString(const std::vector<boardAndPreviousMove>& moves)
{
	std::stringstream ss;



	for (size_t i = 0; i < moves.size(); i++)
	{

		ss << i << ".) " << moves[i].second.getStringRepresentation() << '\n';
	}

	return ss.str();
}

void TextUIChess::showMoves()
{
	std::cout << getShowMovesString(m_game.getPossibleBoards()) << std::endl;
}

void TextUIChess::promptMove()
{
	if (m_playingAs == player::black) {
		makeMoveWithNN(&m_game,m_adversary,player::black);
	}
	int moveNumber = 0;
	std::cout << "enter move to make: ";
	std::cin >> moveNumber;

	m_game.setNext(m_game.getPossibleBoards()[moveNumber]);
	if (m_playingAs == player::white) {
		makeMoveWithNN(&m_game,m_adversary,player::black);
	}


}
