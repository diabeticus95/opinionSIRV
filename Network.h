#ifndef NETWORK_H_
#define NETWORK_H_
#include <random>
#include <vector>

class Network {
public:
	Network(int size, double p);
	virtual ~Network();
	void print_node(int i);
	void print_all_nodes();
	void debug_print();
//	Network(const Network &other);
//	Network(Network &&other);
private:
	int size;
	std::vector<int> net;
	int* k;
	int* l_seg;
	double p;
    std::mt19937_64 mt;
	std::uniform_real_distribution<double> dist;

	void draw_node_connections(int* tmp, int i);
	int push_node(int* tmp);
	int retrieve_link(int i, int j);
	int track_node_by_index_in_net(int i);
};

#endif /* NETWORK_H_ */
