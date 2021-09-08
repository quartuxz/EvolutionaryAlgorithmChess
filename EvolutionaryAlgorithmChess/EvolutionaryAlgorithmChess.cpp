#include <iostream>
#include "TextUIChess.h"
#include "NeuralNetwork.h"
#include "MatchMaker.h"

int main()
{
    MatchMaker mm(20);

    mm.matchMake();


    //randomizationStrategy::seedEngine();

    ChessGame game;

    //std::cout << TextUIChess::getBoardString(game.getCurrentBoard(),TextUIChess::mayusMinus) << std::endl;
    //std::cout << TextUIChess::getBoardString(flipBoard(game.getCurrentBoard()),TextUIChess::mayusMinus) << std::endl;

    Topology top;
    top.push_back(834);
    top.push_back(416);
    top.push_back(208);
    top.push_back(104);
    top.push_back(52);
    top.push_back(26);
    top.push_back(13);
    top.push_back(4);
    top.push_back(2);
    top.push_back(1);

    randomizationStrategy strat;
    strat.individual.maxRangeBeforeTransform = 1;

    gameCondition temp = gameCondition::tieBy50Moves;

    while (temp == gameCondition::tieBy50Moves) {
        ChessGame *game1 = new ChessGame();
        NeuralNetwork white(top, strat);
        NeuralNetwork black(top, strat);

        temp = matchTwoNNs(game1, &black, &white);
        
        std::cout << getGameConditionString(temp) << std::endl;
        delete game1;
    }

    NeuralNetwork nn(top, strat);
    TextUIChess chessUI(&nn,player::white);
    while (true) {
        chessUI.showBoard();
        chessUI.showMoves();
        chessUI.promptMove();
    }

}
