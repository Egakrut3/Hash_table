#ifndef HASH_TABLE
#define HASH_TABLE

#include "Common.h"

typedef char const	*HT_arg_key_t;
typedef char		*HT_mem_key_t;

#define HT_KEY_COPY(dest, src)	\
do {				\
	dest = strdup(src);	\
} while (false)
#define HT_KEY_FREE(key)	\
do {				\
	free(key);		\
} while (false);
#define HT_KEY_EQUAL(a, b) (!strcmp(a, b))

typedef size_t (*HT_hash_func_t)(struct Hash_gen *gen, HT_arg_key_t key);

struct Hash_gen {
	size_t	size;
	void	*data;
};

struct List_node {
	struct List_node	*next;

	HT_mem_key_t		key;
};

struct Hash_table {
	HT_hash_func_t	hash;
	struct Hash_gen	*gen;

	size_t			buckets_cnt;
	struct List_node	**buckets;
};

int Hash_table_ctor(struct Hash_table *ht, HT_hash_func_t hash, struct Hash_gen const *gen, size_t buckets_cnt);
int Hash_table_dtor(struct Hash_table *ht);

int Hash_table_insert(struct Hash_table *ht, HT_arg_key_t key);
byte_t Hash_table_find(struct Hash_table const *ht, HT_arg_key_t key);

#endif
