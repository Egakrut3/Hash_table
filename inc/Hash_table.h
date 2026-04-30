#ifndef HASH_TABLE
#define HASH_TABLE

#include "Common.h"

typedef char *ht_key_t;

typedef size_t (*hash_func)(void *gen, ht_key_t key);

struct List_node {
	struct List_node	*next;
	ht_key_t		key;
};

struct Hash_table {
	hash_func	hash;
	void		*gen;
	size_t		gen_len;

	struct List_node *buckets;
};

int Hash_table_ctor(struct Hash_table *ht, hash_func hash, void *gen, size_t gen_len);
int Hash_table_dtor(struct Hash_table *ht);

#endif