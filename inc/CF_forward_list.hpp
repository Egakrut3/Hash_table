#ifndef CF_FORWARD_LIST
#define CF_FORWARD_LIST

#include "Common.hpp"
// TODO - Add linearize and shrink



typedef char const	*CFFL_arg_t;
typedef char		*CFFL_mem_t;

#if HT_OPTIMIZATION > 0

#define CFFL_ALIGNEMENT		((size_t)64)
#define CFFL_MAX_STR_LEN	((size_t)64)
#define CFFL_VAL_COPY(dest, src)					\
do {									\
	CALLOCA_ARR(dest, CFFL_ALIGNEMENT, CFFL_MAX_STR_LEN);		\
	strcpy(dest, src);						\
} while (false)

#define CFFL_VAL_FREE(val) FREEA_ARR(val, CFFL_ALIGNEMENT, CFFL_MAX_STR_LEN)

#else

#define CFFL_VAL_COPY(dest, src)	\
do {					\
	dest = strdup(src);		\
} while (false)

#define CFFL_VAL_FREE(val) free(val)

#endif

#define CFFL_EXPANSION		((size_t)2)
#define CFFL_MIN_CAPACITY	((size_t)2)
static_assert(CFFL_EXPANSION > 1);
static_assert(CFFL_MIN_CAPACITY > 1);



struct CFFL_node {
	size_t		next;

	CFFL_mem_t	val;
};

struct CFFL {
	size_t			capacity;
	struct CFFL_node	*buffer;

	size_t			first_vacant;
};

int CFFL_ctor(struct CFFL *list, size_t start_capacity);
int CFFL_dtor(struct CFFL *list);

size_t CFFL_before_begin(struct CFFL const *list);
size_t CFFL_begin(struct CFFL const *list);
size_t CFFL_end(struct CFFL const *list);

int CFFL_insert_after(struct CFFL *list, size_t *result, size_t pos, CFFL_arg_t val);
int CFFL_push_front(struct CFFL *list, CFFL_arg_t val);
int CFFL_erase_after(struct CFFL *list, size_t pos);
int CFFL_pop_front(struct CFFL *list);

#endif
