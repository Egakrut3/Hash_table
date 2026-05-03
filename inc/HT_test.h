#ifndef HT_TEST
#define HT_TEST

#include "Hash_table.h"

#define HT_TEST_BUCKETS ((size_t)0x2'00'00)

int ht_test_fill(struct Hash_table *ht);

int ht_test_query(struct Hash_table const *ht);

#endif