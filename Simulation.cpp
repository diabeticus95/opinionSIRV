#include "Simulation.h"
#include "Network.h"
#include "zerowanie.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include "random.h"

Simulation::Simulation(double b, double y, Network& sirv, int size, std::mt19937& mt, int chunk) :
	b(b), y(y), sirv(sirv), size(size), chunk(chunk){
	std::uniform_int_distribution<int> pcg_seed(0, RAND_MAX);
	rand = pcg(mt, pcg_seed);
	infection_dist = std::uniform_real_distribution<double>(0,1); //also opinion trigger
	states = new char[size];
	states_tmp = new char[size];
	init_states();
	this->neighbor_dist = neighbor_dist;
	for(int i = 0; i < size; i++){
		states_tmp[i] = states[i];
	}
}

Simulation::~Simulation() {
	delete[] states;
	delete[] states_tmp;
}

void Simulation::init_states(){
	states[0] = 'I';
	for (int i = 1; i < size; i++){
		states[i] = 'S';
	}
}


void Simulation::recover_trial(int& i){
	double rnd = infection_dist(rand);
	if(rnd < y)	states_tmp[i] = 'R';
}

void Simulation::get_sick(int& i){
	states_tmp[i] = 'I';
}

void Simulation::infection_trial(int& i, int& sick){
	//bool debug = false;
	if(states[i] == 'S'){
		for(int j = 0; j < sick; j++){
			double rnd = infection_dist(rand);
			if(rnd < b){
				get_sick(i);
				return;
			}
		}
	}
	else{
		std::cerr<<"infection trial got a node with state "<<states[i]<<std::endl;
		exit(-10);
	}
}


void Simulation::iterate_sirv(){
// kill off agents sick for dying period
	bool debug = false;
// epidemy trial
	for(int i = 0; i < size; i++){
		if(states[i] == 'S'){
			int sick_neighbor = 0;
			for(auto s : sirv.get_neighbors(i)){
				if(states[s] == 'I'){
					sick_neighbor++;
				}
			}
			if(sick_neighbor > 0) infection_trial(i, sick_neighbor);
		}
		else if(states[i] == 'I'){
			recover_trial(i);
		}
	}
	for(int i = 0; i < size; i++){
		states[i] = states_tmp[i];
	}
}

int Simulation::iterate_until_end_of_epidemy(){
	int i = 0;
	int debug = false;
	std::string filename(std::to_string(chunk) + "chart.csv");
	while(get_sick_number()){
		//if(i == 0 && b == 0.4 && y == 0.4) print_for_charts(filename, true, i);
		if(i == 0) print_for_charts(filename, true, i);
		print_for_charts(filename, false, i);
		iterate_sirv();
		i++;
	}

	return i;
}
int Simulation::get_sick_number(){
	int I = 0;
	for(int i = 0; i < size; i++)
			if(states[i] == 'I') I++;
	return I;
}

void Simulation::print_feature_arrays(){
	for(int i = 0; i < size; i++){
		std::cout<<states[i]<<std::endl;
	}
}

void Simulation::print_state_counts(){
	int I = 0; int S = 0; int R = 0;
	for(int i = 0; i < size; i++){
		if(states[i] == 'S') S++;
			else if(states[i] == 'I') I++;
			else if(states[i] == 'R') R++;
	}
	std::cout<<"S = "<<(double)S/size<<", I = "<<(double)I/size<<", R = "<<(double)R/size<<std::endl;
}

void Simulation::print_for_charts(std::string filename, bool first_run, int i){
	FILE* fp;
	if(first_run){
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "%c,%c,%c,%c,%c,%s\n", 'S', 'I', 'R', 'b', 'y', "lp");
	}
	else fp = fopen(filename.c_str(), "a");
	int I = 0; int S = 0; int R = 0;

	for(int i = 0; i < size; i++){
		if(states[i] == 'S') S++;
			else if(states[i] == 'I') I++;
			else if(states[i] == 'R') R++;
	}
	fprintf(fp, "%d,%d,%d, %f, %f, %d\n", S,I,R, b, y, i);
	fclose(fp);
}
void Simulation::print_for_charts(FILE* fp, bool first_run, int days){
	if(first_run){
		fprintf(fp, "%c,%c,%c,%c\n", 'S', 'I', 'R', 'b');
	}
	int I = 0; int S = 0; int R = 0;

	for(int i = 0; i < size; i++){
		if(states[i] == 'S') S++;
			else if(states[i] == 'I') I++;
			else if(states[i] == 'R') R++;
	}
	fprintf(fp, "%f,%f,%f,%f\n", (double)S/size,(double)I/size,(double)R/size, b);
}
