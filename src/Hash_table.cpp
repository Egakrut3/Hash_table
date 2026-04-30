#include "Hash_table.h"

int Hash_table_ctor(struct Hash_table *const ht, hash_func hash, void *const gen, size_t const gen_len, size_t const buckets_cnt) {
	assert(ht);

	ht->hash	= hash;
	if (gen) {
		ht->gen = malloc(gen_len);
		memcpy(ht->gen, gen, gen_len);
	}
	ht->gen_len	= gen_len;

	ht->buckets_cnt	= buckets_cnt;
	ht->buckets	= (decltype(ht->buckets))calloc(ht->buckets_cnt, sizeof(*ht->buckets));

	return 0;
}

static void list_free(List_node **const cur) {
	assert(cur);

	if (!*cur) { return; }

	list_free(&(*cur)->next);
	HT_KEY_FREE((*cur)->key);
	free(*cur);
	*cur = nullptr;
}

int Hash_table_dtor(struct Hash_table *const ht) {
	assert(ht);

	if (ht->gen) { free(ht->gen); }

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		if (ht->buckets[i]) {
			list_free(&ht->buckets[i]);
		}
	}
	free(ht->buckets);

	return 0;
}

static int list_insert(List_node **const cur, ht_arg_key_t const key) {
	assert(cur);

	if (!*cur) {
		*cur = (List_node *)malloc(sizeof(**cur)); // TODO - how to use decltype?
		(*cur)->next	= nullptr;
		HT_KEY_COPY((*cur)->key, key);

		return 0;
	}

	if (HT_KEY_EQUAL((*cur)->key, key)) { return 0; }

	return list_insert(&(*cur)->next, key);
}

int Hash_table_insert(struct Hash_table *const ht, ht_arg_key_t const key) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key) % ht->buckets_cnt;
	return list_insert(&ht->buckets[bucket], key);
}

static byte_t list_find(List_node const *const cur, ht_arg_key_t const key) {
	if (!cur)		{ return 0; }
	if (HT_KEY_EQUAL(cur->key, key)) { return 1; }
	return list_find(cur->next, key);
}

byte_t Hash_table_find(struct Hash_table const *const ht, ht_arg_key_t const key) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key) % ht->buckets_cnt;
	return list_find(ht->buckets[bucket], key);
}