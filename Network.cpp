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
		draw_node_connections(tmp, i);
		k[i] = push_node(tmp);
		i==0?l_seg[i] = 0:l_seg[i] = l_seg[i-1] + k[i-1];
	}



}

Network::~Network() {
	delete(k);
	delete(l_seg);
}

/*Network::Network(const Network &other) {
	// TODO Auto-generated constructor stub

}*/

/*Network::Network(Network &&other) {
	// TODO Auto-generated constructor stub

}*/

void Network::draw_node_connections(int* tmp, int i){
// Lower matrix triangle drawing, leaving already drawn spots empty, by starting iterations with i, which is number of column
	bool debug = 1;
	for(int j = i; j<size; j++){
		if(j == i) tmp[j] = 0;
		else{
			double trigger = dist(mt);
			trigger < p?tmp[j] = 1:tmp[j] = 0;
		}
	}
//still need to fill up spaces drawn in earlier iterations
//I need to feel exactly 'i' spaces.
// go through all net before now and save all occurences of current node
	std::vector<int> occurences;
	std::vector<int> neighbors;
	for(int j = 0; j < (int)net.size(); j++){
		if (net[j] == i) occurences.push_back(j);
	}
	for (auto j : occurences){
		neighbors.push_back(track_node_by_index_of_neighbor(j));
	}
	// i assume there were no neighbors
	for(int j = 0; j < i; j++){
		tmp[j] = 0;
	}
	if(debug == 1){
		std::cout<<"node nr "<<i<<std::endl<<"occurences: ";
		for(auto j:occurences) std::cout<<j<<" ";
		std::cout<<std::endl;
		std::cout<<"neighbors: ";
		for(auto j:neighbors) std::cout<<j<<" ";
		std::cout<<std::endl<<std::endl;
	}
	// and then overwrite connections I found with ones
	for (auto j : neighbors){
		tmp[j] = 1;
	}
}

int Network::track_node_by_index_of_neighbor(int i){
	for (int j = 0; j < size; j++){
		if( i >= l_seg[j] && i < l_seg[j] + k[j]) return j;
	}
	std::cerr<<"unable to track node by neighbor, with index "<<i<<std::endl;
	return size-1; //last element
}

int Network::retrieve_link(int i, int j){
	for (int x = 0; x < k[i]; x++){
		if(net[l_seg[i] + x] == j){
			return 1;
		}
	}
	return 0;
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
		std::cout<<j<<" column"<<std::endl;
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

void Network::debug_print(){
	std::cout<<"net: "<<std::endl;
	for(int i = 0; i < (int)net.size(); i++){
		std::cout<<net[i]<<" ";
	}
	std::cout<<std::endl;

	std::cout<<"k: "<<std::endl;
	for(int i = 0; i < size; i++){
		std::cout<<k[i]<<" ";
	}
	std::cout<<std::endl;

	std::cout<<"l_seg: "<<std::endl;
	for(int i = 0; i < size; i++){
		std::cout<<l_seg[i]<<" ";
	}
	std::cout<<std::endl;

	std::cout<<"index 9 in net belongs to node nr "<<track_node_by_index_of_neighbor(9);
	std::cout<<std::endl;

}


