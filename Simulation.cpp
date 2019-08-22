#include "Simulation.h"
#include "Network.h"
#include "zerowanie.h"
#include <vector>
#include <random>
#include <iostream>

Simulation::Simulation(int b, int w, Network& sirv, Network& opinion, int size) : b(b), w(w), sirv(sirv), opinion(opinion), size(size){
	//mt = std::mt19937(time(0));
	//states = new char[size];
	//opinions = new int[size];
	//sick_time = new int[size];
	//init_opinions(); init_states(); zeruj(sick_time, size);
	//update_vaxxers()
}

Simulation::~Simulation() {
/*	delete[] states;
	delete[] opinions; */
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

void Simulation::iterate_sirv(){
// kill off agents sick for dying period
// musze gdzies trzymac wektor chorych, wektor zaszczepionych itp i pilnowac kiedy sie zmienia
	for(int i = 0; i < size; i++){
		sick_time[i]++;
		if(sick_time[i] == dying_period) states[i] = 'R';
	}
// vaccinate all the agents with opinion +2

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

