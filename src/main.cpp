#include "Common.h"
#include "HT_test.h"

// TODO - Add My_functions

#define MOD 0x82F63B78
static size_t crc32_hash([[maybe_unused]] struct Hash_gen *const gen, char const *str) {
	assert(str);
	assert(!gen);

	uint32_t crc = 0;
	while (*str) {
	#if HT_OPTIMIZATION > 0
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

int main(int argc, char const **argv) {
	assert(argc == 3);

	#define HT_TEST_BUCKETS	((size_t)0x2'00'00)
	#define HT_TEST_REPEAT	((size_t)5)
	CHECK_PROC(HT_test_report, crc32_hash, nullptr, HT_TEST_BUCKETS, HT_TEST_REPEAT, argv[1], argv[2]);

	return 0;
}

#undef FINAL_CODE
