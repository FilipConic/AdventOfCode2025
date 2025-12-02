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

void test_repeats(size_t* sum) {
	if (range.num1 > range.num2) { return; }
	size_t digit_count1 = (size_t)(floor(log10(range.num1)) + 1);
	size_t digit_count2 = (size_t)(floor(log10(range.num2)) + 1);
	size_t divisor1 = (size_t)pow(10, (size_t)(digit_count1 / 2));
	size_t divisor2 = (size_t)pow(10, (size_t)(digit_count2 / 2));

	size_t start_from = range.num1;
	if (digit_count1 % 2 == 1) {
		if (digit_count1 == digit_count2) { return; }
		start_from = (size_t)(pow(10, digit_count1));
	}
	printf("%zu - %zu:\n", range.num1, range.num2);
	printf("digit_counts: %zu - %zu\n", digit_count1, digit_count2);
	printf("start_from: %zu\n", start_from);
	for (size_t i = start_from; i <= range.num2; ++i) {
		for (size_t divisor = divisor1; divisor <= divisor2; divisor *= 10) {
			size_t quot = i / divisor;
			size_t rem = i % divisor;
			if (rem == quot) {
				 printf("value: %zu, divisor: %zu, quot: %zu and rem: %zu\n", i, divisor, quot, rem);
				 *sum += i;
			}
		}
	}
}
size_t parse_ids(char* str) {
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

	return sum;
}

int main() {
	char* file = str_from_file("./input.txt");
	printf("Final sum is: %zu\n", parse_ids(file));
	str_free(file);
	return 0;
}
