#include "Common.hpp"
#include <time.h>

int main(int const argc, char *const argv[]) {
	#define FINAL_CODE

	assert(argc == 2);

	srand((unsigned)time(nullptr));

	#define MIN_LEN	((size_t)4)
	#define MAX_LEN	((size_t)8)
	static_assert(MIN_LEN <= MAX_LEN);

	size_t count = 0;
	sscanf(argv[1], "%zu", &count);
	size_t *lens = nullptr;
	ALLOC_ARR(lens, count);
	#undef FINAL_CODE
	#define FINAL_CODE	\
	FREE_ARR(lens, count);

	size_t sum_len = 0;
	for (size_t i = 0; i < count; i++) {
		lens[i] = MIN_LEN + (size_t)rand() % (MAX_LEN - MIN_LEN);
		sum_len += lens[i] + 1; // One extra character is needed for \n
	}

	char *buffer = nullptr;
	ALLOC_ARR(buffer, sum_len);
	#undef FINAL_CODE
	#define FINAL_CODE		\
	FREE_ARR(buffer, sum_len);	\
	FREE_ARR(lens, count);

	size_t cur_beg = 0;
	for (size_t i = 0; i < count; cur_beg += lens[i] + 1, i++) {
		for (size_t j = 0; j < lens[i]; j++) {
			buffer[cur_beg + j] = 'a' + (REMOVE_POINTER(TYPEOF_UNQUAL(buffer)))(rand() % ('z' - 'a' + 1));
		}
		buffer[cur_beg + lens[i]] = '\n';
	}
	fwrite(buffer, sizeof(REMOVE_POINTER(TYPEOF_UNQUAL(buffer))), sum_len, stdout);

	CLEAR_RESOURCES();
	return 0;
}
