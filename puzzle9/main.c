#include <stdio.h>
#include <stdlib.h>

#define CYLIBX_STRIP_PREFIX
#define CYLIBX_IMPLEMENTATION
#include "../cylibx.h"

typedef struct {
	size_t start;
	size_t end;
} Range;

size_t test_freshness(Range* ranges, size_t num) {
	Range* r = ranges;
	for (size_t i = 0; i < array_length(ranges); ++i) {
		if (r->start <= num && num <= r->end) { return 1; }
		++r;
	}
	return 0;
}
size_t count_fresh(char* file) {
	size_t count = 0;
	Range* ranges = array_new(Range);
	
	char prev_is_nl = 0;
	char buffer[64] = { 0 };
	size_t len = 0;

	size_t i = 0;
	Range r = { 0 };
	for (; i < str_length(file); ++i) {
		if (file[i] == '\n') {
			if (prev_is_nl) {
				++i;
				break;
			} else {
				buffer[len] = '\0';
				r.end = atol(buffer);
				len = 0;
				array_append(ranges, r);

				r.start = 0;
				r.end = 0;
				prev_is_nl = 1;
			}
		} else {
			if (file[i] == '-') {
				buffer[len] = '\0';
				r.start = atol(buffer);
				len = 0;
			} else {
				buffer[len++] = file[i];
			}
			prev_is_nl = 0;
		}
	}
	for (; i < str_length(file); ++i) {
		if (file[i] == '\n') {
			buffer[len] = '\0';
			len = 0;
			size_t num = atol(buffer);
			count += test_freshness(ranges, num);
		} else {
			buffer[len++] = file[i];
		}
	}
	
	array_free(ranges);
	return count;
}

int main() {
	char* file = str_from_file("input.txt");
	printf("Final result is: %zu\n", count_fresh(file));
	str_free(file);
	return 0;
}
