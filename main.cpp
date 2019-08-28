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
#include "Simulation.h"
#include "string"

using namespace std;

int main() {

	int size = 100000;
	double p = (double)4/size;

	clock_t begin = clock();
	Network sirv(size, p);
	Network opinion(size, p);
	clock_t end = clock();
	double time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"Tworzenie sieci ER trwa³o "<<time_elapsed<<std::endl;



	//begin = clock();
	//int b, int w, Network sirv, Network opinion, int size


	//end = clock();

	time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"init time "<<time_elapsed<<std::endl;
	//std::cout<<"feature arrays"<<std::endl;
	//sim.print_feature_arrays();
	//std::cout<<"groups"<<std::endl;
	//sim.print_groups();

	//sim.print_state_counts();
	//sim.print_opinion_counts();
	begin = clock();


	for (int i = 0; i < 10; i++){
		Simulation sim(0.1, (double)i/10, (double)10/11, (double)1/11, sirv, opinion, size);
		int days = sim.iterate_until_end_of_epidemy();
		std::string filename("test.csv");
		if(i == 0) sim.print_for_charts(filename, true, days);
		else sim.print_for_charts(filename, false, days);
	}

	end = clock();
	time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"both iters "<<time_elapsed<<std::endl;



}

