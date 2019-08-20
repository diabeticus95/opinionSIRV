#include "model_functions.h"
#include "Network.h"
#include <iostream>

void init_state(char* state_arr, int size){
	state_arr[0] = 'I';
	for (int i = 1; i < size; i++){
		state_arr[i] = 'S';
	}
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
