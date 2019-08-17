#include "Network.h"
#include <ctime>
#include <iostream>

Network::Network(int size, double p):size(size), p(p){
	mt = std::mt19937_64(time(0));
	dist = std::uniform_real_distribution<double>(0.0, 1.0);
	net = new int*[size];
	for (int i = 0; i < size; i++){
		net[i] = new int[size];
	}
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			net[i][j] = 0;
		}
	}

	for (int i = 1; i < size; i++) {
		for (int j = 0; j < i; j++) {
			double trigger = dist(mt);
			if (trigger < p) {
				net[i][j] = 1;
			}
			else net[i][j] = 0;
		}
	}


}

Network::~Network() {

}

/*Network::Network(const Network &other) {
	// TODO Auto-generated constructor stub

}*/

/*Network::Network(Network &&other) {
	// TODO Auto-generated constructor stub

}*/





