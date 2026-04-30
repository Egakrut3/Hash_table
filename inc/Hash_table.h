#ifndef HASH_TABLE
#define HASH_TABLE

#include "Common.h"

typedef char *ht_key_t;
#define HT_KEY_COPY(dest, src)	\
do {				\
	dest = strdup(src);	\
} while (false)
#define HT_KEY_EQUAL(a, b) (!strcmp(a, b))
#define HT_KEY_FREE(key)	\
do {				\
	free(key);		\
} while (false);

typedef size_t (*hash_func)(void *gen, ht_key_t key);

struct List_node {
	struct List_node	*next;
	ht_key_t		key;
};

#define HT_INITIAL_BUCKETS ((size_t)0x100'000)

struct Hash_table {
	hash_func		hash;
	void			*gen;
	size_t			gen_len;

	struct List_node	**buckets;
	size_t			buckets_cnt;
};

int Hash_table_ctor(struct Hash_table *ht, hash_func hash, void *gen, size_t gen_len);
int Hash_table_dtor(struct Hash_table *ht);

int Hash_table_insert(struct Hash_table *ht, ht_key_t key);
byte_t Hash_table_find(struct Hash_table const *ht, ht_key_t key);

#endif