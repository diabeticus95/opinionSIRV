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
	std::string filename("chart_var5_z_fixed_chunk" + std::to_string(chunk) + ".csv");
	FILE* fp_w = fopen(filename.c_str(), "w");
	FILE* fp_a = fopen(filename.c_str(), "a");
	for(int i = 0; i < 18; i++){
		neighbor_dist[i] = std::uniform_int_distribution<int>(0,i+1);
	}
	for(unsigned int rep = 0; rep < 8/std::thread::hardware_concurrency(); rep++){
		Network* sirv = new Network(size, p, mt);
		Network* opinion = new Network(size, p, mt);
			for (int lag = 0; lag < 20; lag+=1){
				for (int b = 0; b < 5; b++){
					clock_t iter_begin = clock();
					double b_c = 0.1;
					if (b == 1) b_c = 0.2;
					else if(b == 2) b_c = 0.4;
					else if(b == 3) b_c = 0.6;
					else if(b == 4) b_c = 0.8;
					double z = 0.9;
					Simulation* sim;
					int swap_counter = 0;
					int abandon_counter = 0;
					int days = 0;
					do {
						if(abandon_counter > 2) break;
						if(swap_counter > 0) delete sim;
						if(swap_counter > 50){
							delete sirv; delete opinion;
							sirv = new Network(size, p, mt);
							opinion = new Network(size, p, mt);
							std::cout<<"swapping networks"<<std::endl;
							swap_counter = 0;
							abandon_counter++;
						}
						sim = new Simulation(b_c, z, lag, (double)1/11, (double)10/11, *sirv, *opinion, size, mt, neighbor_dist);
						days = sim->iterate_until_end_of_epidemy();
						swap_counter++;
					}
					while (sim->get_recovered_number() <= cutoff);
					if(abandon_counter > 4) continue;

					if(lag == 0 && b == 0 && rep == 0){
						sim->print_for_charts(fp_w, true, days);
						fclose(fp_w);
					}
					else sim->print_for_charts(fp_a, false, days);
					delete sim;
					clock_t iter_end = clock();
					std::cout << "iteration no. " << (250*rep) + 5*lag + b + 1 << ", repeated " << abandon_counter * (swap_counter - 1) << " times" << std::endl;
					std::cout << "iteration time " << double(iter_end - iter_begin) / CLOCKS_PER_SEC << std::endl;
				}
			}
	}
	fclose(fp_a);
}


