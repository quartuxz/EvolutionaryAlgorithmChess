#include "NNManager.h"
#include <sstream>

NNManager::NNManager()
{
}

void NNManager::addNN(const std::string& name, NeuralNetwork* nn)
{
	m_NNs[name] = nn;
}

void NNManager::addNN(const std::string& name, const std::string& data)
{
	m_NNs[name] = new NeuralNetwork(data);
}

NeuralNetwork* NNManager::getNN(const std::string& name) const
{
	return m_NNs.at(name);
}

std::string NNManager::showNNs() const
{
	std::stringstream ss;
	for (auto x : m_NNs)
	{
		ss << x.first << std::endl;
	}
	return ss.str();
}

NNManager::~NNManager()
{

	for (auto x : m_NNs) {
		delete x.second;
	}
}
