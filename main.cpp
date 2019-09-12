//============================================================================
// Name        : Simulation.cpp
// Author      : pk
// Version     :
// Copyright   : praca mgr, wydzia� Fizyki PW
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
	int size = 10000;
	double p = (double)4/size;
	int cutoff = 2;
	std::mt19937 mt(time(0)); std::uniform_int_distribution<int> seeds(0, RAND_MAX);

	time_t begin = clock();
	int n_of_chunks = std::thread::hardware_concurrency();
	std::thread threads_array[n_of_chunks];
	for (int chunk = 0; chunk < n_of_chunks; chunk++){
		int seed = seeds(mt);
	    threads_array[chunk] = std::thread(simulate_parallel,size, p, cutoff,  chunk, seed);
	}
	for (int chunk = 0; chunk < n_of_chunks; chunk++){
		threads_array[chunk].join();
	}
	time_t end = clock();
	double time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"both iters "<<time_elapsed<<std::endl;
	return 0;

}
void simulate_parallel(int size, double p, int cutoff, int chunk, int seed){
	std::mt19937 mt(seed);
	for(unsigned int rep = 0; rep < 8/std::thread::hardware_concurrency(); rep++){
		Network* sir = new Network(size, p, mt);
		for (double b = 0.3; b < 1; b+= 0.1){
			for(double y = 0.1; y < 1; y += 0.1){
				clock_t iter_begin = clock();
				Simulation* sim;
				int swap_counter = 0;
				int abandon_counter = 0;
				do {
					if(abandon_counter > 4) break;
					if(swap_counter > 0) delete sim;
					if(swap_counter > 50){
						delete sir;
						sir = new Network(size, p, mt);
						std::cout<<"swapping networks"<<std::endl;
						swap_counter = 0;
						abandon_counter++;
					}
					sim = new Simulation(b, y, *sir, size, mt, chunk);
					sim->iterate_until_end_of_epidemy();
					swap_counter++;
				}
				while (sim->get_recovered_number() <= cutoff);
				if(abandon_counter > 4) continue;

				clock_t iter_end = clock();
				std::cout << "iteration no. " << (100*rep) + 10*y + b + 1 << ", repeated " << abandon_counter * (swap_counter - 1) << " times" << std::endl;
				std::cout << "iteration time " << double(iter_end - iter_begin) / CLOCKS_PER_SEC << std::endl;
			}
			}


	}
}


