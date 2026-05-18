#include "Hash_table_test.hpp"

static int HT_test_fill(struct Hash_table *const ht, char const *const keys_path) {
	#define FINAL_CODE

	assert(keys_path);

	FILE *const input = fopen(keys_path, "r");
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(input);

	size_t size = 0;
	fscanf(input, "%zu\n", &size);
	char *buffer = nullptr;
	ALLOC_ARR(buffer, size);
	char *const buffer_end = buffer + size;
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREE_ARR(buffer, size);	\
	fclose(input);

	fread(buffer, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))), size, input);
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		if (*ptr == '\n') { *ptr = 0; }
	}

	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		CHECK_PROC(Hash_table_insert, ht, ptr);

		while (*ptr) { ptr++; }
	}

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

static int HT_test_query(struct Hash_table const *const ht, char const *const queries_path) {
	#define FINAL_CODE

	assert(queries_path);

	FILE *const input = fopen(queries_path, "r");
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(input);

	size_t size = 0;
	fscanf(input, "%zu\n", &size);
	char *buffer = nullptr;
	ALLOC_ARR(buffer, size);
	char *const buffer_end = buffer + size;
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREE_ARR(buffer, size);	\
	fclose(input);

	fread(buffer, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))), size, input);
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		if (*ptr == '\n') { *ptr = 0; }
	}

	size_t tot_found = 0;
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		bool found = false;
		CHECK_PROC(Hash_table_find, ht, &found, ptr);
		tot_found += found;

		while (*ptr) { ptr++; }
	}
	fprintf(stderr, "%zu\n", tot_found);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}

int HT_test_report(HT_hash_func_t hash, size_t const buckets_cnt, char const *const keys_path, char const *const queries_path) {
	#define FINAL_CODE

	struct Hash_table ht = {};
	CHECK_PROC(Hash_table_ctor, &ht, hash, buckets_cnt);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	Hash_table_dtor(&ht);

	CHECK_PROC(HT_test_fill, &ht, keys_path);
	CHECK_PROC(HT_test_query, &ht, queries_path);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}
