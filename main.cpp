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
#include <string>

using namespace std;

int main() {

	int size = 100000;
	double p = (double)4/size;
	int cutoff = 100;

	clock_t begin = clock();
	Network sirv(size, p);
	Network opinion(size, p);
	clock_t end = clock();
	double time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"Tworzenie sieci ER trwa³o "<<time_elapsed<<std::endl;



	//begin = clock();
	//int b, int w, Network sirv, Network opinion, int size


	//end = clock();

	/*Simulation sim(0.1, 0.1, (double)10/11, (double)1/11, sirv, opinion, size);
	std::cout<<"feature arrays"<<std::endl;
	sim.print_feature_arrays();


	sim.print_state_counts();
	sim.print_opinion_counts();
	sim.iterate_sirv();
	sim.iterate_opinion();
	sim.print_state_counts();
	sim.print_opinion_counts();*/

	begin = clock();

/*
	for (int w = 0; w < 3; w++){
		for (int b = 0; b < 5; b++){
			clock_t iter_begin = clock();
			double b_c = 0.1;
			if (b == 1) b_c = 0.2;
			else if(b == 2) b_c = 0.4;
			else if(b == 3) b_c = 0.6;
			else if(b == 4) b_c = 0.8;
			double w_c = 0;
			if (w == 1) w_c = 0.5;
			else if (w == 2) w_c = 0.8;
			Simulation* sim;
			int counter = 0;
			int days = 0;
			do {
				if (counter > 0) delete sim;
				sim = new Simulation(b_c, w_c, (double)1 / 11, (double)10 / 11, sirv, opinion, size);
				days = sim->iterate_until_end_of_epidemy();
				counter++;
				std::cout << "recovered number:" << std::endl;
				std::cout << sim->get_recovered_number() << std::endl;
			}
			while (sim->get_recovered_number() <= cutoff);
			
			std::string filename("chart1.csv");
			if(w == 0 && b == 0) sim->print_for_charts(filename, true, days);
			else sim->print_for_charts(filename, false, days);
			delete sim;
			clock_t iter_end = clock();
			std::cout << "iteration no. " << 5*w + b + 1 << ", repeated " << counter - 1 << " times" << std::endl;
			std::cout << "iteration time " << double(iter_end - iter_begin) / CLOCKS_PER_SEC << std::endl;
		}
	}*/
	for (int i = 0; i < 100; i++){
		Network sirv(size, p);
		Network opinion(size, p);
		Simulation sim(0.1,0.5, (double)1 / 11, (double)10 / 11, sirv, opinion, size);
		std::cout<<sim.iterate_until_end_of_epidemy()<<"   "<<sim.get_recovered_number()<<std::endl;
	}


	/*for(int i = 0; i < 50; i++){
		sim.iterate_sirv();
		sim.iterate_opinion();
		if(i == 0) sim.print_for_charts("test_sirv.csv", true);
		else sim.print_for_charts("test_sirv.csv", false);
	}*/

	end = clock();
	time_elapsed = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"both iters "<<time_elapsed<<std::endl;
	return 0;

}

