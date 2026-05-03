#ifndef HT_TEST
#define HT_TEST

#include "Hash_table.h"

int HT_test_report(HT_hash_func_t hash, struct Hash_gen *gen, size_t buckets, size_t repeat, char const *keys_path, char const *queries_path);

#endif
