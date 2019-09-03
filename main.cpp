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
	int size = 100000;
	double p = (double)4/size;
	int cutoff = 30;
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
	std::uniform_int_distribution<int> neighbor_dist[18];
	std::string filename("chart1.csv");
	FILE* fp_w = fopen(filename.c_str(), "w");
	FILE* fp_a = fopen(filename.c_str(), "a");
	for(int i = 0; i < 18; i++){
		neighbor_dist[i] = std::uniform_int_distribution<int>(0,i+1);
	}
	for(unsigned int rep = 0; rep < 8/std::thread::hardware_concurrency(); rep++){
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
						if(counter > 50){
							Network sirv(size, p, mt);
							Network opinion(size, p, mt);
							std::cout<<"swapping networks"<<std::endl;
							counter = 1;
						}
						sim = new Simulation(b_c, w, (double)1 / 11, (double)10 / 11, sirv, opinion, size, mt, neighbor_dist);
						days = sim->iterate_until_end_of_epidemy();
						counter++;
						std::cout << "recovered number:" << std::endl;
						std::cout << sim->get_recovered_number() << std::endl;
					}
					while (sim->get_recovered_number() <= cutoff);

					std::string filename("chart" + std::to_string(chunk) + ".csv");
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
			}
	}
	fclose(fp_a);
}


