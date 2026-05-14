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
	ALLOC_ARR(ht->buckets, ht->buckets_cnt);

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		CHECK_PROC(CFFL_ctor, &ht->buckets[i], CFFL_MIN_CAPACITY);
	}

	return 0;
}

int Hash_table_dtor(struct Hash_table *const ht) {
	assert(ht);

	if (ht->gen) { FREE_ELEM(ht->gen); }

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		CHECK_PROC(CFFL_dtor, &ht->buckets[i]);
	}

	FREE_ARR(ht->buckets, ht->buckets_cnt);

	return 0;
}

int Hash_table_insert(struct Hash_table *const ht, HT_arg_key_t const key) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key) % ht->buckets_cnt;
	CHECK_PROC(CFFL_insert_after, &ht->buckets[bucket], 0, key, nullptr);

	return 0;
}

#if HT_OPTIMIZATION > 0

extern "C" int list_find(struct CFFL const *list, HT_arg_key_t key, byte_t *found);

#else

static int list_find(struct CFFL const *const list, HT_arg_key_t const key, byte_t *const found) {
	assert(list); assert(found);

	size_t cur = list->buffer[0].next;
	while (cur != CFFL_UNAVAILABLE_IND) {
		if (HT_KEY_EQUAL(list->buffer[cur].val, key)) {
			*found = 1;

			CLEAR_RESOURCES();
			return 0;
		}

		cur = list->buffer[cur].next;
	}
	*found = 0;

	CLEAR_RESOURCES();
	return 0;
}

#endif

int Hash_table_find(struct Hash_table const *const ht, HT_arg_key_t const key, byte_t *const found) {
	assert(ht);

	size_t bucket = ht->hash(ht->gen, key);
#if HT_OPTIMIZATION > 2
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
	CHECK_PROC(list_find, &ht->buckets[bucket], key, found);

	return 0;
}

#undef FINAL_CODE
