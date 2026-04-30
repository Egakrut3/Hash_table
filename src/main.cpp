#include "Common.h"
#include "Hash_table.h"

// TODO - Add My_functions

#define MOD 0x82F63B78
static size_t crc32_hash([[maybe_unused]] void *gen, char *str) {
	assert(str);

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

int main() {
	struct Hash_table ht = {};
	Hash_table_ctor(&ht, crc32_hash, nullptr, 0);

	char str[] = "abc";

	printf("%d\n", Hash_table_find(&ht, str));
	Hash_table_insert(&ht, str);
	printf("%d\n", Hash_table_find(&ht, str));

	Hash_table_dtor(&ht);
	return 0;
}