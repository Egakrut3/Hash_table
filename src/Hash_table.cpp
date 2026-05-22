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

	struct CFFL_node const *const	list_buffer	= list->buffer;
	size_t				cur		= CFFL_begin(list),
					list_end	= CFFL_end(list);
	int				result		= 0;

#if HT_OPTIMIZATION > 2
Loop:	__asm__ goto (
		"\t\tcmp\t%0, %1\n"
		"\t\tje\t%l[No]\n":
		:
		"r" (cur),
		"g" (list_end):
		"cc":
		No
	);
#else
	while (cur != list_end) {
#endif
		result = HT_KEY_NEQUAL(list_buffer[cur].val, key);
		cur = list_buffer[cur].next;
	#if HT_OPTIMIZATION > 2
		__asm__ goto (
			"\t\ttest\t%0, %0\n"
			"\t\tjnz\t%l[Loop]\n":
			:
			"r" (result):
			"cc":
			Loop
		);
	#else
		if (result) { continue; }
	#endif
		*found = true;

		CLEAR_RESOURCES();
		return 0;
#if HT_OPTIMIZATION > 2
No:
#else
	}
#endif
	*found = false;

	CLEAR_RESOURCES();
	return 0;
}

int Hash_table_find(struct Hash_table const *const ht, bool *const found, HT_arg_key_t const key) {
	assert(ht);

	size_t const bucket = ht->hash(key) % ht->buckets_cnt;
	CHECK_PROC(list_find, &ht->buckets[bucket], found, key);

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
