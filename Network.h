#ifndef NETWORK_H_
#define NETWORK_H_
#include <random>

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
	const int size;
	int **net;
	double p;
    std::mt19937_64 mt;
	std::uniform_real_distribution<double> dist;

};

#endif /* NETWORK_H_ */
