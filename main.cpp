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
	double p = (double)100/size;
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
	Network sir(size, p, mt);
	for(unsigned int rep = 0; rep < 8/std::thread::hardware_concurrency(); rep++){
		double b = 0.1;
		double y = 0.1;
		Simulation sim(b, y, sir, size, mt, chunk);
		sim.iterate_until_end_of_epidemy();
	}
}





