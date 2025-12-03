#include <limits.h>
#include <stdio.h>

#define CYLIBX_IMPLEMENTATION
#define CYLIBX_STRIP_PREFIX
#include "../cylibx.h"

size_t find_in_row(char* row) {
	size_t largest_pos1 = 0;
	for (size_t i = 1; i < str_length(row) - 1; ++i) {
		if (row[largest_pos1] < row[i]) {
			largest_pos1 = i;
		}
	}
	size_t largest_pos2 = largest_pos1 + 1;
	for (size_t i = largest_pos1 + 1; i < str_length(row); ++i) {
		if (row[largest_pos2] < row[i]) {
			largest_pos2 = i;
		}
	}
	return (row[largest_pos1] - '0') * 10 + row[largest_pos2] - '0';
}
size_t calculate_joltage(char* file) {
	char** rows = array_new(char*, .is_ptr = 1, .defer_fn = str_free);

	size_t start = 0;
	for (size_t i = 0; i < str_length(file) - 1; ++i) {
		if (file[i] == '\n') {
			array_append(rows, str_from_lit_n(file + start, i - start));
			start = i + 1;
		}
	}
	size_t sum = 0;
	array_foreach(row, rows) {
		sum += find_in_row(*row.value);
	}

	array_free(rows);
	return sum;
}

int main() {
	char* file = str_from_file("./input.txt");
	printf("Final result is: %zu\n", calculate_joltage(file));
	str_free(file);
	return 0;
}
