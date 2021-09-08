#pragma once
#include <vector>
#include <random>
#include <mutex>


typedef double (*doubleToDoubleFunc)(double);


struct randomizationStrategy {
	static std::default_random_engine engine;
	static void seedEngine();

	struct tactic {
		enum tacticType {
			porcentual, absolute
		}ttype = tacticType::absolute;
		double maxRangeBeforeTransform;
		//the default transform is the identity function
		doubleToDoubleFunc transform = [](double in) {return in; };
	}individual;
};


typedef std::vector<size_t> Topology;

class Neuron;

typedef std::pair<double, Neuron*> synapse;

class Neuron {
private:
	std::vector<synapse> m_synapses;
	double m_value;
	double m_output;
	doubleToDoubleFunc m_activationFunction;
public:


	void addRandomWeights(const randomizationStrategy &randStrat)const noexcept;


	Neuron(doubleToDoubleFunc activationFunction);
	void setOutput(double val);
	void addValue(double val);
	double getOutput()const noexcept;
	
	
	void calculateOutput();

	void forwardPropagate()const;

	void addSynapse(const synapse &syn);

	void reset();
};




typedef std::vector<Neuron*> NeuronLayer;

class NeuralNetwork
{
private:
	std::vector<NeuronLayer> m_hiddenLayers;
	NeuronLayer m_inputLayer;
	NeuronLayer m_outputLayer;
	std::vector<NeuronLayer> m_allLayers;

	mutable std::mutex m_lock;

public:

	NeuralNetwork(Topology top, randomizationStrategy generationStrategy, doubleToDoubleFunc activationFunction = [](double in) {return 1 / (1 + exp(-in)); });

	void addRandomWeights(const randomizationStrategy &randStrat);

	//sets the input layer to the passed values, pass them through the entire neural net, and get a vector of outputs.
	std::vector<double> getResult(const std::vector<double> &input)const;

	~NeuralNetwork();

};

