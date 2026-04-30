#include "HT_test.h"

int ht_test_fill(struct Hash_table *const ht) {
	FILE *const input = fopen("data/keys.txt", "r");
	size_t size = 0;
	fscanf(input, "%zu\n", &size);

	char *buffer = (char *)calloc(size, sizeof(*buffer));
	fread(buffer, sizeof(*buffer), size, input);
	for (size_t i = 0; i < size; i++) {
		if (buffer[i] == '\n') { buffer[i] = '\0'; }
	}

	for (size_t i = 0; i < size; i++) {
		Hash_table_insert(ht, buffer + i);

		while (buffer[i] != '\0') { i++; }
	}

	free(buffer);
	fclose(input);

	return 0;
}