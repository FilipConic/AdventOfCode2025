#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define CYLIBX_STRIP_PREFIX
#define CYLIBX_IMPLEMENTATION
#include <cylibx.h>

size_t cyx_str_count_char(char* str, char c);
#define str_count_char cyx_str_count_char
size_t cyx_str_count_char(char* str, char c) {
	size_t sum = 0;
	for (size_t i = 0; i < cyx_str_length(str); ++i) {
		if (str[i] == c) {
			++sum;
		}
	}
	return sum;
}

typedef enum {
	OP_ERROR,
	OP_MULT,
	OP_ADD,
} Operator;

size_t calculate(char* file) {
	size_t line_count = str_count_char(file, '\n');
	size_t sum = 0;

	char buffer[64] = { 0 };
	size_t buf_count = 0;
	size_t* values = array_new(size_t);
	Operator* ops = array_new(Operator);

	size_t lines_counted = 0;
	size_t row_len = 0;
	for (size_t i = 0; i < cyx_str_length(file); ++i) {
		if (file[i] != '\n' && file[i] != ' ') {
			if (lines_counted != line_count - 1) {
				buffer[buf_count++] = file[i];
			} else {
				array_append(ops, file[i] == '+' ? OP_ADD : (file[i] == '*' ? OP_MULT : OP_ERROR));
			}
		} else {
			if (file[i] == '\n') { ++lines_counted; }
			if (lines_counted != line_count - 1 && buf_count != 0) {
				assert(buf_count < 64);
				buffer[buf_count] = '\0';
				buf_count = 0;
				array_append(values, atol(buffer));
			}
		}
		if (!lines_counted) { ++row_len; }
	}
	++row_len;
	// for (size_t i = 0; i < array_length(values); ++i) {
	// 	printf("%zu\n", values[i]);
	// }
	// for (size_t i = 0; i < array_length(ops); ++i) {
	// 	printf("%s\n", ops[i] == OP_ADD ? "ADD" : (ops[i] == OP_MULT ? "MULT" : "ERROR"));
	// }
	size_t val_count = line_count - 1;
	size_t len = array_length(values) / val_count;
	// printf("%zu, %zu\n", val_count, array_length(values));
	// printf("%zu, %zu\n", len, array_length(ops));
	assert(len == array_length(ops));

	for (size_t i = 0; i < len; ++i) {
		Operator op = ops[i];
		assert(op != OP_ERROR);
		size_t digit_count = 0;
		for (size_t j = 0; j < val_count; ++j) {
			size_t d = log10(values[len * j + i]) + 1;
			if (digit_count < d) { digit_count = d; }
		}
		values[i] = digit_count;
	}

	size_t* new_values = array_new(size_t);
	size_t passed_count = 0;
	for (size_t i = 0; i < len; ++i) {
		size_t digit_count = values[i];
		size_t added_count = 0;
		for (size_t digit = 0; digit < digit_count; ++digit) {
			for (size_t row = 0; row < val_count; ++row) {
				char c = file[row * row_len + digit + passed_count];
				// printf("(row, pos): (%zu, %zu) = %c\n", row + 1, digit + passed_count + 1, c);
				if (c != ' ') {
					buffer[buf_count++] = c;
				}
			}
			if (buf_count) {
				buffer[buf_count] = '\0';
				buf_count = 0;
				array_append(new_values, atol(buffer));
				++added_count;
			}
		}
		values[i + len] = added_count;
		passed_count += 1 + digit_count;
	}

	size_t* curr = new_values;
	for (size_t i = 0; i < len; ++i) {
		// size_t digit_count = values[i];
		size_t added = values[i + len];
		// printf("i: %zu, digit_count: %zu, added: %zu\n", i, digit_count, added);
		Operator op = ops[i];
		assert(op != OP_ERROR);
		size_t acc = op == OP_ADD ? 0 : 1;
		for (size_t j = 0; j < added; ++j) {
			if (op == OP_ADD) {
				acc += *curr;
			} else {
				acc *= *curr;
			}
			// printf("curr: %zu\n", *curr);
			++curr;
		}
		sum += acc;
	}
	array_free(new_values);
	array_free(values);
	array_free(ops);

	return sum;
}

int main() {
	char* file = str_from_file("input.txt");
	printf("Final result is: %zu\n", calculate(file));
	str_free(file);
	return 0;
}
