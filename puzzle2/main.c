#include <stdio.h>
#include <stdlib.h>

#define CYLIBX_IMPLEMENTATION
#include "../cylibx.h"

#define mod(a, b) ((a % b + b) % b)

size_t parse_file(char* file) {
	int curr = 50;
	size_t counter = 0;

	char buffer[16];
	for (size_t i = 0; i < cyx_str_length(file); ++i) {
		int side = 0;

		if (file[i] == 'L') {
			side = -1;
		} else if (file[i] == 'R') {
			side = 1;
		} else {
			assert(0 && "UNREACHABLE");
		}
		++i;

		size_t c = 0;
		for (; file[i] != '\n' && i < cyx_str_length(file); ++c, ++i) {
			buffer[c] = file[i];
		}
		buffer[c] = '\0';

		int num = side * atol(buffer);
		
		int curr_num = curr + num;
		if (curr_num < 0) {
			if (curr > 0) {
				counter++;
			} 	
			counter += (-curr_num) / 100;
		} else if (curr_num >= 100) {
			counter += curr_num / 100;
		} else if (!curr_num) {
			counter++;
		}
		curr = mod(curr_num, 100);
	}

	return counter;
}

int main() {
	char* file = cyx_str_from_file("input.txt");
	printf("Result is: %zu\n", parse_file(file));
	cyx_str_free(file);
	return 0;
}
