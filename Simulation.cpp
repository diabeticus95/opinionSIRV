#include "Simulation.h"
#include "Network.h"
#include "zerowanie.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

Simulation::Simulation(double b, double w, double p, double q, Network& sirv, Network& opinion, int size) : b(b), w(w), p(p), q(q), sirv(sirv), opinion(opinion), size(size){
	r = p/q;
	mt = std::mt19937(time(0));
	infection_dist = std::uniform_real_distribution<double>(0,1); //also opinion trigger
	states = new char[size];
	opinions = new int[size];
	sick_time = new int[size];
	init_opinions(); init_states(); zeruj(sick_time, size);
	update_vaxxers();
	vaccinate();
}

Simulation::~Simulation() {
	delete[] states;
	delete[] opinions;
}

void Simulation::init_states(){
	states[0] = 'I';
	for (int i = 1; i < size; i++){
		states[i] = 'S';
	}
	vaccinate();

}

void Simulation::init_opinions(){
    std::uniform_int_distribution<int> dist(0, 3);
    for (int i = 0; i < size; i++){
    	int rnd = dist(mt);
    	if(rnd == 0) opinions[i] = -2;
    	else if(rnd == 1) opinions[i] = -1;
    	else if(rnd == 2) opinions[i] = 1;
    	else if(rnd == 3) opinions[i] = 2;
    }
}

void Simulation::update_vaxxers(){
	for(int i = 0; i < size; i++){
		if(opinions[i] == 2){
			vaxxers.push_back(i);
		}
	}
}

void Simulation::vaccinate(){
	for (auto i : vaxxers){
			if(states[i] == 'S') states[i] = 'V';
		}
}

void Simulation::die(int i){
	states[i] = 'R';
}

void Simulation::get_sick(int i){
	states[i] = 'I';
}

void Simulation::infection_trial(int i){
	double rnd = infection_dist(mt);
	if(states[i] == 'S'){
		if(rnd < b) get_sick(i);
	}
	else if(states[i] == 'V'){
		if(rnd <(1-w)*b){
			get_sick(i);
			opinions[i] = -2;
			vaxxers.erase(std::remove(vaxxers.begin(), vaxxers.end(), i), vaxxers.end()); //usuniecie z vaxxerow chorujacego wezla
		}
	}
	else{
		std::cerr<<"infection trial got a node with state "<<states[i]<<std::endl;
		exit(-10);
	}
}

bool Simulation::can_interact(int agent_opinion, int neighbor_index){
	int opinion_j = opinions[neighbor_index];
	if(agent_opinion == -2 && opinion_j == -2) return false;
	else return true;
}

void Simulation::interact(int agent_index, int agent_opinion, int neighbor_opinion){
	double trigger = infection_dist(mt);

    if (agent_opinion == 1 && neighbor_opinion > 0 && trigger < p){
      opinions[agent_index] = agent_opinion + 1;
      if(states[agent_index] == 'S') vaxxers.push_back(agent_index);
    } else if (agent_opinion == -1 && neighbor_opinion < 0 && trigger < p){
    	opinions[agent_index] = agent_opinion - 1;
    } else if (agent_opinion == 1 && neighbor_opinion < 0 && trigger < q){
    	opinions[agent_index] = agent_opinion - 2;
    } else if (agent_opinion == -1 && neighbor_opinion > 0 && trigger < q){
    	opinions[agent_index]= agent_opinion + 2;
    } else if (agent_opinion == -2 && neighbor_opinion > 0 && trigger < q){
    	opinions[agent_index] = agent_opinion + 1;
    }
}

void Simulation::iterate_sirv(){
// kill off agents sick for dying period
	for(int i = 0; i < size; i++){
		if(states[i] == 'I'){
			sick_time[i]++;
			if(sick_time[i] == dying_period){
				die(i);
			}
		}
	}
// vaccinate all the agents with opinion +2
	vaccinate(); // remember to push to vaxxers on opinion iteration
// epidemy trial
	for(int i = 0; i < size; i++){
		if(states[i] == 'S' || states[i] == 'V'){
			bool sick_neighbor = 0;
			for(auto s : sirv.get_neighbors(i)){
				if(states[s] == 'I'){
					sick_neighbor = true;
					break;
				}
			}
			if(sick_neighbor) infection_trial(i);
		}
	}
}

