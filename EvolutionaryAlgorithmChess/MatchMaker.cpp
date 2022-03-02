#include "MatchMaker.h"

#include "TextUIChess.h"
#include <iostream>

bool MatchMaker::verboseOutputAndTracking = false;


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

MatchMaker::MatchMaker(size_t initialNNs, Topology top):
	m_initialNNs(initialNNs),
	m_top(top),
	m_initialRandStrat()
{
	for (size_t i = 0; i < initialNNs; i++) {
		m_competitors.push_back(std::make_pair(new NeuralNetwork(top, m_initialRandStrat),0));
	}

}

MatchMaker::MatchMaker(std::vector<NeuralNetwork*> initialNNs):
	m_initialNNs(initialNNs.size()),
	m_initialRandStrat()
{
	for (size_t i = 0; i < initialNNs.size(); i++)
	{
		m_competitors.push_back(std::make_pair(initialNNs[i],0));
	}
}

#include <iostream>
#include <stack>



void addScores(std::vector<std::pair<NeuralNetwork*, size_t>>& m_competitors, size_t blackIndex, size_t whiteIndex, gameCondition cond) {
	if (cond == gameCondition::blackVictory) {
		m_competitors[blackIndex].second += 2;
	}
	else if (cond == gameCondition::whiteVictory) {
		m_competitors[whiteIndex].second += 2;
	}
	else {
		m_competitors[blackIndex].second += 1;
		m_competitors[whiteIndex].second += 1;
	}
}

void matchMakeThreadedOnce(size_t blackIndex, size_t whiteIndex, std::vector<std::pair<NeuralNetwork*, size_t>>& m_competitors, std::mutex &matchesLock) {
	
	matchesLock.lock();
	auto blackNN = m_competitors[blackIndex].first;
	auto whiteNN = m_competitors[whiteIndex].first;
	matchesLock.unlock();
	
	ChessGame* game = new ChessGame();

	gameCondition cond;

	cond = matchTwoNNs(game, blackNN, whiteNN);
	if (MatchMaker::verboseOutputAndTracking) {
		std::cout << std::endl << getGameConditionString(cond);
	}
	

	matchesLock.lock();
	addScores(m_competitors,blackIndex,whiteIndex,cond);
	matchesLock.unlock();


	delete game;
}


void matchMakeThreaded(std::stack<std::pair<size_t, size_t>>& matches, std::vector<std::pair<NeuralNetwork*, size_t>> &m_competitors, std::mutex &matchesLock, size_t i) {
	
	//std::cout << "-";
	while (true) {
		size_t blackIndex, whiteIndex;
		matchesLock.lock();
		if (matches.empty()) {
			
			matchesLock.unlock();
			break;
		}
		else {
			auto thisMatch = matches.top();
			blackIndex = thisMatch.first;
			whiteIndex = thisMatch.second;
			matches.pop();
		}
		matchesLock.unlock();



		//std::cout << " " << i << " " << std::endl;

		//the most computationally intensive operation, takes between 2-5 seconds @4ghz. Possibly can be optimized.
		gameCondition result;
		matchMakeThreadedOnce(blackIndex,whiteIndex, m_competitors,matchesLock);
		//std::cout << "lol" << std::endl;
	}
}


#include <algorithm>
#include <chrono>

#define START_CHRONO auto start = std::chrono::high_resolution_clock::now();
#define END_CHRONO_LOG auto finish = std::chrono::high_resolution_clock::now();\
						std::cout << std::endl;\
						std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << std::endl;

std::string MatchMaker::getScoresStrings() const noexcept
{
	std::stringstream ss;
	ss << "scores: " << std::endl;
	for (auto x : m_competitors)
	{
		ss << x.second << std::endl;
	}
	return ss.str();
}

std::vector<NeuralNetwork*> MatchMaker::getNNs()
{
	std::vector<NeuralNetwork*> retval;
	for (auto x : m_competitors)
	{
		retval.push_back(x.first);
	}
	return retval;
}

