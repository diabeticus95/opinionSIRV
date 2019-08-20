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
#include "model_functions.h"

using namespace std;

int main() {
	int size = 100000;
	double p = (double)4/size;

	clock_t begin = clock();
	Network* sirv = new Network(size, p);
	Network* opinion = new Network(size, p);
	clock_t end = clock();
	double time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"Tworzenie sieci ER trwa³o "<<time_elapsed<<std::endl;

    std::mt19937 mt(time(0));

	int sickness_duration = 6;
	int sick_time[size], opinions[size];
	bool angry[size];
	char state[size];
	zeruj(sick_time,size); zeruj(angry,size); init_opinions(opinions, size, mt); init_state(state, opinions, size);


	/*for(int i = 0; i < size; i++){
		std::cout<<opinions[i]<<"   "<<state[i]<<std::endl;
	}*/


	return 0;
}

