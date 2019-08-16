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

	void draw_node_connections(int* tmp);
	int push_node(int* tmp);

};

#endif /* NETWORK_H_ */
