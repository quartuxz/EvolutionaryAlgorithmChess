#pragma once
#include "ChessGame.h"
#include "NeuralNetwork.h"
#include <tuple>
#include <thread>
#include <queue>
#include <sstream>

#define QEAC_DEFAULT_TOPOLOGY {834,416,208,104,52,26,13,4,2,1}

gameCondition matchTwoNNs(ChessGame *game, NeuralNetwork* black, NeuralNetwork* white);


gameCondition makeMoveWithNN(ChessGame *game, NeuralNetwork *nn, player whoIsPlayed);


class MatchMaker
{
private:
	std::vector<std::pair<NeuralNetwork*, size_t>> m_competitors;


	size_t m_maxThreads = 4;
	size_t m_initialNNs;

	Topology m_top;
	randomizationStrategy m_initialRandStrat;

	

public:

	static bool verboseOutputAndTracking;

	MatchMaker(size_t initialNNs, Topology top = QEAC_DEFAULT_TOPOLOGY);
	MatchMaker(std::vector<NeuralNetwork*> initialNNs);

	std::vector<NeuralNetwork*> getNNs();

	std::string getScoresStrings()const noexcept;

	void matchMake();

	void sortNNs();

	NeuralNetwork* getBest();

	void split();

	void regenerate();
	
	std::stringstream *serializeMatchMaker()const;

	~MatchMaker();


};

