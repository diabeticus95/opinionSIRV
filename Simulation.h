#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <random>
#include <vector>
#include "Network.h"
#include <ctime>


class Simulation {
public:
	Simulation(int b, int w, Network& sirv, Network& opinions, int size);
	Simulation();
	virtual ~Simulation();
	void print_feature_arrays();
	void print_groups();
private:
    std::mt19937 mt;
	int dying_period = 6;
	int b; // bheta getting sick coefficient
	int w; // omega vax efficiency coefficient
	//Network sirv;
	//Network opinion;
	int size;
	int* sick_time;
	int* opinions;
	char* states;
	std::vector<int> vaxxers;


	void init_states();
	void init_opinions();
	void update_vaxxers();
	void vaccinate();
	void iterate_sirv();

};

#endif /* SIMULATION_H_ */
