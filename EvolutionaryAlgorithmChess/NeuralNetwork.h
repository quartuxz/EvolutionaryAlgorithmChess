#pragma once
#include <vector>




typedef std::vector<size_t> Topology;

class Neuron;

typedef std::pair<double, Neuron*> synapse;

class Neuron {
private:
	std::vector<synapse> m_synapses;
	double m_value;
	double m_output;
public:
	Neuron();
	void setValue(double val);
	double activate()const;
	void reset();
};


class NeuralNetwork
{
private:
	
public:
	NeuralNetwork(Topology top);


	//sets the input layer to the passed values, pass them through the entire neural net, and get a vector of outputs.
	std::vector<double> pass(const std::vector<double> &input)const;

};

