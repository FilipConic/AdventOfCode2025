#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CYLIBX_IMPLEMENTATION
#define CYLIBX_STRIP_PREFIX
#include "../cylibx.h"

struct {
	size_t num1;
	size_t num2;
} range;

typedef struct {
	size_t key;
	size_t* value;
} KV;

KV* cache = NULL;

/*
 * 2  => 11
 * 3  => 111 
 * 4  => 101 1111
 * 5  => 11111
 * 6  => 1001 10101 111111
 * 7  => 1111111
 * 8  => 10001 1001001 1010101 11111111
 * 9  => 1001001 111111111
 * 10 => 100001 101010101 1111111111
 * */
size_t curr_digit_count = 0;
void map_fn(void* dst, const void* src) {
	assert(curr_digit_count >= 2);
	size_t val = *(size_t*)src;
	size_t res = 0;
	size_t parts = curr_digit_count / val;
	for (size_t i = 0; i < val; ++i) {
		res = res * pow(10, parts) + 1;
	}
	*(size_t*)dst = res;
}
size_t* set_divisors(size_t digit_count) {
	assert(digit_count >= 2);
	curr_digit_count = digit_count;

	size_t* divisors = cyx_array_new(size_t);
	for (size_t i = 2; i <= digit_count; ++i) {
		if (digit_count % i == 0) {
			cyx_array_append(divisors, i);
		}
	}
	cyx_hashmap_add_v(cache, digit_count, cyx_array_map_self(divisors, map_fn));

	return divisors;
}
void test_repeats(size_t* sum) {
	if (range.num1 > range.num2) { return; }
	for (size_t i = range.num1; i <= range.num2; ++i) {
		size_t digit_count = (size_t)(floor(log10(i)) + 1);
		if (digit_count == 1) { continue; }
		size_t** divisor_ptr = cyx_hashmap_get(cache, digit_count);
		size_t* divisors;
		if (!divisor_ptr) {
			divisors = set_divisors(digit_count);
		} else {
			divisors = *divisor_ptr;
		}
		cyx_array_foreach(val, divisors) {
			size_t quot = i / *val.value;
			if (i == quot * *val.value) {
				// printf("num: %zu, digit_count: %zu, divisor: %zu, quot: %zu\n", i, digit_count, *val.value, quot);
				*sum += i;
				break;
			}
		}
	}
}
size_t parse_ids(char* str) {
	if (!cache) {
		cache = cyx_hashmap_new(KV, hash_size_t, eq_int64, .is_value_ptr = 1, .defer_value_fn = cyx_array_free);
	}
	char buffer[256] = { 0 };
	size_t counter = 0;
	size_t sum = 0;
	char working_on = 0;
	for (size_t i = 0; i < str_length(str); ++i) {
		char c = str[i];
		if (c == '-' || c == ',') {
			if (working_on) {
				range.num2 = atol(buffer);
				test_repeats(&sum);
			} else {
				range.num1 = atol(buffer);
			}
			working_on = !working_on;
			counter = 0;
			memset(buffer, 0, 256);
		} else {
			buffer[counter++] = c;
		}
	}
	if (working_on) {
		range.num2 = atol(buffer);
		test_repeats(&sum);
	} else {
		range.num1 = atol(buffer);
	}
	cyx_hashmap_free(cache);

	return sum;
}

int main() {
	char* file = str_from_file("./input.txt");
	printf("Final sum is: %zu\n", parse_ids(file));
	str_free(file);
	return 0;
}
