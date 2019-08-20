#ifndef MODEL_FUNCTIONS_H_
#define MODEL_FUNCTIONS_H_

#include <random>
#include <vector>

void init_state(char* state_arr, int* opinions, int size);
void init_opinions(int* opinions_arr, int size, std::mt19937 mt);
std::vector<int> get_vaxxers(int* opinions_arr, int size);


#endif /* MODEL_FUNCTIONS_H_ */