void MatchMaker::matchMake()
{

	using milli = std::chrono::milliseconds;


	//first is black and second is white
	std::vector<std::pair<size_t, size_t>> unorderedMatches;
	std::vector<std::thread*> workers;

	std::mutex matchesLock;

	//matches are made, black is first and second is white, care is taken to not match NNs against themselves.
	for (size_t i = 0; i < m_initialNNs; i++)
	{
		for (size_t o = i; o < m_initialNNs; o++) {

			if (i != o) {
				for (size_t x = 0; x < 2; x++) {




					auto black = x == 0 ? i : o;
					auto white = x == 0 ? o : i;


					unorderedMatches.push_back(std::make_pair(black, white));


				}
			}

		}
	}


	std::stack<std::pair<size_t, size_t>> matches;
	std::vector<size_t> alreadySelectedNNsIndices;


	//first we order the matches so that the order means every subsequent match is guaranteed to have
	//different NNs than the most previous amount of matches possible
	while (!unorderedMatches.empty()) {
		bool foundOne = false;
		for (size_t i = 0; i < unorderedMatches.size(); i++) {
			auto vecContains = [&alreadySelectedNNsIndices](size_t index) {return std::find(alreadySelectedNNsIndices.begin(), alreadySelectedNNsIndices.end(), index) != alreadySelectedNNsIndices.end(); };
			if ( !vecContains(unorderedMatches[i].first) && !vecContains(unorderedMatches[i].second)) {
				alreadySelectedNNsIndices.push_back(unorderedMatches[i].first);
				alreadySelectedNNsIndices.push_back(unorderedMatches[i].second);

				matches.push(unorderedMatches[i]);


				//std::cout << unorderedMatches[i].first << "; " << unorderedMatches[i].second << std::endl;

				unorderedMatches.erase(unorderedMatches.begin() + i);
				foundOne = true;
				break;
			}
		}
		if (!foundOne) {
			alreadySelectedNNsIndices.clear();


		}
	}

	START_CHRONO

	for (size_t o = 0; o < m_maxThreads; o++) {
		//matchMakeThread(matches,m_competitors,matchesLock,competitorsLock);
		workers.push_back(new std::thread([&, o]() {matchMakeThreaded(matches,m_competitors,matchesLock,o); }));
	}

	for (size_t i = 0; i < workers.size(); i++) {
		workers[i]->join();
		delete workers[i];
	}



	


	END_CHRONO_LOG

	/*
	auto start = std::chrono::high_resolution_clock::now();
	matchMakeThreaded(matches, m_competitors, matchesLock, competitorsLock);

	auto finish = std::chrono::high_resolution_clock::now();

	std::cout << std::endl;
	std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish - start).count() << std::endl;
	*/
	//return;



}

#include <algorithm>


bool sortBySec(const std::pair<NeuralNetwork*, size_t> &a, const std::pair<NeuralNetwork*, size_t>& b) {
	return (a.second > b.second);
}


void MatchMaker::sortNNs()
{
	std::sort(m_competitors.begin(), m_competitors.end(), sortBySec);
}

NeuralNetwork* MatchMaker::getBest()
{
	return m_competitors[0].first;
}

void MatchMaker::split()
{


	for (size_t i = m_competitors.size()/2; i < m_competitors.size(); i++) {
		delete m_competitors[i].first;
	}

	m_competitors.erase(m_competitors.begin()+ m_competitors.size()/2, m_competitors.end());

}

void MatchMaker::regenerate()
{
	
	randomizationStrategy strat;
	//the values changed for mutations range from -x to +x according to the nummber below.
	strat.individual.maxRangeBeforeTransform = 0.01;

	size_t initialCompetitorsSize = m_competitors.size();

	
	for (size_t i = 0; i < initialCompetitorsSize-1; i++)
	{

		m_competitors[i].second = 0;
		NeuralNetwork* newNN;


		newNN = new NeuralNetwork(*m_competitors[i].first);

		
		newNN->addRandomWeights();
		m_competitors.push_back(std::make_pair(newNN,0));

	}

	NeuralNetwork* newNN;

	newNN = new NeuralNetwork(m_top,m_initialRandStrat);

	m_competitors.push_back(std::make_pair(newNN,0));
	
}

std::stringstream *MatchMaker::serializeMatchMaker() const
{
	std::stringstream *ss = new std::stringstream;
	for (auto nn : m_competitors)
	{

		(*ss) << nn.first->serialize() << std::endl << std::endl;
	}

	return ss;
}

MatchMaker::~MatchMaker()
{
	for (auto x : m_competitors) {
		delete x.first;
	}
}
