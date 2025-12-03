#include <limits.h>
#include <stdio.h>

#define CYLIBX_IMPLEMENTATION
#define CYLIBX_STRIP_PREFIX
#include "../cylibx.h"

#define DIGIT_COUNT 12
static size_t pos[DIGIT_COUNT] = { 0 };
size_t find_in_row(char* row) {
	memset(pos, 0, DIGIT_COUNT * sizeof(size_t));
	for (size_t j = 0; j < DIGIT_COUNT; ++j) {
		if (j) { pos[j] = pos[j - 1] + 1; }
		for (size_t i = pos[j]; i < str_length(row) - (DIGIT_COUNT - 1 - j); ++i) {
			if (row[pos[j]] < row[i]) {
				pos[j] = i;
			}
		}
	}
	size_t val = 0;
	for (size_t i = 0; i < DIGIT_COUNT; ++i) {
		val *= 10;
		val += row[pos[i]] - '0';
	}
	return val;
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
