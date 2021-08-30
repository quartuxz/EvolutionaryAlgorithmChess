#include <iostream>
#include "TextUIChess.h"

int main()
{
    TextUIChess chessUI;
    while (true) {
        chessUI.showBoard();
        chessUI.showMoves();
        chessUI.promptMove();
    }

}
