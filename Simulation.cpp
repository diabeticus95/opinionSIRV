#include "Simulation.h"
#include "Network.h"
#include "zerowanie.h"
#include <vector>
#include <random>
#include <iostream>
#include <algorithm>
#include "random.h"

Simulation::Simulation(double b, double w, double p, double q, Network& sirv, Network& opinion, int size, std::mt19937& mt,
		std::uniform_int_distribution<int> neighbor_dist[18], std::string filename) :
	b(b), w(w), p(p), q(q), sirv(sirv), opinion(opinion), size(size), filename(filename){
	r = p/q;
	std::uniform_int_distribution<int> pcg_seed(0, RAND_MAX);
	rand = pcg(mt, pcg_seed);
	infection_dist = std::uniform_real_distribution<double>(0,1); //also opinion trigger
	states = new char[size];
	opinions = new int[size];
	states_tmp = new char[size];
	opinions_tmp = new int[size];
	sick_time = new int[size];
	init_states(); init_opinions(); zeruj(sick_time, size);
	this->neighbor_dist = neighbor_dist;
	for(int i = 0; i < size; i++){
		opinions_tmp[i] = opinions[i];
		states_tmp[i] = states[i];
	}
}

Simulation::~Simulation() {
	delete[] states;
	delete[] opinions;
	delete[] states_tmp;
	delete[] opinions_tmp;
	delete[] sick_time;
}

void Simulation::init_states(){
	states[0] = 'I';
	for (int i = 1; i < size; i++){
		states[i] = 'S';
	}
}

void Simulation::init_opinions(){
    std::uniform_int_distribution<int> dist(0, 1);
    for (int i = 0; i < size; i++){
    	double rnd = infection_dist(rand);
    	int rnd_2 = dist(rand);
    	if(rnd < 0.87){ //wziete z ankiety CBOS
    		if(rnd_2 == 0){
    			opinions[i] = 2;
        		vaccinate(i);
    		}
    		else
    			opinions[i] = 1;
    	}
    	else{
    		if(rnd_2 == 0){
    			opinions[i] = -2;
    		}
    		else
    			opinions[i] = -1;
    	}
    }
}


void Simulation::vaccinate(int& i){
	if(states[i] == 'S'){
		states[i] = 'V';
		states_tmp[i] = 'V';
	}
}

void Simulation::die(int& i){
	states_tmp[i] = 'R';
}

void Simulation::get_sick(int& i){
	states_tmp[i] = 'I';
}

void Simulation::infection_trial(int& i, int& sick_num){
	//bool debug = false;
	for(int j = 0; j < sick_num; j++){
		double rnd = infection_dist(rand);
		if(states[i] == 'S'){
			if(rnd < b){
				get_sick(i);
				return;
			}
		}
		else if(states[i] == 'V'){
			if(rnd <(1-w)*b){
				get_sick(i);
				opinions[i] = -2;
				opinions_tmp[i] = -3;
				return;
			}
		}
		else{
			std::cerr<<"infection trial got a node with state "<<states[i]<<std::endl;
			exit(-10);
		}
	}

}

bool Simulation::can_interact(int& agent_opinion, int& neighbor_index){
	int opinion_j = opinions[neighbor_index];
	if(agent_opinion == -2 && opinion_j < 0) return false;
	else return true;
}

void Simulation::interact(int& agent_index, int& agent_opinion, int& neighbor_opinion){
	double trigger = infection_dist(rand);

    if ((agent_opinion == 1 && neighbor_opinion > 0) && trigger < p){
      opinions_tmp[agent_index] = 2;
      vaccinate(agent_index);
    } else if ((agent_opinion == -1 && neighbor_opinion < 0) && trigger < p){
    	opinions_tmp[agent_index] = -2;
    } else if ((agent_opinion == 1 && neighbor_opinion < 0) && trigger < q){
    	opinions_tmp[agent_index] = -1;
    } else if ((agent_opinion == -1 && neighbor_opinion > 0) && trigger < q){
    	opinions_tmp[agent_index]= 1;
    } else if ((agent_opinion == -2 && neighbor_opinion > 0) && trigger < q){
    	opinions_tmp[agent_index] = -1;
    }
}

