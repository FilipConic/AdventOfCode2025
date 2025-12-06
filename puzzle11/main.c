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
	}
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
		size_t acc = op == OP_ADD ? 0 : 1;
		for (size_t j = 0; j < val_count; ++j) {
			if (op == OP_ADD) {
				acc += values[len * j + i];
			} else {
				acc *= values[len * j + i];
			}
		}
		sum += acc;
	}
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
