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

using namespace std;

int main() {
	Network* sirv = new Network(10, 0.5);
	sirv->print_all_nodes();
	std::cout<<std::endl<<std::endl;
	sirv->debug_print();
	return 0;
}

