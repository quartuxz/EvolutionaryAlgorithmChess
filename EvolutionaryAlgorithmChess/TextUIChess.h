#pragma once
#include "ChessGame.h"

class TextUIChess
{
private:
	ChessGame m_game;

public:
	TextUIChess();
	
	void showBoard();

	void showMoves();

	void promptMove();

};

