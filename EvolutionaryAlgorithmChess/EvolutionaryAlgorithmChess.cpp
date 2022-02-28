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
    int optionSelected = 1;
    while (optionSelected != 0) {
        std::cout << "1.) Play NN." << std::endl;
        std::cout << "2.) Load NN with Name." << std::endl;
        std::cout << "3.) Load NNs with Name+_Y to _Z number." << std::endl;
        std::cout << "4.) Generate Y-X random NNs with Name_X to Name_Y, if only one is generated then the name is just Name" << std::endl;
        std::cout << "5.) Do X Training generations simulations among NNs with Name+_Y to _Z number. Provide a naming convention, NNs are saved in order of best to worst, namingConvention+_ordinal." << std::endl;
        std::cout << "6.) Do X Training generations simulations among NNs selected. Provide a naming convention, NNs are saved in order of best to worst, namingConvention+_ordinal." << std::endl;
        std::cout << "7.) Save to files NNs with Name+_Y to _Z number." << std::endl;
        std::cout << "8.) Save to file NN with Name." << std::endl;
        std::cout << "9.) Save to files selected NNs." << std::endl;
        std::cout << "10) Show NN names." << std::endl;
        std::cout << "0.) Exit." << std::endl;
        std::cin >> optionSelected;

        auto doMM = [&](const std::vector<NeuralNetwork*> selectedNNs, const std::string &namingConvention, unsigned int generations) {            
            MatchMaker mm(selectedNNs);
            for (size_t i = 0; i < generations; i++) {
                if (i != 0) {
                    mm.split();
                    mm.regenerate();
                }
                mm.matchMake();
                mm.sortNNs();

                std::cout << i << " generations completed!";
            }


            auto result = mm.getNNs();

            for (size_t i = 0; i < result.size(); i++)
            {
                std::stringstream finalName;
                finalName << namingConvention << "_" << i;
                NNs.addNN(finalName.str(), result[i]);
            }
        };

        auto selectNNRange = [&](unsigned int rangeStart, unsigned int rangeEnd, const std::string& name, std::vector<NeuralNetwork*> &selectedNNs ) {            
            for (size_t i = rangeStart; i <= rangeEnd; i++)
            {
                std::stringstream ss;
                ss << name << "_" << i;
                selectedNNs.push_back(NNs.getNN(ss.str()));
            }
        };


        auto selectNNsIndividually = [&](std::vector<NeuralNetwork*> &selectedNNs, std::vector<std::string> *selectedNNsNames = nullptr) {

            std::string option;
            while (true) {
                int optionToInt = 0;
                std::cout << NNs.showNNs();
                std::cout << "enter option or stop to start simulating: ";
                std::cin >> option;
                if (option == "stop") {
                    break;
                }
                optionToInt = std::atoi(option.c_str());
                selectedNNs.push_back(NNs.getNN(optionToInt));
                if (selectedNNsNames != nullptr) {
                    selectedNNsNames->push_back(NNs.getNNName(optionToInt));
                }
            }
        };
        
        auto saveNN = [&](const std::string& finalName) {
            const std::string& serializedNN = NNs.getNN(finalName)->serialize();
            std::ofstream savefile(finalName, std::ios::out | std::ios::trunc);
            savefile << serializedNN;
        };

        switch (optionSelected)
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
            unsigned int numberStart = 0;
            unsigned int numberEnd = 0;
            
            std::cout << "enter NN Name: ";
            std::cin >> NNName;

            std::cout << "enter first range for NNs numbering: ";
            std::cin >> numberStart;

            std::cout << "enter last range for NNs numbering: ";
            std::cin >> numberEnd;

            if (numberStart == numberEnd) {
                NNs.addNN(NNName, new NeuralNetwork(QEAC_DEFAULT_TOPOLOGY,randomizationStrategy()));
            }
            else {
                for (size_t i = numberStart; i <= numberEnd; i++)
                {
                    std::stringstream finalName;
                    finalName << NNName << "_" << i;
                    NNs.addNN(finalName.str(), new NeuralNetwork(QEAC_DEFAULT_TOPOLOGY, randomizationStrategy()));
                }
            }
        }
            break;
        case 5:
        {
            std::vector<NeuralNetwork*> selectedNNs;
            unsigned int generations, rangeStart, rangeEnd;
            std::string name, namingConvention, verboseAnswer;
            std::cout << "enter generations: ";
            std::cin >> generations;
            std::cout << std::endl << "enter name: ";
            std::cin >> name;
            std::cout << std::endl << "enter start of range: ";
            std::cin >> rangeStart;
            std::cout << std::endl << "enter end of range: ";
            std::cin >> rangeEnd;
            std::cout << "enter naming convention: ";
            std::cin >> namingConvention;

            selectNNRange(rangeStart, rangeEnd, name, selectedNNs);

            doMM(selectedNNs,namingConvention,generations);
        }
            break;
        case 6:
        {
            std::vector<NeuralNetwork*> selectedNNs;
            unsigned int generations;
            std::string name, namingConvention;
            std::cout << "enter generations: ";
            std::cin >> generations;
            std::cout << "enter naming convention: ";
            std::cin >> namingConvention;

            selectNNsIndividually(selectedNNs);

            doMM(selectedNNs, namingConvention, generations);
        }
        break;
        case 7:
        {
            std::vector<NeuralNetwork*> selectedNNs;
            unsigned int rangeStart, rangeEnd;
            std::string name;
            std::cout << std::endl << "enter name: ";
            std::cin >> name;
            std::cout << std::endl << "enter start of range: ";
            std::cin >> rangeStart;
            std::cout << std::endl << "enter end of range: ";
            std::cin >> rangeEnd;
            selectNNRange(rangeStart,rangeEnd,name,selectedNNs);
            
            for (size_t i = rangeStart; i <= rangeEnd; i++)
            {
                std::stringstream finalName;
                finalName << name << "_" << i;
                saveNN(finalName.str());
            }
        }
        break;
        case 8:
        {
            std::string name;
            std::cout << "enter name: ";
            std::cin >> name;
            saveNN(name);
        }
        break;
        case 9:
        {
            std::vector<NeuralNetwork*> selectedNNs;
            std::vector<std::string> selectNNNames;
            selectNNsIndividually(selectedNNs);
        }
        break;
        case 10:
        {
            std::cout << NNs.showNNs() << std::endl;
        }
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
