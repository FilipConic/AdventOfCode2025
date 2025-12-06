#include <stdio.h>
#include <stdlib.h>

#define CYLIBX_STRIP_PREFIX
#define CYLIBX_IMPLEMENTATION
#include "../cylibx.h"

typedef struct {
	size_t start;
	size_t end;
} Range;
size_t hash_range(const void* val) {
	Range* r = (void*)val;
	size_t v = r->start ^ r->end;
	return hash_size_t(&v);
}
int eq_range(const void* a, const void* b) { return ((Range*)a)->start == ((Range*)b)->start && ((Range*)a)->end == ((Range*)b)->end; }

/*
 * #########
 *   ####
 *
 *   ####
 * #########
 *
 * #####
 *   #####
 *
 *   #####
 * #####
 *
 * ###
 *     ###
 *
 *     ###
 * ###
 * */
Range merge_two_ranges(Range* r1, Range* r2, int* ret) {
	if (r1->start <= r2->start && r2->end <= r1->end) {
		*ret = 0;
		return *r1;
	} else if (r2->start <= r1->start && r1->end <= r2->end) {
		*ret = 0;
		return *r2;
	} else if (r1->start < r2->start && r2->start <= r1->end && r1->end <= r2->end) {
		*ret = 0;
		return (Range){ .start = r1->start, .end = r2->end };
	} else if (r2->start < r1->start && r1->start <= r2->end && r2->end <= r1->end) {
		*ret = 0;
		return (Range){ .start = r2->start, .end = r1->end };
	}
	*ret = 1;
	return (Range){ 0 };
}
void merge_ranges(Range* ranges) {
	Range* temp = hashset_new(Range, hash_range, eq_range);
	char merged = 1;
	for (size_t err = 0; err < 1000 && merged; ++err) {
		printf("Loop: %zu\n", err + 1);
		merged = 0;
		hashset_clear(temp);

		Range* r1 = ranges;
		for (size_t i = 0; i < array_length(ranges); ++i) {
			char merged_none = 1;
			Range* r2 = ranges;
			for (size_t j = 0; j < array_length(ranges); ++j) {
				if (i == j) { goto after; }
				int ret;
				Range res = merge_two_ranges(r1, r2, &ret);
				if (!ret) {
					merged_none = 0;
					merged = 1;
					hashset_add(temp, res);
					break;
				}
after:
				++r2;
			}
			if (merged_none) { hashset_add(temp, *r1); }
			++r1;
		}

		array_clear(ranges);
		hashset_foreach(val, temp) {
			array_append(ranges, *val);
		}
	}
	hashset_free(temp);
}
size_t count_ranges(Range* ranges) {
	size_t sum = 0;
	Range* r = ranges;
	for (size_t i = 0; i < array_length(ranges); ++i) {
		sum += r->end - r->start + 1;
		++r;
	}
	return sum;
}
size_t count_fresh(char* file) {
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
	
	printf("%zu\n", array_length(ranges));
	merge_ranges(ranges);
	size_t count = count_ranges(ranges);
	array_free(ranges);
	return count;
}

int main() {
	char* file = str_from_file("input.txt");
	printf("Final result is: %zu\n", count_fresh(file));
	str_free(file);
	return 0;
}
