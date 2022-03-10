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


//TODO: could expand in the fuuture to allow for varying topologies for regeneration of NNs.(NEAT?)
class MatchMaker
{
private:
	std::vector<std::pair<NeuralNetwork*, size_t>> m_competitors;

	//8 seems to be the best option for a laptop with a 10th generation mobile multicore i7.
	size_t m_maxThreads = 8;
	size_t m_initialNNs;

	Topology m_top;
	randomizationStrategy m_initialRandStrat;

	

public:

	static bool verboseOutputAndTracking;

	//generates x amount of NNs all with the same topology passed
	MatchMaker(size_t initialNNs, Topology top = QEAC_DEFAULT_TOPOLOGY);
	//pass a vector with NNs that can have any topology BUT the same input and output amount of neurons
	//also pass a default topology to generate new ones in the regeneration phases of matchmaking/simulation.
	MatchMaker(std::vector<NeuralNetwork*> initialNNs, Topology defaultTop = QEAC_DEFAULT_TOPOLOGY);

	std::vector<NeuralNetwork*> getNNs();

	std::string getScoresStrings()const noexcept;

	void matchMake();

	void sortNNs();

	NeuralNetwork* getBest();

	void split();
	//creates double-1 amount of new NNs based on the ordered old ones, the one in the last position is not replicated
	//and instead a whole new one is generated.
	void regenerate();
	
	std::stringstream *serializeMatchMaker()const;

	~MatchMaker();


};

