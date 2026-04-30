#include "Hash_table.h"

int Hash_table_ctor(struct Hash_table *ht, hash_func hash, void *gen, size_t gen_len) {
	
}

static void list_insert(List_node **cur, my_int val) {
	if (!*cur) {
		*cur = malloc(sizeof(**cur));
		(*cur)->next	= nullptr;
		(*cur)->val	= val;
		return;
	}

	if ((*cur)->val == val) { return; }

	list_insert(&(*cur)->next, val);
}

void list_erase(List_node **cur, my_int val) {
	if (!*cur) { return; }

	if ((*cur)->val == val) {
		List_node *next_node = (*cur)->next;
		free(*cur);
		*cur = next_node;
		return;
	}

	list_erase(&(*cur)->next, val);
}

byte_t list_find(List_node const *cur, my_int val) {
	if (!cur)		{ return 0; }
	if (cur->val == val)	{ return 1; }
	return list_find(cur->next, val);
}

void list_free(List_node **cur) {
	if (!*cur) { return; }

	list_free(&(*cur)->next);
	free(*cur);
	*cur = nullptr;
}