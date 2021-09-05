#include "NeuralNetwork.h"

void Neuron::setValue(double val)
{
	m_value = val;
}

double Neuron::activate() const
{
	return 0.0;
}

void Neuron::reset()
{
	m_value = 0;
	m_output = 0;
}
