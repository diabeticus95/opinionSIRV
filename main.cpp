//============================================================================
// Name        : Simulation.cpp
// Author      : pk
// Version     :
// Copyright   : praca mgr, wydzia³ Fizyki PW
//============================================================================

#include <iostream>
#include <random>
#include <ctime>
#include "Network.h"
#include "zerowanie.h"
#include "Simulation.h"
#include <string>

using namespace std;

int main() {
	int size = 100000;
	double p = (double)4/size;
	int cutoff = 300;
	std::mt19937 mt(time(0)); std::uniform_int_distribution<int> seeds(0, RAND_MAX);

	time_t begin = clock();

	std::uniform_int_distribution<int> neighbor_dist[18];
	std::string filename("chart_op.csv");
	FILE* fp_w = fopen(filename.c_str(), "w");
	FILE* fp_a = fopen(filename.c_str(), "a");
	for(int i = 0; i < 18; i++){
		neighbor_dist[i] = std::uniform_int_distribution<int>(0,i+1);
	}
	for(unsigned int rep = 0; rep < 1; rep++){
		Network* sirv = new Network(size, p, mt);
		Network* opinion = new Network(size, p, mt);
			double w = 0.95;
			double b = 0.9;
			clock_t iter_begin = clock();
			Simulation* sim;
			int counter = 0;
			int days = 0;
			do {
				if (counter > 0) delete sim;
				if(counter > 50){
					delete sirv; delete opinion;
					sirv = new Network(size, p, mt);
					opinion = new Network(size, p, mt);
					std::cout<<"swapping networks"<<std::endl;
					counter = 1;
				}
				sim = new Simulation(b, w, (double)10 / 11, (double)1 / 11, *sirv, *opinion, size, mt, neighbor_dist);
				days = sim->iterate_until_end_of_epidemy();
				counter++;
			}
			while (sim->get_recovered_number() <= cutoff);

			if(w == 0 && b == 0 && rep == 0){
				sim->print_for_charts(fp_w, true, days);
				fclose(fp_w);
			}
			else sim->print_for_charts(fp_a, false, days);
			delete sim;
			clock_t iter_end = clock();
			std::cout << "iteration no. " << (50*rep) + 50*w + b + 1 << ", repeated " << counter - 1 << " times" << std::endl;
			std::cout << "iteration time " << double(iter_end - iter_begin) / CLOCKS_PER_SEC << std::endl;

	}
	fclose(fp_a);
	time_t end = clock();
	double time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"both iters "<<time_elapsed<<std::endl;
	return 0;
}


