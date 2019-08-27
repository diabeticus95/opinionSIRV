#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <random>
#include <vector>
#include "Network.h"
#include <ctime>


class Simulation {
public:
	Simulation(double b, double w, Network& sirv, Network& opinion, int size);
	virtual ~Simulation();
	void print_feature_arrays();
	void print_groups();
	void iterate_sirv();
	void iterate_opinion();
	void print_state_counts();
	void print_opinion_counts();

private:
    std::mt19937 mt;
	std::uniform_real_distribution<double> infection_dist;
	int dying_period = 6;
	double b; // bheta getting sick coefficient
	double w; // omega vax efficiency coefficient
	Network& sirv;
	Network& opinion;
	int size;
	int* sick_time;
	int* opinions;
	char* states;
	std::vector<int> vaxxers;

	void init_states();
	void init_opinions();
	void update_vaxxers();
	void vaccinate();
	void die(int i);
	void get_sick(int i);
	void infection_trial(int i);

};

#endif /* SIMULATION_H_ */
