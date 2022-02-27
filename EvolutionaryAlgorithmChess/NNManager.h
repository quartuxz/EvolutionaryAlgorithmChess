#pragma once
#include "NeuralNetwork.h"
#include <map>

class NNManager
{
private:

	std::map<std::string, NeuralNetwork*> m_NNs;
public:

	NNManager();

	//provide name and pointer
	void addNN(const std::string& name, NeuralNetwork *nn);
	//provide name and serialized data
	void addNN(const std::string& name,const std::string& data);
	NeuralNetwork* getNN(const std::string& name)const;

	std::string showNNs()const;

	~NNManager();
};

