#include "HT_test.h"

#define FINAL_CODE

static int HT_test_fill(struct Hash_table *const ht, char const *const keys_path) {
	assert(ht); assert(keys_path);

	FILE *const input = fopen(keys_path, "r");

	size_t size = 0;
	fscanf(input, "%zu\n", &size);
	char *buffer = (TYPEOF_UNQUAL(buffer))calloc(size, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))));

	fread(buffer, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))), size, input);
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

static int HT_test_query(struct Hash_table const *const ht, char const *const queries_path) {
	assert(ht); assert(queries_path);

	FILE *const input = fopen(queries_path, "r");

	size_t size = 0;
	fscanf(input, "%zu\n", &size);
	char *buffer = (TYPEOF_UNQUAL(buffer))calloc(size, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))));

	fread(buffer, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))), size, input);
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

	size_t const spent_cyc = cur_cyc - last_cyc;
	fprintf(stderr,	"%zu queries passed, %zu found\n"
			"Total %zu cycles spent = %g CPQ\n",
			tot_queries, tot_found,
			spent_cyc, (double)spent_cyc / (double)size);

	free(buffer);
	fclose(input);

	return 0;
}

int HT_test_report(HT_hash_func_t hash, struct Hash_gen *gen, size_t buckets, size_t repeat, char const *keys_path, char const *queries_path) {
	for (size_t it = 0; it < repeat; it++) {
		struct Hash_table ht = {};
		CHECK_PROC(Hash_table_ctor, &ht, hash, gen, buckets);

		CHECK_PROC(HT_test_fill, &ht, keys_path);
		CHECK_PROC(HT_test_query, &ht, queries_path);

		CHECK_PROC(Hash_table_dtor, &ht);
	}

	return 0;
}

#undef FINAL_CODE
