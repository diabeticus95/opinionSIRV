#ifndef SIMULATION_H_
#define SIMULATION_H_

#include <random>
#include <vector>
#include "Network.h"
#include <ctime>
#include "random.h"


class Simulation {
public:
	Simulation(double b, double w, double p, double q, Network& sirv, Network& opinion, int size, std::mt19937& mt, std::uniform_int_distribution<int> neighbor_dist[18]);
	virtual ~Simulation();
	void print_feature_arrays();
	void iterate_sirv();
	void iterate_opinion();
	int iterate_until_end_of_epidemy(); //returns number of days until end of epidemy
	int get_recovered_number();	//used to implement cutoff
	double get_sir_iter();
	double get_op_iter();


	void print_state_counts();
	void print_opinion_counts();
	void print_for_charts(std::string filename, bool first_run);
	void print_for_charts(FILE* handle, bool first_run, int days);


private:
    pcg rand;
    std::uniform_real_distribution<double> infection_dist;
	std::uniform_int_distribution<int>* neighbor_dist;
	int dying_period = 6;
	double b; // bheta getting sick coefficient
	double w; // omega vax efficiency coefficient
	double p; // extremization coefficient;
	double q; // moderation coefficient
	double r; //personality ratio
	Network& sirv;
	Network& opinion;
	int size;
	int* sick_time;
	int* opinions;
	int* opinions_tmp;
	char* states;
	char* states_tmp;
	std::vector<double> sir_time;
	std::vector<double> op_time;
	double sir_iter = 0;
	double op_iter = 0;

	void init_states();
	void init_opinions();
	void vaccinate(int& i);
	void die(int& i);
	void get_sick(int& i);
	void infection_trial(int& i);
	bool can_interact(int& agent_opinion, int& neighbor_index);
	void interact(int& agent_index, int& agent_opinion, int& neighbor_opinion);
	int get_sick_number();

};

#endif /* SIMULATION_H_ */
