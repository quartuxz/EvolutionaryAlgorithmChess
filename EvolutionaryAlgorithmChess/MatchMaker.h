#pragma once
#include "ChessGame.h"
#include "NeuralNetwork.h"



gameCondition matchTwoNNs(ChessGame *game, NeuralNetwork* black, NeuralNetwork* white);


gameCondition makeMoveWithNN(ChessGame *game, NeuralNetwork *nn, player whoIsPlayed);


class MatchMaker
{
	
};

