#ifndef COMMON_DECLARATIONS
#define COMMON_DECLARATIONS

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

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

#define TYPEOF_UNQUAL(expr)	typeof(expr)
#define REMOVE_POINTER(type)	TYPEOF_UNQUAL(*(type)nullptr)

#else

#include <type_traits>

#define TYPEOF_UNQUAL(expr)	std::remove_cv_t<decltype(expr)>
#define REMOVE_POINTER(type)	std::remove_pointer_t<type>

#endif

typedef char unsigned byte_t;

#endif
