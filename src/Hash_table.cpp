#include "Hash_table.hpp"

#define FINAL_CODE

int Hash_table_ctor(struct Hash_table *const ht, HT_hash_func_t hash, size_t const buckets_cnt) {
	assert(ht);

	ht->hash = hash;

	ht->buckets_cnt	= buckets_cnt;
	ALLOCU_ARR(ht->buckets, ht->buckets_cnt);

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		CHECK_PROC(CFFL_ctor, &ht->buckets[i], CFFL_MIN_CAPACITY);
	}

	CLEAR_RESOURCES();
	return 0;
}

int Hash_table_dtor(struct Hash_table *const ht) {
	assert(ht);

	for (size_t i = 0; i < ht->buckets_cnt; i++) {
		CHECK_PROC(CFFL_dtor, &ht->buckets[i]);
	}

	FREEU_ARR(ht->buckets, ht->buckets_cnt);

	CLEAR_RESOURCES();
	return 0;
}

int Hash_table_insert(struct Hash_table *const ht, HT_arg_key_t const key) {
	assert(ht);

	size_t const bucket = ht->hash(key) % ht->buckets_cnt;
	CHECK_PROC(CFFL_push_front, &ht->buckets[bucket], key);

	CLEAR_RESOURCES();
	return 0;
}

static int list_find(struct CFFL const *const list, bool *const found, HT_arg_key_t const key) {
	assert(list); assert(found);

	size_t cur = CFFL_begin(list);
	while (cur != CFFL_end(list)) {
		if (HT_KEY_EQUAL(list->buffer[cur].val, key)) {
			*found = true;

			CLEAR_RESOURCES();
			return 0;
		}

		cur = list->buffer[cur].next;
	}
	*found = false;

	CLEAR_RESOURCES();
	return 0;
}

int Hash_table_find(struct Hash_table const *const ht, bool *const found, HT_arg_key_t const key) {
	assert(ht);

#if HT_OPTIMIZATION > 2
	size_t	bucket	= ht->hash(key),
		_tmp	= 0;
	__asm__ (	"mov\t%2, %1\n\t"
			"dec\t%1\n\t"
			"and\t%1, %0":
			"+g" (bucket),
			"=r" (_tmp):
			"g" (ht->buckets_cnt):
			"cc");
#else
	size_t const bucket = ht->hash(key) % ht->buckets_cnt;
#endif
	CHECK_PROC(list_find, &ht->buckets[bucket], found, key);

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
