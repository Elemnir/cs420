#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <random>
#include <cmath>
#include <stdlib.h>

#include "nd_rand.h"

using namespace std;

mt19937 rng(nd_rand());
double J1,J2;
int h,r1,r2;

struct Cell
{
    Cell() {};
    Cell(int v, int j, int i) {val = v; x = j; y = i;}
    bool update();
    int val, x, y;
    bool updated;
};

vector<vector<Cell> > field;

bool Cell::update()
{//updates the state of the cell, returns true if a change in state occurred.
    int oldval = val, radx, rady, rad;
    double sum = h;

    //iterate over all of the cell's neighbors
    for (int i = 0; i < field.size(); ++i)
    {
        for (int j = 0; j < field[i].size(); ++j)
        {
            //skip if the neighbor IS the cell
            if (i == y && j == x)
                continue;

            //compute the distance between the cell and its neighbor
            radx = abs(x - field[i][j].x);
            if (radx > 15) radx = 30 - radx;
            rady = abs(y - field[i][j].y);
            if (rady > 15) rady = 30 - rady;
            rad = radx + rady;

            //add the appropriate amount to the sum
            if (rad < r1) 
                sum += J1 * field[i][j].val;
            else if (rad < r2)
                sum += J2 * field[i][j].val;
        }
    }

    //update the state based on the sign of the sum
    if (sum < 0)
        val = -1;
    else
        val = 1;

    return oldval != val;
}

double beta(int s)
{
    return double(1 + s) / 2.0f;
}

void printImage(string const& fname)
{//prints the field to a .pgm image and converts it to .png w/ a system calls
    stringstream ss;
    ss << "r1-" << r1 << "_r2-" << r2 << "_h-" << h << "_" << fname;
    string filename = ss.str() + ".pgm",
           cmd = "convert " + ss.str() + ".pgm " + ss.str() + ".png",
           rm = "rm " + ss.str() + ".pgm";
    ofstream out(filename.c_str());
    out << "P2\n30 30\n255\n";
    for (auto i : field)
    {
        for (auto j : i)
        {
            if (j.val == 1) out << "0 ";
            else out << "255 ";
        }
        out << "\n";
    }
    out.close();
    system(cmd.c_str());
    system(rm.c_str());
}

int main(int argc, char** argv)
{
    if (argc != 7)
        return -1;

    //set global parameters 
    J1 = stod(argv[1]); J2 = stod(argv[2]);
    h = stoi(argv[3]); r1 = stoi(argv[4]); r2 = stoi(argv[5]);
    string id = argv[6];

    //initialize the field
    discrete_distribution<int> init_dist {1, 0, 1};
    for (int i = 0; i < 30; i++)
    {
        field.emplace_back();
        for (int j = 0; j < 30; j++)
            field.back().emplace_back(init_dist(rng) - 1, j, i);

    }
    
    //update the aica until it's in a stable state
    int changed_count = 1, iter_count = 0, index, jndex;
    uniform_int_distribution<int> dist(0,29);
    while (changed_count != 0 && iter_count++ < 1000)
    {
        //reset for a new iteration over the field
        for (vector<Cell>& i : field)
            for (Cell& j : i)
                j.updated = false;
        changed_count = 0;
        
        //iterate over the field
        for (int k = 0; k < 30 * 30; ++k)
        {
            do { //pick an unupdated cell
                index = dist(rng);
                jndex = dist(rng);
            } while ((field[index][jndex]).updated);

            //update the cell, and increment the counter if the state changed
            if ((field[index][jndex]).update())
                ++changed_count;
        }
    }

    //print the image and open the statistics file
    printImage(id);
    stringstream ss;
    ss << "r1-" << r1 << "_r2-" << r2 << "_h-" << h << ".csv";
    string statsfile = ss.str();
    ofstream stats(statsfile.c_str(), ofstream::app);

    //calculate correlation, joint entropy, and mutual information
    map<int, double> rho_l, h_l, i_l;
    int rad, radx, rady;
    double correlation_const=0.0f, prob_pl=0.0f, prob_mi=0.0f, entropy=0.0f;
    for (auto i : field)
        for (auto j : i)
        {
            correlation_const += j.val;
            prob_pl += beta(j.val);
        }

    //correlation
    correlation_const *= 1.0f / (30 * 30);
    correlation_const *= correlation_const;
    
    rho_l[0] = abs(1 - correlation_const);

    //system entropy
    prob_pl *= 1.0f / (30 * 30);
    prob_mi = 1 - prob_pl;

    if (prob_pl != 0.0f)
        entropy += prob_pl * log(prob_pl);
    if (prob_mi != 0.0f)
        entropy += prob_mi * log(prob_mi);
    entropy = -entropy;
    h_l[0] = i_l[0] = entropy;

    //iterated calculations
    for (int k = 1; k <= 14; ++k)
    {
        double inner_sum = 0.0f, beta_pos = 0.0f, beta_neg = 0.0f;
        double prob_pos = 0.0f, prob_neg = 0.0f, prob_dif = 0.0f;
        for (int m1 = 0; m1 < 30; ++m1)
            for (int n1 = 0; n1 < 30; ++n1)
                for (int m2 = m1; m2 < 30; ++m2)
                    for (int n2 = n1 + 1; n2 < 30; ++n2)
                    {
                        radx = abs(field[m1][n1].x - field[m2][n2].x);
                        if (radx > 15) radx = 30 - radx;
                        rady = abs(field[m1][n1].y - field[m2][n2].y);
                        if (rady > 15) rady = 30 - rady;
                        rad = radx + rady;

                        if (rad == k)
                        {
                            inner_sum += field[m1][n1].val * field[m2][n2].val;
                            beta_pos += beta(field[m1][n2].val) * beta(field[m2][n2].val); 
                            beta_neg += beta(-field[m1][n2].val) * beta(-field[m2][n2].val); 
                        }
                    }
        //correlation
        rho_l[k] = abs((double(2.0f * inner_sum) / (30*30*4*k)) - correlation_const);
        
        //joint entropy
        prob_pos = (2.0f * beta_pos) / (30*30*4*k);
        prob_neg = (2.0f * beta_neg) / (30*30*4*k);
        prob_dif = 1 - prob_pos - prob_neg;

        if (prob_pos != 0.0f)
            h_l[k] += prob_pos * log(prob_pos);
        if (prob_neg != 0.0f)
            h_l[k] += prob_neg * log(prob_neg);
        if (prob_dif != 0.0f)
            h_l[k] += prob_dif * log(prob_dif);
        h_l[k] = -h_l[k];

        //mutual information
        i_l[k] = (2 * entropy) - h_l[k];
    }
    
    stats << "Test:," << id << ", ";
    stats << "Rho_l:,";
    for (int i = 0; i <= 14; ++i)
        stats << rho_l[i] << ", ";
    stats << ",H_l:,";
    for (int i = 0; i <= 14; ++i)
        stats << h_l[i] << ", ";
    stats << ",I_l:,";
    for (int i = 0; i <= 14; ++i)
        stats << i_l[i] << ", ";
    stats << endl;

    stats.close();
    return 0;
}
