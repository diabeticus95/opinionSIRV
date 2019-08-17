#include "Network.h"
#include "zerowanie.h"
#include <ctime>
#include <iostream>
#include <unordered_set>

Network::Network(int size, double p):size(size), p(p){
	mt = std::mt19937_64(time(0));
	dist = std::uniform_int_distribution<int>(0, size-1);
	this->k = new int[size];
	this->l_seg = new int[size];
	this->n_of_links = p * size * (size-1) / 2;
	zeruj(k, size); zeruj(l_seg, size);
	this->net = new std::unordered_set<int>[size];

	draw_links();


}

Network::~Network() {
	delete[] k;
	delete[] l_seg;
	for(int i = 0; i < size; i++){
		net[i].clear();
	}
}


void Network::draw_links(){
    std::pair<std::unordered_set<int>::iterator,bool> ret;
	int rep_counter = 0;
	for (int i = 0; i < n_of_links; i+=2){
		while(!insert_link(ret)){
			//std::cerr<<"repeating the draw"<<std::endl;
			rep_counter++;
		}
	}
	std::cerr<<"draw repeated "<<rep_counter<<" times"<<std::endl;
}
bool Network::insert_link(std::pair<std::unordered_set<int>::iterator,bool> ret){
	int tmp1 = dist(mt);
	int tmp2 = dist(mt);
	if(tmp1==tmp2) return 0;
	ret = net[tmp1].insert(tmp2);
	if(!ret.second) return 0;
	ret = net[tmp2].insert(tmp1);
	if (!ret.second){
		net[tmp1].erase(tmp2);
		return 0;
	}
	return 1;
}

void Network::print_all_links(){
	for (int i = 0; i < size; i++){
		std::cout<<i<<" column"<<std::endl;
		for (auto j : net[i]){
			std::cout<<j<<" ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}



		// dzielenie na segmenty
		// macierz wektorow





