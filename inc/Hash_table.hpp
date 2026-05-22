#ifndef HASH_TABLE
#define HASH_TABLE

#include "Common.hpp"
#include "CF_forward_list.hpp"



typedef CFFL_arg_t HT_arg_key_t;
typedef CFFL_mem_t HT_mem_key_t;

#define HT_KEY_COPY(dest, src)	CFFL_VAL_COPY(dest, src)
#define HT_KEY_FREE(key)	CFFL_VAL_FREE(key)

#if HT_OPTIMIZATION > 0

extern "C" int my_strcmp(HT_arg_key_t a, HT_arg_key_t b);
#define HT_KEY_EQUAL(a, b)	(!my_strcmp(a, b))

#else

#define HT_KEY_EQUAL(a, b)	(!strcmp(a, b))

#endif



typedef size_t (*HT_hash_func_t)(HT_arg_key_t key);

struct Hash_table {
	HT_hash_func_t		hash;

	size_t			buckets_cnt;
	struct CFFL		*buckets;
};

int Hash_table_ctor(struct Hash_table *ht, HT_hash_func_t hash, size_t buckets_cnt);
int Hash_table_dtor(struct Hash_table *ht);

int Hash_table_insert(struct Hash_table *ht, HT_arg_key_t key);
int Hash_table_find(struct Hash_table const *ht, bool *found, HT_arg_key_t key);

#endif
