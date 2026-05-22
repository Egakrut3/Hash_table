#include "Common.hpp"

#define CRC_MOD 0x82F63B78
static size_t crc32_hash(char const *str) {
	assert(str);

	uint32_t crc = 0;
	while (*str) {
		crc ^= (uint32_t)*str;

		for (size_t bt = 0; bt < CHAR_BIT; bt++) {
			if (crc & 1) {
				crc = crc >> 1 ^ CRC_MOD;
			}
			else {
				crc = crc >> 1;
			}
		}

		str++;
	}

	return crc;
}

#define POLY_EXP 473
static size_t polinomial_hash(char const *str) {
	assert(str);

	size_t hash = 0;
	while (*str) {
		hash = hash * POLY_EXP + (size_t)*str;

		str++;
	}

	return hash;
}

static size_t sum_hash(char const *str) {
	assert(str);

	size_t hash = 0;
	while (*str) {
		hash += (unsigned char)*str;

		str++;
	}

	return hash;
}

static size_t len_hash(char const *const str) {
	assert(str);

	return strlen(str);
}

typedef size_t (*hash_func_t)(char const *key);

#define HASH_FUNC_CNT 4
hash_func_t hash_func[HASH_FUNC_CNT] = {	&crc32_hash,
						&polinomial_hash,
						&sum_hash,
						&len_hash,};

#define BUCKETS_CNT ((size_t)0x2'00'00)
size_t buckets[BUCKETS_CNT][HASH_FUNC_CNT] = {};

int main() {
	#define FINAL_CODE

	size_t size = 0;
	scanf("%zu\n", &size);
	char *buffer = nullptr;
	ALLOCU_ARR(buffer, size);
	char *const buffer_end = buffer + size;
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREEU_ARR(buffer, size);

	fread(buffer, sizeof(buffer), size, stdin);
	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		if (*ptr == '\n') { *ptr = 0; }
	}

	for (char *ptr = buffer; ptr != buffer_end; ptr++) {
		for (size_t i = 0; i < HASH_FUNC_CNT; i++) {
			size_t bucket = hash_func[i](ptr) % BUCKETS_CNT;
			buckets[bucket][i]++;
		}

		while (*ptr) { ptr++; }
	}

	printf(	"%s,"
		"%s,"
		"%s,"
		"%s,"
		"\n",
		"CRC32",
		"Polynomial",
		"Sum",
		"Length");
	for (size_t bucket = 0; bucket < BUCKETS_CNT; bucket++) {
		for (size_t i = 0; i < HASH_FUNC_CNT; i++) {
			printf("%zu,", buckets[bucket][i]);
		}
		printf("\n");
	}

	CLEAR_RESOURCES();
	return 0;
}
