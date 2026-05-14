#include "CF_forward_list.hpp"

#define FINAL_CODE

int CFFL_ctor(struct CFFL *list, size_t start_capacity) {
	assert(list);

	if (start_capacity < CFFL_MIN_CAPACITY) {
		start_capacity = CFFL_MIN_CAPACITY;
	}
	list->capacity	= start_capacity;
	list->size	= 0;
	ALLOC_ARR(list->buffer, list->capacity);

	list->buffer[0].next	= CFFL_UNAVAILABLE_IND;
	for (size_t i = 1; i < list->capacity - 1; ++i) {
		list->buffer[i].next = i + 1;
	}
	list->buffer[list->capacity - 1].next = CFFL_UNAVAILABLE_IND;
	list->first_empty = 1;

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_dtor(struct CFFL *list) {
	assert(list);

	size_t cur = list->buffer[0].next;
	while (cur != CFFL_UNAVAILABLE_IND) {
		CFFL_VAL_FREE(list->buffer[cur].val);

		cur = list->buffer[cur].next;
	}

	FREE_ARR(list->buffer, list->capacity);

	CLEAR_RESOURCES();
	return 0;
}



size_t CFFL_before_begin(struct CFFL const *list) {
	assert(list);

	return 0;
}

size_t CFFL_begin(struct CFFL const *list) {
	assert(list);

	return list->buffer[0].next;
}

size_t CFFL_end(struct CFFL const *list) {
	assert(list);

	return CFFL_UNAVAILABLE_IND;
}



static int CFFL_reallocate_up(struct CFFL *list, size_t new_capacity) {
	assert(list);
	assert(new_capacity > list->capacity);

	REALLOC_ARR(list->buffer, new_capacity);
	for (size_t i = list->capacity; i < new_capacity - 1; i++) {
		list->buffer[i].next = i + 1;
	}
	list->buffer[new_capacity - 1].next	= list->first_empty;
	list->first_empty			= list->capacity;
	list->capacity				= new_capacity;

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_insert_after(struct CFFL *list, size_t after_what, CFFL_arg_t val, size_t *result) {
	assert(list);

	if (list->first_empty == CFFL_UNAVAILABLE_IND) { CHECK_PROC(CFFL_reallocate_up, list, list->capacity * CFFL_EXPANSION); }

	size_t new_pos = list->first_empty;
	list->first_empty		= list->buffer[list->first_empty].next;
	list->buffer[new_pos].next	= list->buffer[after_what].next;
	list->buffer[after_what].next	= new_pos;

	CFFL_VAL_COPY(list->buffer[new_pos].val, val);
	list->size++;

	if (result) { *result = new_pos; }

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_erase_after(struct CFFL *list, size_t after_what) {
	assert(list);
	assert(after_what < list->capacity); assert(list->buffer[after_what].next != CFFL_UNAVAILABLE_IND);

	size_t cur_pos = list->buffer[after_what].next;
	list->buffer[after_what].next	= list->buffer[cur_pos].next;
	list->buffer[cur_pos].next	= list->first_empty;
	list->first_empty		= cur_pos;

	CFFL_VAL_FREE(list->buffer[cur_pos].val);
	list->size--;

	CLEAR_RESOURCES();
	return 0;
}
