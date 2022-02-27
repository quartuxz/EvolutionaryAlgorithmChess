#include "NeuralNetwork.h"
#include <random>
#include <math.h>
#include <stdexcept>
#include <chrono>


std::default_random_engine randomizationStrategy::engine = std::default_random_engine(static_cast<long unsigned int>(time(0)));

double generateRandomNumber(const randomizationStrategy::tactic& tac) {

	std::uniform_real_distribution<double> unif(-tac.maxRangeBeforeTransform, tac.maxRangeBeforeTransform);
	
	
	return tac.transform(unif(randomizationStrategy::engine));
}


void Neuron::addRandomWeights(const randomizationStrategy& randStrat) const noexcept
{
	for (auto x : m_synapses) {
		x.first += generateRandomNumber(randStrat.individual);
	}
}

Neuron::Neuron(doubleToDoubleFunc activationFunction):
	m_activationFunction(activationFunction)
{
}

Neuron::Neuron(doubleToDoubleFunc activationFunction, const std::vector<synapse> &synapses):
	m_activationFunction(activationFunction),
	m_synapses(synapses)
{

}

void Neuron::setOutput(double val)
{
	m_output = val;
}

void Neuron::addValue(double val)
{
	m_value += val;
}

double Neuron::getOutput() const noexcept
{
	return m_output;
}

void Neuron::calculateOutput()
{
	m_output = m_activationFunction(m_value);
}

void Neuron::forwardPropagate() const
{
	for (auto syn : m_synapses)
	{
		syn.second->addValue(syn.first * m_output);
	}
}

void Neuron::addSynapse(const synapse &syn)
{
	m_synapses.push_back(syn);
}


void Neuron::reset()
{
	m_value = 0;
	m_output = 0;
}

std::vector<double> Neuron::getSynapseWeights()const
{
	std::vector<double> retval;

	for (auto syn:m_synapses)
	{
		retval.push_back(syn.first);
	}

	return retval;
}

void Neuron::setSynapseWeights(std::vector<double> newWeights)
{
	for (size_t i = 0; i < m_synapses.size(); i++) {
		m_synapses[i].first = newWeights[i];
	}
}

#include <iostream>

//we build up the neural network
NeuralNetwork::NeuralNetwork(Topology top, randomizationStrategy generationStrategy, doubleToDoubleFunc activationFunction):
	m_top(top),
	m_generationStrategy(generationStrategy),
	m_activationFunction(activationFunction)
{
	//we ccreate the layers
	for (size_t i = 0; i < top.size(); i++)
	{
		//we populate the layers
		NeuronLayer layer;
		for (size_t o = 0; o < top[i]; o++) {

			Neuron* neuron = new Neuron(activationFunction);
			layer.push_back(neuron);
			//if its not the input layer we add synapse connections the the previous layer.
			if (i != 0) {
				for (auto lastNeuron : m_allLayers[i-1]) {
					//std::cout << generateRandomNumber(generationStrategy.individual) << std::endl;
					lastNeuron->addSynapse({ generateRandomNumber(generationStrategy.individual),neuron });
				}

			}

		}
		if (i == 0) {
			m_inputLayer = layer;
		}
		else if(i == top.size()-1) {
			m_outputLayer = layer;
		}
		else {
			m_hiddenLayers.push_back(layer);
		}
		m_allLayers.push_back(layer);

	}


}


Topology getTopology(std::string str) {
	std::string currentNumber;

	Topology retval;

	int line = 0;

	for (char current : str) {
		if (current == '\n') {
			break;
		}
		if (current == ' ') {
			retval.push_back(std::atoi(currentNumber.c_str()));
			continue;
		}
		else {
			currentNumber.push_back(current);
		}
		
	}
	return retval;
}

NeuralNetwork::NeuralNetwork(const std::string& str) :
	NeuralNetwork(getTopology(str), randomizationStrategy())
{
	std::string currentNumber;
	
	Topology retval;

	//line of the string being read
	unsigned int line = 0;

	//orienting indexes for creating the NN with the corresponding values as read.
	unsigned int layerN = 0, neuronN = 0, synapseN = 0;

	for (char current : str) {
		//the first line was already read for topology information.
		if (current == '\n') {
			if (line >= 1) {
				break;
			}
			line++;
			continue;
		}
		if (line == 0) {
			continue;
		}


		if (current == ' ') {
			m_allLayers[layerN][neuronN]->m_synapses[synapseN].first = std::atof(currentNumber.c_str());
			currentNumber = "";
			synapseN++;
			continue;
		}
		else if (current == '/') {
			neuronN++;
			synapseN = 0;
		}
		else if (current == ',') {
			layerN++;
			neuronN = 0;
		}
		else {
			currentNumber.push_back(current);
		}

	}
}

void NeuralNetwork::addRandomWeights(const randomizationStrategy& randStrat)
{
	m_lock.lock();
	for (auto layer : m_allLayers) {
		for (auto neuron : layer)
		{
			neuron->addRandomWeights(randStrat);
		}
	}
	m_lock.unlock();
}

#include <sstream>

std::vector<double> NeuralNetwork::getResult(const std::vector<double>& input) const
{
	std::lock_guard<std::mutex> lockGuard(m_lock);

	if (m_inputLayer.size() != input.size()) {
		std::stringstream ss;
		ss << "input layer is not the same size as the parameters passed: " << m_inputLayer.size() << " vs " << input.size() << std::endl;
		throw std::invalid_argument(ss.str());
	}



	for (size_t i = 0; i < m_inputLayer.size(); i++) {
		m_inputLayer[i]->setOutput(input[i]);
	}

	for (auto neuron : m_inputLayer) {
		neuron->forwardPropagate();
	}

	for (auto layer : m_hiddenLayers) {

		for (auto neuron : layer)
		{
			neuron->calculateOutput(); 
			neuron->forwardPropagate();
		}
	}



	std::vector<double> retval;

	for (auto neuron : m_outputLayer)
	{
		neuron->calculateOutput();
		retval.push_back(neuron->getOutput());
	}

	for (auto layer : m_allLayers) {

		for (auto neuron : layer)
		{
			neuron->reset();
		}
	}

	return retval;
}

NeuralNetwork::NeuralNetwork(const NeuralNetwork& other):
	NeuralNetwork(other.m_top, other.m_generationStrategy, other.m_activationFunction)
{
	for (size_t i = 0; i < m_allLayers.size(); i++) {
		for (size_t o = 0; o < m_allLayers[i].size(); o++) {
			m_allLayers[i][o]->setSynapseWeights(other.m_allLayers[i][o]->getSynapseWeights());
		}
	}
}

std::string NeuralNetwork::serialize()const
{
	std::stringstream ss;
	for (auto x : m_top)
	{
		ss << x << " ";
	}
	ss << std::endl;
	for (auto layer : m_allLayers)
	{
		for (auto neuron : layer) {
			auto synapses = neuron->getSynapseWeights();
			for (auto syn : synapses) {
				ss << syn << " ";
			}
			ss << "/";
		}
		ss << ",";
	}
	return ss.str();
}

void NeuralNetwork::deserialize(std::string str) {

}

NeuralNetwork::~NeuralNetwork()
{
	if (deserializeNN != nullptr) {
		delete deserializeNN;
	}
	for (auto lay : m_allLayers) {
		for (auto neur : lay)
		{
			delete neur;
			
		}
	}
}

void randomizationStrategy::seedEngine()
{
	engine.seed();
}
