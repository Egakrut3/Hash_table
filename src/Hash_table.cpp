#include "Hash_table.hpp"

#define FINAL_CODE

int Hash_table_ctor(struct Hash_table *const ht, HT_hash_func_t hash, struct Hash_gen const *const gen, size_t const buckets_cnt) {
	assert(ht);

	ht->hash	= hash;
	if (!gen) { ht->gen = nullptr; }
	else {
		ht->gen		= (TYPEOF_UNQUAL(ht->gen))malloc(sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(ht->gen))));

		ht->gen->size	= gen->size;
		ht->gen->data	= malloc(ht->gen->size);
		memcpy(ht->gen->data, gen->data, gen->size);
	}

	ht->buckets_cnt	= buckets_cnt;
	ht->buckets	= (TYPEOF_UNQUAL(ht->buckets))calloc(ht->buckets_cnt, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(ht->buckets))));

	return 0;
}

static int list_free(List_node **const cur) {
	assert(cur);

	if (!*cur) { return 0; }

	CHECK_PROC(list_free, &(*cur)->next);

	HT_KEY_FREE((*cur)->key);
	free_sized(*cur, sizeof(REMOVE_POINTER(REMOVE_POINTER(TYPEOF_UNQUAL(cur)))));
	*cur = nullptr;

	return 0;
}

int Hash_table_dtor(struct Hash_table *const ht) {
	assert(ht);

	if (ht->gen) { free_sized(ht->gen->data, ht->gen->size); }

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		if (ht->buckets[i]) { CHECK_PROC(list_free, &ht->buckets[i]); }
	}
	free_sized(ht->buckets, ht->buckets_cnt * sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(ht->buckets))));

	return 0;
}

static int list_insert(List_node **const cur, HT_arg_key_t const key) {
	assert(cur);

	if (!*cur) {
		*cur = (REMOVE_POINTER(TYPEOF_UNQUAL(cur)))malloc(sizeof(REMOVE_POINTER(REMOVE_POINTER(TYPEOF_UNQUAL(cur)))));

		(*cur)->next = nullptr;

		HT_KEY_COPY((*cur)->key, key);

		return 0;
	}

	if (HT_KEY_EQUAL((*cur)->key, key)) { fprintf(stderr, "Key %s already exists\n", key); return 0; }

	CHECK_PROC(list_insert, &(*cur)->next, key);

	return 0;
}

int Hash_table_insert(struct Hash_table *const ht, HT_arg_key_t const key) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key);
#if HT_OPTIMIZATION > 1
	__asm__ (	"dec %1\n"
			"and %1, %0"
		:	"+r" (bucket)
		:	"r" (ht->buckets_cnt)
		:	"cc");
#else
	bucket %= ht->buckets_cnt;
#endif
	CHECK_PROC(list_insert, &ht->buckets[bucket], key);

	return 0;
}

#undef FINAL_CODE

static byte_t list_find(List_node const *const cur, HT_arg_key_t const key) {
	if (!cur)				{ return 0; }
	if (HT_KEY_EQUAL(cur->key, key))	{ return 1; }

	return list_find(cur->next, key);
}

byte_t Hash_table_find(struct Hash_table const *const ht, HT_arg_key_t const key) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key) % ht->buckets_cnt;
	return list_find(ht->buckets[bucket], key);
}
