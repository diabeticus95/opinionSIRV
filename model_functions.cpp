#include "model_functions.h"
#include "Network.h"
#include <vector>

void init_state(char* state_arr, int* opinions, int size){
	std::vector<int> vaxxers = get_vaxxers(opinions, size);
	for (int i = 1; i < size; i++){
		state_arr[i] = 'S';
	}
	for (auto i : vaxxers){
		state_arr[i] = 'V';
	}
	state_arr[0] = 'I';

}

void init_opinions(int* op_arr, int size, std::mt19937 mt){
    std::uniform_int_distribution<int> dist(0, 3);
    for (int i = 0; i < size; i++){
    	int rnd = dist(mt);
    	if(rnd == 0) op_arr[i] = -2;
    	else if(rnd == 1) op_arr[i] = -1;
    	else if(rnd == 2) op_arr[i] = 1;
    	else if(rnd == 3) op_arr[i] = 2;
    }
}

std::vector<int> get_vaxxers(int* opinions_arr, int size){
	std::vector<int> vaxxers;
	for(int i = 0; i < size; i++){
		if(opinions_arr[i] == 2){
			vaxxers.push_back(i);
		}
	}
	return vaxxers;
}
