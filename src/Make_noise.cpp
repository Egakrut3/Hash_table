#include "Common.h"
#include <time.h>

int main(int argc, char *argv[]) {
	assert(argc == 2);

	srand((unsigned)time(0));

	#define MIN_LEN ((size_t)4)
	#define MAX_LEN ((size_t)8)
	static_assert(MIN_LEN <= MAX_LEN);

	size_t count = 0;
	sscanf(argv[1], "%zu", &count);
	size_t	*const	lens	= (TYPEOF(lens))calloc(count, sizeof(REMOVE_POINTER(lens))),
			sum_len	= 0;
	for (size_t i = 0; i < count; i++) {
		lens[i] = MIN_LEN + (size_t)rand() % (MAX_LEN - MIN_LEN + 1);
		sum_len += lens[i] + 1; // One extra character is needed for \n
	}

	char *const buffer = (TYPEOF(buffer))calloc(sum_len, sizeof(REMOVE_POINTER(buffer)));
	size_t cur_beg = 0;
	for (size_t i = 0; i < count; cur_beg += lens[i] + 1, i++) {
		for (size_t j = 0; j < lens[i]; j++) {
			buffer[cur_beg + j] = 'a' + (char)(rand() % ('z' - 'a' + 1));
		}
		buffer[cur_beg + lens[i]] = '\n';
	}

	fwrite(buffer, sizeof(REMOVE_POINTER(buffer)), sum_len, stdout);

	free(buffer);
	free(lens);
	return 0;
}