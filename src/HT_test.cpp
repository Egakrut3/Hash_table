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

int ht_test_query(struct Hash_table const *const ht) {
	assert(ht);

	FILE *const input = fopen("data/keys.txt", "r");
	size_t size = 0;
	fscanf(input, "%zu\n", &size);

	char *buffer = (char *)calloc(size, sizeof(*buffer));
	fread(buffer, sizeof(*buffer), size, input);
	for (size_t i = 0; i < size; i++) {
		if (buffer[i] == '\n') { buffer[i] = '\0'; }
	}

	size_t	tot_queries	= 0,
		tot_found	= 0,
		last_cyc	= 0,
		cur_cyc		= 0;
	__asm__ volatile ("mfence" ::: "memory");
	last_cyc = __rdtsc();
	for (size_t i = 0; i < size; i++) {
		tot_queries++;
		tot_found += Hash_table_find(ht, buffer + i);

		while (buffer[i] != '\0') { i++; }
	}
	__asm__ volatile ("mfence" ::: "memory");
	cur_cyc = __rdtsc();

	size_t spent_cyc = cur_cyc - last_cyc;
	fprintf(stderr,	"%zu queries passed, %zu found\n"
			"Total %zu cycles spent = %g CPQ\n",
			tot_queries, tot_found, spent_cyc, (double)spent_cyc / (double)size);

	free(buffer);
	fclose(input);
	return 0;
}