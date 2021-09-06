#include "MatchMaker.h"

#include "TextUIChess.h"
#include <iostream>


gameCondition matchTwoNNs(ChessGame* game, NeuralNetwork* black, NeuralNetwork* white)
{
	
	gameCondition retval = gameCondition::playing;
	player whoToPlay = player::black;
	while (retval == gameCondition::playing) {

		//std::cout << TextUIChess::getBoardString(game->getCurrentBoard(),TextUIChess::boardDisplayType::mayusMinus) << std::endl;
		

		whoToPlay = flipColor(whoToPlay);
		retval = makeMoveWithNN(game,(whoToPlay==player::white?white:black),whoToPlay);
	}
	
	return retval;
}

gameCondition makeMoveWithNN(ChessGame* game, NeuralNetwork* nn, player whoIsPlayed)
{
	gameCondition retval;
	auto possibleMoves = game->getPossibleBoards(&retval);
	if (retval != gameCondition::playing) {
		return retval;
	}


	double highestRating = 0;
	size_t chosenMove = 0;

	for (size_t i = 0; i < possibleMoves.size(); i++) {
		auto inputVec = getNumericRepresentationOfBoard(possibleMoves[i].first, whoIsPlayed);
		addExtraInputs(inputVec, game);
		double moveRating = nn->getResult(inputVec)[0];
		if (moveRating > highestRating) {
			highestRating = moveRating;
			chosenMove = i;
		}
		//std::cout << moveRating << std::endl;
	}
	game->setNext(possibleMoves[chosenMove]);
	return gameCondition::playing;
}
