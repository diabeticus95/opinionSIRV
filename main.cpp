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
#include "string"
#include <thread>

using namespace std;

int main() {
	void simulate_parallel(int size, double p, int cutoff, int rep, int seed);
	int size = 100000;
	double p = (double)4/size;
	int cutoff = 50;
	std::mt19937 mt(time(0)); std::uniform_int_distribution<int> seeds(0, RAND_MAX);

	time_t begin = clock();
	int n_of_reps = 10;
	std::thread threads_array[n_of_reps];
	for (int rep = 0; rep < n_of_reps; rep++){
		int seed = seeds(mt);
	    threads_array[rep] = std::thread(simulate_parallel,size, p, cutoff,  rep, seed);
	}
	for (int rep = 0; rep < n_of_reps; rep++){
		threads_array[rep].join();
	}
	time_t end = clock();
	double time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"both iters "<<time_elapsed<<std::endl;
	return 0;

}
void simulate_parallel(int size, double p, int cutoff, int rep, int seed){
	std::mt19937 mt(seed);
	Network sirv(size, p, mt);
	Network opinion(size, p, mt);

	for (double w = 0; w < 1; w+=0.1){
		for (int b = 0; b < 5; b++){
			clock_t iter_begin = clock();
			double b_c = 0.1;
			if (b == 1) b_c = 0.2;
			else if(b == 2) b_c = 0.4;
			else if(b == 3) b_c = 0.6;
			else if(b == 4) b_c = 0.8;
			Simulation* sim;
			int counter = 0;
			int days = 0;
			do {
				if (counter > 0) delete sim;
				if(counter > 10){
					Network sirv(size, p, mt);
					Network opinion(size, p, mt);
					std::cout<<"swapping networks"<<std::endl;
					counter = 1;
				}
				sim = new Simulation(b_c, w, (double)1 / 11, (double)10 / 11, sirv, opinion, size, mt);
				days = sim->iterate_until_end_of_epidemy();
				counter++;
				std::cout << "recovered number:" << std::endl;
				std::cout << sim->get_recovered_number() << std::endl;
			}
			while (sim->get_recovered_number() <= cutoff);
			
			std::string filename("chart" + std::to_string(rep) + ".csv");
			if(w == 0 && b == 0) sim->print_for_charts(filename, true, days);
			else sim->print_for_charts(filename, false, days);
			delete sim;
			clock_t iter_end = clock();
			std::cout << "iteration no. " << 50*w + b + 1 << ", repeated " << counter - 1 << " times" << std::endl;
			std::cout << "iteration time " << double(iter_end - iter_begin) / CLOCKS_PER_SEC << std::endl;
		}
	}

}


