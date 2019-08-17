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
	clock_t begin = clock();
	Network* sirv = new Network(100000, 0.5);
	clock_t end = clock();
	double time = double(end - begin)/CLOCKS_PER_SEC;
	std::cout<<"uplynelo dokladnie "<<time;
	return 0;
}