void Simulation::iterate_sirv(){
// kill off agents sick for dying period
	bool debug = false;
	for(int i = 0; i < size; i++){
		if(states[i] == 'I'){
			sick_time[i]++;
			if(sick_time[i] == dying_period){
				die(i);
			}
		}
	}
// epidemy trial
	for(int i = 0; i < size; i++){
		if(states[i] == 'S' || states[i] == 'V'){
			int sick_neighbor = 0;
			for(auto s : sirv.get_neighbors(i)){
				if(states[s] == 'I'){
					sick_neighbor++;
					if(debug) std::cout<<"sick neighbor for node "<<i<<std::endl;
				}
			}
			if(sick_neighbor > 0) infection_trial(i, sick_neighbor);
		}
	}
	for(int i = 0; i < size; i++){
		states[i] = states_tmp[i];
	}
}

void Simulation::iterate_opinion(){
	//iterate over all the individuals and give each one of them the chance to interact with only one of its neighbors
	//This neighbor is chosen among those who can change the individual opinion.
	bool debug = false;
	std::vector<int> interactive_neighbors;
	int interaction_neighbor_opinion = 0;
	for(int i = 0; i < size; i++){
		int agent_opinion = opinions[i];
		if(debug){
			std::cout<<"agent "<<i<<" with opinion" <<agent_opinion<<std::endl;
		}
		if (agent_opinion == 2) continue; //+2 cannot change opinion by interaction, only by getting sick
		if (opinions_tmp[i] == -3){
			opinions_tmp[i] = -2;
			continue;
		}
		for(auto n : opinion.get_neighbors(i)){
			if(debug) std::cout<<"neighbor "<<n<<std::endl;
			if(can_interact(agent_opinion, n)){
				interactive_neighbors.push_back(n);
				if(debug) std::cout << "can interact with neighbor "<<n<<" with opinion "<<opinions[n]<<std::endl;
			}
		}
		if(interactive_neighbors.size() == 0) continue;
		else if( interactive_neighbors.size() == 1)
					interaction_neighbor_opinion = opinions[interactive_neighbors[0]];
				else if(interactive_neighbors.size() > 1){
					if(interactive_neighbors.size() < 20){
						int random_index = neighbor_dist[interactive_neighbors.size() - 2](rand);
						if(debug) std::cout<<"random index: "<<random_index<<std::endl;
						interaction_neighbor_opinion = opinions[interactive_neighbors[random_index]];
					}
					else{
						std::uniform_int_distribution<int> opinion_dist(0, interactive_neighbors.size()-1);
						interaction_neighbor_opinion = opinions[interactive_neighbors[opinion_dist(rand)]];
					}
		}
		interact(i, agent_opinion, interaction_neighbor_opinion);
		interactive_neighbors.clear();
	}
	for(int i = 0; i < size; i++){
		opinions[i] = opinions_tmp[i];
	}
}
int Simulation::iterate_until_end_of_epidemy(){
	int i = 0;
	int debug = false;
	while(get_sick_number()){
		iterate_sirv();
		if(debug) print_state_counts();
		if(i == 0)print_for_charts(filename, true);
		else print_for_charts(filename, false);
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

int Simulation::get_recovered_number(){ //for cutoff
	int R = 0;
	for (int i = 0; i < size; i++){
		if(states[i] == 'R') R++;
	}
	return R;
}

void Simulation::print_feature_arrays(){
	for(int i = 0; i < size; i++){
		std::cout<<opinions[i]<<"   "<<states[i]<<std::endl;
	}
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
	FILE* fp;
	if(first_run){
		fp = fopen(filename.c_str(), "w");
		fprintf(fp, "%c,%c,%c,%c,%s,%s,%s,%s\n", 'S', 'I', 'R', 'V', "-2", "-1", "1", "2");
	}
	else fp = fopen(filename.c_str(), "a");
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
	fclose(fp);
}
void Simulation::print_for_charts(FILE* fp, bool first_run, int days){
	if(first_run){
		fprintf(fp, "%c,%c,%c,%c,%s,%s,%s,%s,%s,%c,%c\n", 'S', 'I', 'R', 'V', "-2", "-1", "1", "2", "days", 'w', 'b');
	}

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
	fprintf(fp, "%f,%f,%f,%f,%f,%f,%f,%f,%d,%f,%f\n", (double)S/size,(double)I/size,(double)R/size,(double)V/size,(double)opinion_counts[0]/size, (double)opinion_counts[1]/size, (double)opinion_counts[2]/size, (double)opinion_counts[3]/size, days, w, b);
}
