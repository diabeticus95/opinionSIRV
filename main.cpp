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



	begin = clock();
	//int b, int w, Network sirv, Network opinion, int size

	Simulation sim(0.7, 0.9, sirv, opinion, size);

	end = clock();

	time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"init time "<<time_elapsed<<std::endl;
	//std::cout<<"feature arrays"<<std::endl;
	//sim.print_feature_arrays();
	//std::cout<<"groups"<<std::endl;
	//sim.print_groups();

}

