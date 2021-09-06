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


Neuron::Neuron(doubleToDoubleFunc activationFunction):
	m_activationFunction(activationFunction)
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

#include <iostream>

//we build up the neural network
NeuralNetwork::NeuralNetwork(Topology top, randomizationStrategy generationStrategy, doubleToDoubleFunc activationFunction)
{

	for (size_t i = 0; i < top.size(); i++)
	{
		NeuronLayer layer;
		for (size_t o = 0; o < top[i]; o++) {

			Neuron* neuron = new Neuron(activationFunction);
			layer.push_back(neuron);
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

#include <sstream>

std::vector<double> NeuralNetwork::getResult(const std::vector<double>& input) const
{


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

NeuralNetwork::~NeuralNetwork()
{
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
