#ifndef COMMON_DECLARATIONS
#define COMMON_DECLARATIONS

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#include <limits.h>
#include <stdint.h>

#include <immintrin.h>

#if defined(NDEBUG)

#define ON_DEBUG(...)

#else

#define ON_DEBUG(...) __VA_ARGS__

#endif

#define CLEAR_RESOURCES()	\
do {				\
	FINAL_CODE		\
} while (false)

#define PRINT_LINE()							\
do {									\
	fprintf(stderr, __FILE__ ":%d: %s\n", __LINE__, __func__);	\
} while (false)

#define CHECK_PROC(proc, ...)							\
do {										\
	int __cur_err_val = proc(__VA_ARGS__);					\
	if (!__cur_err_val) { break; }						\
	ON_DEBUG(								\
		fprintf(stderr, "Error with code %d found\n", __cur_err_val);	\
		PRINT_LINE();							\
		fputs(#proc " failed\n", stderr);				\
	)									\
	CLEAR_RESOURCES();							\
	return __cur_err_val;							\
} while (false)

#define CHECK_PROC_VOID(proc, ...)						\
do {										\
	int __cur_err_val = proc(__VA_ARGS__);					\
	if (!__cur_err_val) { break; }						\
	ON_DEBUG(								\
		fprintf(stderr, "Error with code %d found\n", __cur_err_val);	\
		PRINT_LINE();							\
		fputs(#proc " failed\n", stderr);				\
	)									\
	CLEAR_RESOURCES();							\
	return;									\
} while (false)

#ifdef __STDC_VERSION__

#define TYPEOF(ent_or_expr) typeof(ent_or_expr)

#define REMOVE_POINTER(ent_or_expr) (typeof(*(typeof(ent_or_expr))nullptr))

#else

#define TYPEOF(ent_or_expr) std::remove_cv_t<decltype(ent_or_expr)>

#include <type_traits>
#define REMOVE_POINTER(ent_or_expr) std::remove_pointer_t<decltype(ent_or_expr)>

#endif

typedef char unsigned byte_t;

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#endif