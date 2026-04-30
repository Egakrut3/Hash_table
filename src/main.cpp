#include "Common.h"
#include "HT_test.h"

// TODO - Add My_functions

#define MOD 0x82F63B78
static size_t crc32_hash([[maybe_unused]] void *gen, char const *str) {
	assert(!gen); assert(str);

	size_t len = strlen(str);
	uint32_t crc = 0;
	for (size_t i = 0; i < len; i++) {
		crc ^= (uint32_t)str[i];

		for (size_t bt = 0; bt < CHAR_BIT; bt++) {
			if (crc & 1) {
				crc = crc >> 1 ^ MOD;
			} else {
				crc = crc >> 1;
			}
		}
	}

	return crc;
}

#define FINAL_CODE

int main() {
	struct Hash_table ht = {};
	CHECK_PROC(Hash_table_ctor, &ht, crc32_hash, nullptr, 0, HT_TEST_BUCKETS);

	CHECK_PROC(ht_test_fill, &ht);
	CHECK_PROC(ht_test_query, &ht);

	CHECK_PROC(Hash_table_dtor, &ht);
	return 0;
}

#undef FINAL_CODE