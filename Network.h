#ifndef NETWORK_H_
#define NETWORK_H_
#include <random>
#include <vector>
#include <unordered_set>
#include "random.h"

class Network {
public:
	Network(int size, double p, std::mt19937 &mt);
	virtual ~Network();
	void print_all_links();
	double get_mean_k();
	void get_degree_distribution(int hist[]);
	void export_igraph_edgelist(char* filename);
	std::unordered_set<int>& get_neighbors(int node); //returns size of neighborsArray



private:
	int n_of_links;
	int size;
	std::unordered_set<int>* net;
	int* k;
	double p;
    pcg rand;
	std::uniform_int_distribution<int> dist;
	void draw_links();
	bool insert_link(std::pair<std::unordered_set<int>::iterator,bool> ret);
};

#endif /* NETWORK_H_ */
