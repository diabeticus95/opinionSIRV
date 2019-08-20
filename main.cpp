//============================================================================
// Name        : Simulation.cpp
// Author      : pk
// Version     :
// Copyright   : praca mgr, wydzia� Fizyki PW
//============================================================================

#include <iostream>
#include <random>
#include <ctime>
#include "Network.h"
#include "zerowanie.h"

using namespace std;

int main() {
	int size = 100000;
	clock_t begin = clock();
	Network* sirv = new Network(size, (double)4/size);
	Network* opinion = new Network(size, (double)4/size);
	clock_t end = clock();
	double time = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"Tworzenie sieci ER trwa�o "<<time<<std::endl;;



	return 0;
}

