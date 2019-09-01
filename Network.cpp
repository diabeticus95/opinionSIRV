#include "Network.h"
#include "zerowanie.h"
#include <ctime>
#include <iostream>
#include <unordered_set>

Network::Network(int size, double p):size(size), p(p){
	rand = pcg(rd);
	dist = std::uniform_int_distribution<int>(0, size-1);
	this->k = new int[size];
	this->n_of_links = p * size * (size-1) / 2;
//	std::cout<<"n of links"<<n_of_links<<std::endl;
	zeruj(k, size);
	this->net = new std::unordered_set<int>[size];

	draw_links();


}

Network::~Network() {
	delete[] k;
	for(int i = 0; i < size; i++){
		net[i].clear();
	}
}


void Network::draw_links(){
    std::pair<std::unordered_set<int>::iterator,bool> ret;
	int rep_counter = 0;
	for (int i = 0; i < n_of_links; i++){
		while(!insert_link(ret)){
			//std::cerr<<"repeating the draw"<<std::endl;
			rep_counter++;
		}
	}
	std::cerr<<"draw repeated "<<rep_counter<<" times"<<std::endl;
}
bool Network::insert_link(std::pair<std::unordered_set<int>::iterator,bool> ret){
	int tmp1 = dist(rand);
	int tmp2 = dist(rand);
	if(tmp1==tmp2) return 0;
	ret = net[tmp1].insert(tmp2);
	if(!ret.second) return 0;
	ret = net[tmp2].insert(tmp1);
	if (!ret.second){
		net[tmp1].erase(tmp2);
		return 0;
	}
	k[tmp1] += 1;
	k[tmp2] += 1;
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

double Network::get_mean_k(){
	bool debug = false;
	int sum = 0;
	for(int i = 0; i < size; i++){
		sum += k[i];
	}
	if(debug) std::cout<<"suma wynosi: "<<sum<<std::endl;
	return (double)sum/size;
}

void Network::get_degree_distribution(int hist[]){
	int debug = 0;
	for (int i = 0; i <size; i++){
		hist[k[i]]++;
	}
	if(debug) std::cout<<hist[size-10]<<std::endl;
}

void Network::export_igraph_edgelist(char* filename){
	int debug = 0;
	FILE *fp = fopen(filename, "w");
	for(int i = 0; i < size; i++){
		for(auto j : net[i]){
			fprintf(fp, "%d, %d\n", i, j);
		}
	}
	fclose(fp);
}

std::unordered_set<int>& Network::get_neighbors(int node){
	return net[node];
}







