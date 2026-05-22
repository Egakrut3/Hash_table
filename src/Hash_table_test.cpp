#include "Hash_table_test.hpp"

static int HT_test_fill(struct Hash_table *const ht, FILE *const input) {
	#define FINAL_CODE

	assert(input);

	size_t size = 0;
	fscanf(input, "%zu\n", &size);
	char *buffer = nullptr;
	ALLOCU_ARR(buffer, size);
	char *const buffer_end = buffer + size;
	#undef FINAL_CODE
	#define FINAL_CODE		\
	FREEU_ARR(buffer, size);

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

static int HT_test_query(struct Hash_table const *const ht, FILE *const input) {
	#define FINAL_CODE

	assert(input);

	size_t size = 0;
	fscanf(input, "%zu\n", &size);
	char *buffer = nullptr;
	ALLOCU_ARR(buffer, size);
	#undef FINAL_CODE
	#define FINAL_CODE		\
	FREEU_ARR(buffer, size);
	char *const buffer_end = buffer + size;

	fread(buffer, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))), size, input);

#if HT_OPTIMIZATION > 0
	size_t str_cnt = 0;
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		str_cnt++;

		while (*ptr != '\n') { ptr++; }
		*ptr = '\0';
	}

	char *aligned_buffer = nullptr;
	CALLOCA_ARR(aligned_buffer, CFFL_ALIGNEMENT, str_cnt * CFFL_MAX_STR_LEN);
	#undef FINAL_CODE
	#define FINAL_CODE							\
	FREEA_ARR(aligned_buffer, CFFL_ALIGNEMENT, str_cnt * CFFL_MAX_STR_LEN);	\
	FREEU_ARR(buffer, size);
	char *const aligned_buffer_end = aligned_buffer + str_cnt * CFFL_MAX_STR_LEN;

	for (char *ptr = buffer, *aligned_ptr = aligned_buffer; ptr != buffer_end; ptr++, aligned_ptr += CFFL_MAX_STR_LEN) {
		strcpy(aligned_ptr, ptr);

		while (*ptr) { ptr++; }
	}

	size_t tot_found = 0;
	for (char *ptr = aligned_buffer; ptr != aligned_buffer_end; ptr += CFFL_MAX_STR_LEN) {
		bool found = false;
		CHECK_PROC(Hash_table_find, ht, &found, ptr);
		tot_found += found;
	}
	fprintf(stderr, "%zu\n", tot_found);
#else
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		if (*ptr == '\n') { *ptr = '\0'; }
	}

	size_t tot_found = 0;
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		bool found = false;
		CHECK_PROC(Hash_table_find, ht, &found, ptr);
		tot_found += found;

		while (*ptr) { ptr++; }
	}
	fprintf(stderr, "%zu\n", tot_found);
#endif

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

	FILE *input = fopen(keys_path, "r");
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(input);		\
	Hash_table_dtor(&ht);
	CHECK_PROC(HT_test_fill, &ht, input);
	fclose(input);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	Hash_table_dtor(&ht);

	input = fopen(queries_path, "r");
	#undef FINAL_CODE
	#define FINAL_CODE	\
	fclose(input);		\
	Hash_table_dtor(&ht);
	CHECK_PROC(HT_test_query, &ht, input);
	fclose(input);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	Hash_table_dtor(&ht);

	CLEAR_RESOURCES();
	return 0;

	#undef FINAL_CODE
}
