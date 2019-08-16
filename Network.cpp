#include "Network.h"
#include "zerowanie.h"
#include <ctime>
#include <iostream>

Network::Network(int size, double p):size(size), p(p){
	mt = std::mt19937_64(time(0));
	dist = std::uniform_real_distribution<double>(0.0, 1.0);
	this->k = new int[size];
	this->l_seg = new int[size];
	zeruj(k, size); zeruj(l_seg, size);
	for (int i = 0; i < size; i++){
		int* tmp = new int[size];
		draw_node_connections(tmp);
		k[i] = push_node(tmp);
		i==0?l_seg[i] = 0:l_seg[i] = l_seg[i-1] + k[i];
	}



}

Network::~Network() {
	// TODO Auto-generated destructor stub
}

/*Network::Network(const Network &other) {
	// TODO Auto-generated constructor stub

}*/

/*Network::Network(Network &&other) {
	// TODO Auto-generated constructor stub

}*/

void Network::draw_node_connections(int* tmp){
	for(int i = 0; i<size; i++){
		double trigger = dist(mt);
		trigger < p?tmp[i] = 1:tmp[1] = 0;
	}
}

int Network::push_node(int* tmp){
	int k = 0;
	for (int i = 0; i < size; i++){
		if (tmp[i] == 1){
			net.push_back(i);
			k++;
		}
	}
	return k;
}

void Network::print_all_nodes(){
	for (int j = 0; j < size; j++){
		for (int y = l_seg[j]; y < l_seg[j] + k[j]; y++){
			std::cout<<net[y]<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}

void Network::print_node(int i){
	for (int y = l_seg[i]; y < l_seg[i] + k[i]; y++)
		std::cout<<net[y]<<" ";
	std::cout<<std::endl;
}
