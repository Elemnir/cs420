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
	void train(std::vector<T> &adj);
	
	T output;
	std::vector<Neuron<T>* > inputs;
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
	output = sigmoid(sum);
}

template <typename T>
void Neuron<T>::train(std::vector<T> &adj)
{
}

template <typename T>
T Neuron<T>::sigmoid(T in)
{
	return 1.0f / (1.0f + exp( -alpha * in));
}

#endif // NEURON_H