void Simulation::iterate_opinion(){
	//iterate over all the individuals and give each one of them the chance to interact with only one of its neighbors
	//This neighbor is chosen among those who can change the individual opinion.
	std::vector<int> interactive_neighbors;
	int interaction_neighbor_opinion = 0;
	for(int i = 0; i < size; i++){
		int agent_opinion = opinions[i];
		if (agent_opinion == 2) continue; //+2 cannot change opinion by interaction, only by getting sick
		for(auto n : opinion.get_neighbors(i)){
			if(can_interact(agent_opinion, n))
				interactive_neighbors.push_back(n);
		}
		if(interactive_neighbors.size() == 0) continue;
		else if( interactive_neighbors.size() == 1)
			interaction_neighbor_opinion = opinions[interactive_neighbors[0]];
		else if(interactive_neighbors.size() > 1){
			std::uniform_int_distribution<int> opinion_dist(0, interactive_neighbors.size()-1);
			//sprawdzic koszt tworzenia nowych dist, moge przygotowac z gory do 20 sasiadow i tworzyc nowe tylko jesli jest ich wiecej
			interaction_neighbor_opinion = opinions[interactive_neighbors[opinion_dist(mt)]];
		}
		interact(i, agent_opinion, interaction_neighbor_opinion);
	}
}
int Simulation::iterate_until_end_of_epidemy(){
	int i = 0;
	while(get_sick_number()){
		iterate_sirv();
		iterate_opinion();
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
		std::cout<<opinions[i]<<"   "<<states[i]<<std::endl;
	}
}
void Simulation::print_groups(){
	std::cout<<"wszedlem"<<std::endl;
	for(auto i : vaxxers){
		std::cout<<i<<std::endl;
	}
	std::cout<<"koniec"<<std::endl;
}
void Simulation::print_state_counts(){
	int I = 0; int S = 0; int R = 0; int V = 0;
	for(int i = 0; i < size; i++){
		if(states[i] == 'S') S++;
			else if(states[i] == 'I') I++;
			else if(states[i] == 'R') R++;
			else if(states[i] == 'V') V++;
	}
	std::cout<<"S = "<<(double)S/size<<", I = "<<(double)I/size<<", R = "<<(double)R/size<<", V = "<<(double)V/size<<std::endl;
}
void Simulation::print_opinion_counts(){
	int opinion_counts[4];
	for(int i = 0; i < 4; i++) opinion_counts[i] = 0;
	for(int i = 0; i < size; i++){
		if(opinions[i] == -2) opinion_counts[0]++;
			else if(opinions[i] == -1) opinion_counts[1]++;
			else if(opinions[i] == 1) opinion_counts[2]++;
			else if(opinions[i] == 2) opinion_counts[3]++;
	}
	std::cout<<"-2 = "<<(double)opinion_counts[0]/size<<", -1 = "<<(double)opinion_counts[1]/size<<", 1 = "<<(double)opinion_counts[2]/size<<", 2 = "<<(double)opinion_counts[3]/size<<std::endl;
}

void Simulation::print_for_charts(std::string filename, bool first_run){
	FILE* fp = fopen(filename.c_str(), "a");
	if(first_run) fprintf(fp, "%c,%c,%c,%c,%s,%s,%s,%s\n", 'S', 'I', 'R', 'V', "-2", "-1", "1", "2");
	int opinion_counts[4];
	for(int i = 0; i < 4; i++) opinion_counts[i] = 0;
	int I = 0; int S = 0; int R = 0; int V = 0;

	for(int i = 0; i < size; i++){
		if(opinions[i] == -2) opinion_counts[0]++;
			else if(opinions[i] == -1) opinion_counts[1]++;
			else if(opinions[i] == 1) opinion_counts[2]++;
			else if(opinions[i] == 2) opinion_counts[3]++;
		if(states[i] == 'S') S++;
			else if(states[i] == 'I') I++;
			else if(states[i] == 'R') R++;
			else if(states[i] == 'V') V++;
	}
	fprintf(fp, "%d,%d,%d,%d,%d,%d,%d,%d\n", S,I,R,V,opinion_counts[0], opinion_counts[1], opinion_counts[2], opinion_counts[3]);
}
void Simulation::print_for_charts(std::string filename, int days){
	FILE* fp = fopen(filename.c_str(), "w");
	fprintf(fp, "%c,%c,%c,%c,%s,%s,%s,%s,%s\n", 'S', 'I', 'R', 'V', "-2", "-1", "1", "2", "days");
	int opinion_counts[4];
	for(int i = 0; i < 4; i++) opinion_counts[i] = 0;
	int I = 0; int S = 0; int R = 0; int V = 0;

	for(int i = 0; i < size; i++){
		if(opinions[i] == -2) opinion_counts[0]++;
			else if(opinions[i] == -1) opinion_counts[1]++;
			else if(opinions[i] == 1) opinion_counts[2]++;
			else if(opinions[i] == 2) opinion_counts[3]++;
		if(states[i] == 'S') S++;
			else if(states[i] == 'I') I++;
			else if(states[i] == 'R') R++;
			else if(states[i] == 'V') V++;
	}
	fprintf(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%d,%f\n", (double)S/size,(double)I/size,(double)R/size,(double)V/size,(double)opinion_counts[0]/size, (double)opinion_counts[1]/size, (double)opinion_counts[2]/size, (double)opinion_counts[3]/size, days, w);
}
