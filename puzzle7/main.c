#include <stdio.h>

#define CYLIBX_STRIP_PREFIX
#define CYLIBX_IMPLEMENTATION
#include "../cylibx.h"

#define min(a, b) ((int)(a) < (int)(b) ? (int)(a) : (int)(b))
#define max(a, b) ((int)(a) > (int)(b) ? (int)(a) : (int)(b))

size_t count_rolls(char* rolls, size_t width, size_t height) {
	// for (size_t j = 0; j < height; ++j) {
	// 	for (size_t i = 0; i < width; ++i) {
	// 		printf("%c", rolls[j * width + i] ? '1' : '0');
	// 	}
	// 	printf("\n");
	// }
	size_t sum = 0;
	for (int j = 0; j < (int)height; ++j) {
		for (int i = 0; i < (int)width; ++i) {
			if (rolls[j * width + i]) {
				size_t around = 0;
				for (int y = max(0, j - 1); y <= min(height - 1, j + 1); ++y) {
					for (int x = max(0, i - 1); x <= min(width - 1, i + 1); ++x) {
						if (x == i && y == j) { continue; }
						if (rolls[y * width + x]) { ++around; }
					}
				}
				if (around < 4) { ++sum; }
			}
		}
	}
	return sum;
}
size_t parse_rolls(char* file) {
	char* rolls = array_new(char);
	size_t width = 0;
	size_t height = 0;
	char found_width = 0;

	for (size_t i = 0; i < str_length(file); ++i) {
		if (file[i] == '\n') {
			found_width = 1;
			++height;
		} else {
			if (!found_width) { ++width; }
			array_append(rolls, file[i] == '@' ? 1 : 0);
		}
	}

	size_t sum = count_rolls(rolls, width, height);
	array_free(rolls);
	return sum;
}

int main() {
	char* file = str_from_file("input.txt");
	printf("Final result is: %zu\n", parse_rolls(file));
	str_free(file);
	return 0;
}
