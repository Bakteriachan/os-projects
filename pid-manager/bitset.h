#ifndef BITSET_H
#define BITSET_H
#include <string.h>
#include <stdio.h>

#define BITSET_MAP_SIZE 4700
#define MAX_PID 5000
#define MIN_PID 300

const unsigned int all_active_pids = 4294967295;
const unsigned int idx_size = sizeof(int) * 4;

unsigned int map[BITSET_MAP_SIZE];

void pid_to_idx(int *buff,int pid) {
	int idx = pid / idx_size;
	int bitset_idx = pid % idx_size;
	*buff = idx;
	*(buff + 1) = bitset_idx;
}

int allocate_map(void) {
	memset(map, 0, BITSET_MAP_SIZE);
	return 1;
} 

int allocate_pid() {
	for(int i = 0; i < BITSET_MAP_SIZE; i++) {
		if(map[i] != all_active_pids) {
			for(int k = 0; k < 32; k++)
				if(!(map[i] & (1<<k))) {
					map[i] = map[i] | (1<<k);
					return i * idx_size + k;
				}
			break;
		}
	}
	return -1;
}

void release_pid(unsigned int pid) {
	int idx[2];
	pid_to_idx(idx, pid);
	unsigned int aux = ~(1<<idx[1]);	
	map[idx[0]] = map[idx[0]] & aux;
}

#endif
