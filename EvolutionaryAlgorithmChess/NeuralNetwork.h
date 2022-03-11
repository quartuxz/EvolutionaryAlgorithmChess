#pragma once
#include <vector>
#include <random>
#include <mutex>


typedef double (*doubleToDoubleFunc)(double);


struct randomizationStrategy {
	static std::default_random_engine engine;
	static void seedEngine();

	double initialRandomRange = 1.0f;

	//for mutations
	struct tactic {
		enum tacticType {
			porcentual, absolute
		}ttype = tacticType::absolute;
		double maxRangeBeforeTransform = 0.005f;
		//the default transform is the identity function
		doubleToDoubleFunc transform = [](double in) {return in; };
		tactic();
	}individual = tactic();

	randomizationStrategy();
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
	friend class NeuralNetwork;

	void addRandomWeights(const randomizationStrategy &randStrat);


	Neuron(doubleToDoubleFunc activationFunction);


	Neuron(doubleToDoubleFunc activationFunction, const std::vector<synapse> &synapses);




	void setOutput(double val);
	void addValue(double val);
	double getOutput()const noexcept;
	
	
	void calculateOutput();

	void forwardPropagate()const;

	void addSynapse(const synapse &syn);

	void reset();

	std::vector<double> getSynapseWeights()const;
	void setSynapseWeights(std::vector<double> newWeights);
};




typedef std::vector<Neuron*> NeuronLayer;

class NeuralNetwork
{
private:
	std::vector<NeuronLayer> m_hiddenLayers;
	NeuronLayer m_inputLayer;
	NeuronLayer m_outputLayer;
	std::vector<NeuronLayer> m_allLayers;

	mutable std::mutex m_copyLock, m_resultLock;
	

	NeuralNetwork *deserializeNN = nullptr;

	Topology m_top;
	randomizationStrategy m_randStrat = randomizationStrategy();
	doubleToDoubleFunc m_activationFunction;
public:

	NeuralNetwork(Topology top, const randomizationStrategy &generationStrategy, doubleToDoubleFunc activationFunction = [](double in) {return 1 / (1 + exp(-in)); });
	NeuralNetwork(const std::string& str);

	void addRandomWeights();

	//sets the input layer to the passed values, pass them through the entire neural net, and get a vector of outputs.
	std::vector<double> getResult(const std::vector<double> &input)const;

	NeuralNetwork(const NeuralNetwork &other);


	std::string serialize()const;
	void deserialize(std::string str);

	~NeuralNetwork();

};

