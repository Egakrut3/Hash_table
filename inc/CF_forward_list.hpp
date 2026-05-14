#ifndef CF_FORWARD_LIST
#define CF_FORWARD_LIST

#include "Common.hpp"
// TODO - Linearize and shrink

typedef char const	*CFFL_arg_t;
typedef char		*CFFL_mem_t;

#define CFFL_VAL_COPY(dest, src)	\
do {					\
	dest = strdup(src);		\
} while (false);
#define CFFL_VAL_FREE(val)	\
do {				\
	free(val);		\
} while (false);

struct CFFL_node {
	size_t		next;

	CFFL_mem_t	val;
};

#define CFFL_EXPANSION		((size_t)2)
#define CFFL_MIN_CAPACITY	((size_t)2)
static_assert(CFFL_EXPANSION > 1);
static_assert(CFFL_MIN_CAPACITY > 1);

#define CFFL_UNAVAILABLE_IND (~(size_t)0)

struct CFFL {
	size_t			capacity,
				size;
	struct CFFL_node	*buffer;
	size_t			first_empty;
};

int CFFL_ctor(struct CFFL *list, size_t start_capacity);
int CFFL_dtor(struct CFFL *list);

size_t CFFL_before_begin(struct CFFL const *list);
size_t CFFL_begin(struct CFFL const *list);
size_t CFFL_end(struct CFFL const *list);

int CFFL_insert_after(struct CFFL *list, size_t after_what, CFFL_arg_t val, size_t *result);
int CFFL_erase_after(struct CFFL *list, size_t after_what);

#endif
