#include "TextUIChess.h"
#include <string>
#include <sstream>
#include <iostream>

TextUIChess::TextUIChess()
{

}

void TextUIChess::showBoard()
{
	std::string finalStr;
	auto brd = m_game.getCurrentBoard();
	for (auto x : brd) {
		for (auto piece: x) {
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
	
	std::cout << finalStr << std::endl;
}

void TextUIChess::showMoves()
{
	std::stringstream ss;

	auto x = m_game.getPossibleBoards();


	for (size_t i = 0; i < x.size(); i++)
	{

		ss << i << ".) " << x[i].second.getStringRepresentation() << '\n';
	}

	std::cout << ss.str() << std::endl;
}

void TextUIChess::promptMove()
{
	int moveNumber = 0;
	std::cout << "enter move to make: ";
	std::cin >> moveNumber;

	m_game.setNext(m_game.getPossibleBoards()[moveNumber]);
}
