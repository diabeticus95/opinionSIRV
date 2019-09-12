#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <random>
#include <vector>
#include "Network.h"
#include <ctime>
#include "random.h"


class Simulation {
public:
	Simulation(double b, double y, Network& sirv, int size, std::mt19937& mt, int chunk);
	virtual ~Simulation();
	void print_feature_arrays();
	void iterate_sirv();
	int iterate_until_end_of_epidemy(); //returns number of days until end of epidemy


	void print_state_counts();
	void print_opinion_counts();
	void print_for_charts(std::string filename, bool first_run, int i);
	void print_for_charts(FILE* handle, bool first_run, int days);


private:
    pcg rand;
    std::uniform_real_distribution<double> infection_dist;
	std::uniform_int_distribution<int>* neighbor_dist;
	double b; // bheta getting sick coefficient
	double y; // recovery coeff
	Network& sirv;
	int size;
	char* states;
	char* states_tmp;
	int chunk;

	void init_states();
	void recover_trial(int& i);
	void get_sick(int& i);
	void infection_trial(int& i);
	int get_sick_number();

};

#endif /* SIMULATION_H_ */
