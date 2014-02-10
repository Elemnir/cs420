#ifndef NEURON_H
#define NEURON_H

#include <vector>
#include <cmath>

double alpha(1.0f);

template <typename T>
class Neuron
{
  public:
	Neuron();
	void update();
	void calcoutputdelta(double actual);
	void calcdelta();
	void apply(double rate);
	
	int index;
	T output;
	double bias, delta;
	std::vector<Neuron<T>* > inputs;
	std::vector<Neuron<T>* > outputs;
	std::vector<T> weights;
  
  private:
	T sigmoid(T in);
};

template <typename T>
Neuron<T>::Neuron() 
{
}

template <typename T>
void Neuron<T>::update()
{
	T sum = 0;
	for (int i = 0; i < inputs.size(); ++i)
		sum = inputs[i]->output * weights[i];
	output = sigmoid(sum + bias);
}

template <typename T>
T Neuron<T>::sigmoid(T in)
{
	return 1.0f / (1.0f + exp( -alpha * in));
}

template <typename T>
void Neuron<T>::calcoutputdelta(double actual)
{
	delta = output * (1 - output) * (actual - output);
}

template <typename T>
void Neuron<T>::calcdelta()
{
	delta = 0.0f;
	for (int i = 0; i < outputs.size(); ++i)
	{
		delta += output * (1 - output) * outputs[i]->delta * outputs[i]->weights[index];
	}
}

template <typename T>
void Neuron<T>::apply(double rate)
{
	for (int i = 0; i < weights.size(); ++i)
	{
		weights[i] += rate * delta * inputs[i]->output;
	}
	bias += rate * delta;
}

#endif // NEURON_H
