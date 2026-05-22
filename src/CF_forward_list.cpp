#include "CF_forward_list.hpp"

#define HEAD		((size_t)0)
#define UNAVAILABLE_IND SIZE_MAX



#define FINAL_CODE

int CFFL_ctor(struct CFFL *const list, size_t const start_capacity) {
	assert(list);

	if (start_capacity < CFFL_MIN_CAPACITY) {
		list->capacity = CFFL_MIN_CAPACITY;
	} else {
		list->capacity	= start_capacity;
	}
	ALLOCU_ARR(list->buffer, list->capacity);

	list->buffer[HEAD].next	= UNAVAILABLE_IND;
	for (size_t i = HEAD + 1; i < list->capacity - 1; ++i) {
		list->buffer[i].next = i + 1;
	}
	list->buffer[list->capacity - 1].next = UNAVAILABLE_IND;
	list->first_vacant = HEAD + 1;

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_dtor(struct CFFL *const list) {
	assert(list);

	size_t cur = list->buffer[HEAD].next;
	while (cur != UNAVAILABLE_IND) {
		CFFL_VAL_FREE(list->buffer[cur].val);

		cur = list->buffer[cur].next;
	}

	FREEU_ARR(list->buffer, list->capacity);

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE



size_t CFFL_before_begin(struct CFFL const *const list) {
	assert(list);

	return HEAD;
}

size_t CFFL_begin(struct CFFL const *const list) {
	assert(list);

	return list->buffer[HEAD].next;
}

size_t CFFL_end(struct CFFL const *const list) {
	assert(list);

	return UNAVAILABLE_IND;
}



#define FINAL_CODE

static int CFFL_reallocate_up(struct CFFL *const list, size_t const new_capacity) {
	assert(list);
	assert(new_capacity > list->capacity);

	REALLOC_ARR(list->buffer, new_capacity);
	for (size_t i = list->capacity; i < new_capacity - 1; i++) {
		list->buffer[i].next = i + 1;
	}
	list->buffer[new_capacity - 1].next	= list->first_vacant;
	list->first_vacant			= list->capacity;
	list->capacity				= new_capacity;

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_insert_after(struct CFFL *const list, size_t *const result, size_t const pos, CFFL_arg_t const val) {
	assert(list);
	assert(pos < list->capacity);

	if (list->first_vacant == UNAVAILABLE_IND) { CHECK_PROC(CFFL_reallocate_up, list, list->capacity * CFFL_EXPANSION); }

	size_t const new_pos = list->first_vacant;
	list->first_vacant		= list->buffer[list->first_vacant].next;
	list->buffer[new_pos].next	= list->buffer[pos].next;
	list->buffer[pos].next	= new_pos;

	CFFL_VAL_COPY(list->buffer[new_pos].val, val);

	if (result) { *result = new_pos; }

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_push_front(struct CFFL *list, CFFL_arg_t val) {
	CHECK_PROC(CFFL_insert_after, list, nullptr, CFFL_before_begin(list), val);

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_erase_after(struct CFFL *const list, size_t const pos) {
	assert(list);
	assert(pos < list->capacity);
	assert(list->buffer[pos].next != UNAVAILABLE_IND);

	size_t const cur_pos = list->buffer[pos].next;
	list->buffer[pos].next		= list->buffer[cur_pos].next;
	list->buffer[cur_pos].next	= list->first_vacant;
	list->first_vacant		= cur_pos;

	CFFL_VAL_FREE(list->buffer[cur_pos].val);

	CLEAR_RESOURCES();
	return 0;
}

int CFFL_pop_front(struct CFFL *list) {
	CHECK_PROC(CFFL_erase_after, list, CFFL_before_begin(list));

	CLEAR_RESOURCES();
	return 0;
}

#undef FINAL_CODE
