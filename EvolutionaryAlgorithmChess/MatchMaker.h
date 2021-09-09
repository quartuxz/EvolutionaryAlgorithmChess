#pragma once
#include "ChessGame.h"
#include "NeuralNetwork.h"
#include <tuple>
#include <thread>
#include <queue>



gameCondition matchTwoNNs(ChessGame *game, NeuralNetwork* black, NeuralNetwork* white);


gameCondition makeMoveWithNN(ChessGame *game, NeuralNetwork *nn, player whoIsPlayed);


class MatchMaker
{
private:
	std::vector<std::pair<NeuralNetwork*, size_t>> m_competitors;


	size_t m_maxThreads = 4;
	size_t m_initialNNs;
public:

	MatchMaker(size_t initialNNs, Topology top = {834,416,208,104,52,26,13,4,2,1});

	void matchMake();

	void sortNNs();

	NeuralNetwork* getBest();

	void split();

	void regenerate();
	
	std::string serializeMatchMaker()const;

	~MatchMaker();


};

