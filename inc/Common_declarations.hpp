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



#if defined(__STDC_VERSION__)

#define TYPEOF_UNQUAL(expr)	typeof(expr)
#define LVAL_TYPEOF(expr)	TYPEOF_UNQUAL(expr)
#define REMOVE_POINTER(type)	TYPEOF_UNQUAL(*(type)nullptr)

#else

#include <type_traits>

#define TYPEOF_UNQUAL(expr)	std::remove_cv_t<decltype(expr)>
#define LVAL_TYPEOF(expr)	std::remove_reference_t<TYPEOF_UNQUAL(expr)>
#define REMOVE_POINTER(type)	std::remove_pointer_t<type>

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
	int _cur_err_val = proc(__VA_ARGS__);					\
	if (!_cur_err_val) { break; }						\
	ON_DEBUG(								\
		fprintf(stderr, "Error with code %d found\n", _cur_err_val);	\
		PRINT_LINE();							\
		fputs(#proc " failed\n", stderr);				\
	)									\
	CLEAR_RESOURCES();							\
	return _cur_err_val;							\
} while (false)



#define ALLOC_ELEM(dest)								\
do {											\
	(dest) = (LVAL_TYPEOF(dest))malloc(sizeof(REMOVE_POINTER(LVAL_TYPEOF(dest))));	\
} while (false)
#define FREE_ELEM(elem)							\
do {									\
	free_sized((elem), sizeof(REMOVE_POINTER(LVAL_TYPEOF(elem))));	\
	(elem) = nullptr;						\
} while (false)

#define ALLOC_ARR(dest, size)									\
do {												\
	(dest) = (LVAL_TYPEOF(dest))malloc((size) * sizeof(REMOVE_POINTER(LVAL_TYPEOF(dest))));	\
} while (false)
#define FREE_ARR(arr, size)							\
do {										\
	free_sized((arr), (size) * sizeof(REMOVE_POINTER(LVAL_TYPEOF(arr))));	\
	(arr) = nullptr;							\
} while (false)



typedef char unsigned byte_t;

#endif
