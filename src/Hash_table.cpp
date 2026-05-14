#include "Hash_table.hpp"

#define FINAL_CODE

int Hash_table_ctor(struct Hash_table *const ht, HT_hash_func_t hash, struct Hash_gen const *const gen, size_t const buckets_cnt) {
	assert(ht);

	ht->hash = hash;
	if (!gen) { ht->gen = nullptr; }
	else {
		ALLOC_ELEM(ht->gen);

		ht->gen->size	= gen->size;
		ht->gen->data	= malloc(ht->gen->size);
		memcpy(ht->gen->data, gen->data, gen->size);
	}

	ht->buckets_cnt	= buckets_cnt;
	CALLOC_ARR(ht->buckets, ht->buckets_cnt);

	return 0;
}

static int list_free(struct List_node **const cur) {
	assert(cur);

	if (!*cur) { return 0; }

	CHECK_PROC(list_free, &(*cur)->next);

	HT_KEY_FREE((*cur)->key);

	FREE_ELEM(*cur);

	return 0;
}

int Hash_table_dtor(struct Hash_table *const ht) {
	assert(ht);

	if (ht->gen) { FREE_ELEM(ht->gen); }

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		CHECK_PROC(list_free, &ht->buckets[i]);
	}

	FREE_ARR(ht->buckets, ht->buckets_cnt);

	return 0;
}

static int list_insert(struct List_node **const cur, HT_arg_key_t const key) {
	assert(cur);

	if (!*cur) {
		ALLOC_ELEM(*cur);

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

	size_t bucket = ht->hash(ht->gen, key) % ht->buckets_cnt;
	CHECK_PROC(list_insert, &ht->buckets[bucket], key);

	return 0;
}

#if HT_OPTIMIZATION > 2

extern "C" int list_find(struct List_node const *cur, HT_arg_key_t key, byte_t *found);

#else

static int list_find(struct List_node const *const cur, HT_arg_key_t const key, byte_t *const found) {
	assert(found);

	if (!cur)				{ *found = 0; return 0; }
	if (HT_KEY_EQUAL(cur->key, key))	{ *found = 1; return 0; }

	CHECK_PROC(list_find, cur->next, key, found);

	return 0;
}

#endif

int Hash_table_find(struct Hash_table const *const ht, HT_arg_key_t const key, byte_t *const found) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key);
#if HT_OPTIMIZATION > 1
	size_t _tmp = 0;
	__asm__ (	"mov\t%2, %1\n\t"
			"dec\t%1\n\t"
			"and\t%1, %0"
		:	"+g" (bucket),
			"=r" (_tmp)
		:	"g" (ht->buckets_cnt)
		:	"cc");
#else
	bucket %= ht->buckets_cnt;
#endif
	CHECK_PROC(list_find, ht->buckets[bucket], key, found);

	return 0;
}

#undef FINAL_CODE
