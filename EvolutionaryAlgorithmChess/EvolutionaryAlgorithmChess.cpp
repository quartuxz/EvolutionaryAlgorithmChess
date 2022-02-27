#include <iostream>
#include <sstream>
#include <fstream>
#include "TextUIChess.h"
#include "NeuralNetwork.h"
#include "MatchMaker.h"
#include "NNManager.h"
#include <map>

int main()
{
    NNManager NNs;
    int number = 0;
    while (number != 0) {
        std::cout << "1.) Play NN." << std::endl;
        std::cout << "2.) Load NN with Name." << std::endl;
        std::cout << "3.) Load NNs with Name+_Y to _Z number." << std::endl;
        std::cout << "4.) Generate X random NNs with Name_X, if only one is generated then the name is just Name" << std::endl;
        std::cout << "5.) Do X Training generations simulations among NNs with Name+_Y to _Z number. Overwrite." << std::endl;
        std::cout << "6.) Save to file NNs with Name+_Y to _Z number." << std::endl;
        std::cout << "7.) Save to file NN with Name." << std::endl;
        std::cout << "8.) Save to file NN with." << std::endl;
        std::cout << "10) Show NN names." << std::endl;
        std::cout << "0.) Exit." << std::endl;
        std::cin >> number;
        switch (number)
        {
        case 1:
        {
            std::string NNname;
            std::cout << "enter NN name to play: ";
            std::cin >> NNname;
            std::cout << std::endl;
            TextUIChess uiChess(NNs.getNN(NNname), player::white);
            while (true) {
                uiChess.showBoard();
                uiChess.showMoves();
                if (uiChess.getGameCondition() != gameCondition::playing) {
                    std::cout << getGameConditionString(uiChess.getGameCondition()) << std::endl;
                    break;
                }
                uiChess.promptMove();
            }
        }
        case 2:
        {
            std::string NNname;
            std::cout << "enter NN name to load: ";
            std::cin >> NNname;


            std::ifstream NNFile;
            NNFile.open(NNname);
            
            std::stringstream buffer;
            buffer << NNFile.rdbuf();

            NNs.addNN(NNname, buffer.str());

            std::cout << std::endl << NNname << " was read successfully!";
            NNFile.close();
        }
            break;
        case 3:
        {
            std::string NNName;
            unsigned int rangeStart, rangeEnd;
            std::cout << "enter NN name to load: ";
            std::cin >> NNName;
            std::cout << "enter first number in range: ";
            std::cin >> rangeStart;
            std::cout << "enter second number in range: ";
            std::cin >> rangeEnd;


            for (size_t i = rangeStart; i <= rangeEnd; i++)
            {
                std::stringstream ss;
                ss << NNName << "_" << i;

                std::ifstream NNFile;
                NNFile.open(NNName);
                std::stringstream buffer;
                buffer << NNFile.rdbuf();

                NNs.addNN(ss.str(), buffer.str());

                NNFile.close();
            }

        }
            break;
        case 4:
        {
            std::string NNName;
            unsigned int numberOfNNs;
            
            std::cout << "enter NN Name: ";
            std::cin >> NNName;

            std::cout << "enter number of NNs";
            std::cin >> numberOfNNs;

            if (number == 0) {
                NNs.addNN(NNName, NeuralNetwork());
            }
            for (size_t i = 0; i < numberOfNNs; i++)
            {

            }
        }
            break;
        default:
            break;
        }
    }


    return 0;
    MatchMaker mm(4);




    //20 generations are simulated
    for (size_t i = 0; i < 1; i++) {


        mm.matchMake();
        std::cout << mm.getScoresStrings() << std::endl;
        mm.sortNNs();
        std::cout << mm.getScoresStrings() << std::endl;
        mm.split();
        std::cout << mm.getScoresStrings() << std::endl;
        mm.regenerate();
        std::cout << mm.getScoresStrings() << std::endl;


        std::stringstream ss;
        ss << "generation_" << i << "_save.txt";

        std::ofstream myfile;
        myfile.open(ss.str(), std::ios::trunc);
        std::stringstream* ssOut = mm.serializeMatchMaker();
        myfile << ssOut->str() << std::endl;
        myfile.close();
        delete ssOut;
    }

    mm.matchMake();
    mm.sortNNs();

    TextUIChess uiChess(mm.getBest(),player::white);
    while (true) {
        uiChess.showBoard();
        uiChess.showMoves();
        uiChess.promptMove();
    }



    return 0;
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
