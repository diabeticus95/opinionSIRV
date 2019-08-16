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
	Network* sirv = new Network(10, 0.5);
	sirv->print_node(0);
	sirv->print_all_nodes();
	return 0;
}
