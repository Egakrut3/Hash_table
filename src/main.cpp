#include "Common.hpp"
#include "Hash_table_test.hpp"
#include <immintrin.h>

// TODO - Add My_functions

#define MOD 0x82F63B78
static size_t crc32_hash(char const *str) {
	assert(str);

	uint32_t crc = 0;
	while (*str) {
	#if HT_OPTIMIZATION > 1
		crc = _mm_crc32_u8(crc, (unsigned char)*str);
	#else
		crc ^= (uint32_t)*str;

		for (size_t bt = 0; bt < CHAR_BIT; bt++) {
			if (crc & 1) {
				crc = crc >> 1 ^ MOD;
			}
			else {
				crc = crc >> 1;
			}
		}
	#endif

		str++;
	}

	return crc;
}

#define FINAL_CODE

int main(int argc, char const *argv[]) {
	assert(argc == 3);

	#define HT_TEST_BUCKETS	((size_t)0x2'00'00)
	CHECK_PROC(HT_test_report, crc32_hash, HT_TEST_BUCKETS, argv[1], argv[2]);

	return 0;
}

#undef FINAL_CODE
