#ifndef NEURON_H
#define NEURON_H

#include <vector>

template <typename T>
class Neuron
{
  public:
	Neuron();
	void train(std::vector<T> &adj);
	
	int output;

  private:
	std::vector<Neuron<T>* > inputs;
	std::vector<T> weights;
};

#endif // NEURON_H
