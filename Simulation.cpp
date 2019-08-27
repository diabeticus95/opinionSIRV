#include "Simulation.h"
#include "Network.h"
#include "zerowanie.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>

Simulation::Simulation(double b, double w, Network& sirv, Network& opinion, int size) : b(b), w(w), sirv(sirv), opinion(opinion), size(size){
	mt = std::mt19937(time(0));
	infection_dist = std::uniform_real_distribution<double>(0,1);
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
		}
	}
	else{
		std::cerr<<"infection trial got a node with state "<<states[i]<<std::endl;
		exit(-10);
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
	std::cout<<"S = "<<S<<", I = "<<I<<", R = "<<R<<", V = "<<V<<std::endl;
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
	std::cout<<"-2 = "<<opinion_counts[0]<<", -1 = "<<opinion_counts[1]<<", 0 = "<<opinion_counts[2]<<", 1 = "<<opinion_counts[3]<<std::endl;

}

