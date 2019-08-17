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

using namespace std;

int main() {
	int size = 100000;
	clock_t begin = clock();
	Network* sirv = new Network(size, (double)4/size);
	clock_t end = clock();
	double time = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"uplynelo dokladnie "<<time;
	return 0;
}

