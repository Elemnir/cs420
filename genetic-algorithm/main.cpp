#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

struct Entity
{
    Entity(string const& in) 
    { 
        g = in;  
    }
    
    void update() 
    {
        unsigned val = 0;
        for (unsigned i = 0; i < g.size(); i++)
            if (g[g.size() - 1 - i] == '1')
                val += (1 << i);
        fit = pow((double(val) / pow(2,g.size())), 10);
    }

    double fit, norm, window;
    string g;
};

int main(int argc, char** argv)
{
    ifstream config;
    if (argc == 1) 
        config.open("config.param");
    else
        config.open(argv[1]);
    
    if (config.fail())
    {
        cerr << "Config file not found\n";
        return -1;
    }
    
    int populationSize, geneSize, generationCount;
    double mutateProb, crossProb, seed;
    config >> geneSize >> populationSize >> mutateProb >> crossProb 
           >> seed >> generationCount;
    config.close();
    
    mt19937 rng(random_device{}());
    if (seed != -1)
        rng.seed(seed);

    discrete_distribution<int> init{1,1};
    vector<Entity> population, nextgen;
    string temp;
    for (int i = 0; i < populationSize; ++i)
    {
        temp.clear();
        for (int j = 0; j < geneSize; ++j)
        {
            temp.push_back(init(rng) + 48);
        }
        population.emplace_back(temp);
    }

    //distribution for picking mating parties
    uniform_real_distribution<double> dist(0.0, 1.0);
    
    //distribution for deciding whether or not to mutate
    mutateProb *= 1000;
    discrete_distribution<int> mutate{int(1000 - mutateProb), int(mutateProb)};

    //distribution for deciding whether or not to cross genes
    crossProb *= 1000;
    discrete_distribution<int> cross{int(1000 - crossProb), int(crossProb)};

    //distribution for picking a crossover point
    uniform_int_distribution<int> point(0, populationSize - 1);

    //maps for recording data
    map<int, double> bestFitness, avgFitness, numBits;
    
    cerr << "Completed initialization, starting generation...\n";

    //loop for a number of generations
    double totalfitness, runningsum, maxindex, maxfitness; 
    for (int g = 0; g <= generationCount; ++g)
    {
        nextgen.clear();

        totalfitness = maxfitness = runningsum = 0.0f;
        for (int i = 0; i < population.size(); ++i)
        {
            population[i].update();
            totalfitness += population[i].fit;
            if (population[i].fit > maxfitness)
                maxindex = i;
        }

        avgFitness[g] = totalfitness / population.size();
        bestFitness[g] = population[maxindex].fit;
        for (int i = 0; i < population[maxindex].g.size(); ++i)
            if (population[maxindex].g[i] == '1')
                numBits[g] += 1;

        //calculate windows
        for (int i = 0; i < population.size(); ++i)
        {
            population[i].norm = population[i].fit / totalfitness;
            runningsum += population[i].norm;
            population[i].window = runningsum;
        }

        //mating loop
        for (int i = 0; i < population.size() / 2; ++i)
        {
            //get two parents
            double val1 = dist(rng), val2;
            int index1, index2;
            for (index1 = 0; index1 < population.size(); ++index1)
                if (population[index1].window > val1)
                    break;
            
            do {
                val2 = dist(rng);
                for (index2 = 0; index2 < population.size(); ++index2)
                    if (population[index2].window > val2)
                        break;
            } while (index1 != index2);

            //decide if there needs to be a crossover
            if (cross(rng))
            {
                int cp = point(rng);
                string temp1 = population[index1].g, temp2 = population[index2].g;
                swap_ranges(temp1.begin(), temp1.begin() + cp, temp2.begin());
                
                nextgen.emplace_back(temp1);
                nextgen.emplace_back(temp2);    
            }
            else
            {
                nextgen.emplace_back(population[index1]);
                nextgen.emplace_back(population[index2]);
            }

            //perform any mutations
            for (auto is = nextgen[nextgen.size() - 2].g.begin(); 
                    is != nextgen[nextgen.size() - 2].g.end(); ++is)
                if (mutate(rng))
                {
                    if (*is == '0')
                        *is = '1';
                    else
                        *is = '0';
                }
        
            //perform any mutations
            for (auto is = nextgen[nextgen.size() - 1].g.begin(); 
                    is != nextgen[nextgen.size() - 1].g.end(); ++is)
                if (mutate(rng))
                {
                    if (*is == '0')
                        *is = '1';
                    else
                        *is = '0';
    
                }
        }
        population = nextgen;
    }

    ofstream best("best.csv");
    for (auto& i : bestFitness)
        best << i.first << ", " << i.second << endl;
    
    ofstream avg("avg.csv");
    for (auto& i : avgFitness)
        avg << i.first << ", " << i.second << endl;
    
    ofstream bits("bits.csv");
    for (auto& i : numBits)
        bits << i.first << ", " << i.second << endl;
    
    return 0;
}
