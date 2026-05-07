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
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREE_ARR(buffer, size);	\
	fclose(input);

	fread(buffer, sizeof(REMOVE_POINTER(LVAL_TYPEOF(buffer))), size, input);
	for (size_t i = 0; i < size; i++) {
		if (buffer[i] == '\n') { buffer[i] = '\0'; }
	}

	for (size_t i = 0; i < size; i++) {
		Hash_table_insert(ht, buffer + i);

		while (buffer[i] != '\0') { i++; }
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
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREE_ARR(buffer, size);	\
	fclose(input);

	fread(buffer, sizeof(REMOVE_POINTER(LVAL_TYPEOF(buffer))), size, input);
	for (size_t i = 0; i < size; i++) {
		if (buffer[i] == '\n') { buffer[i] = '\0'; }
	}

	for (size_t i = 0; i < size; i++) {
		Hash_table_find(ht, buffer + i);

		while (buffer[i]) { i++; }
	}

	CLEAR_RESOURCES();

	return 0;

	#undef FINAL_CODE
}

int HT_test_report(HT_hash_func_t hash, struct Hash_gen *const gen, size_t const buckets, char const *const keys_path, char const *const queries_path) {
	#define FINAL_CODE

	struct Hash_table ht = {};
	CHECK_PROC(Hash_table_ctor, &ht, hash, gen, buckets);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	Hash_table_dtor(&ht);

	CHECK_PROC(HT_test_fill, &ht, keys_path);
	CHECK_PROC(HT_test_query, &ht, queries_path);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}
