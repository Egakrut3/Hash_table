#include "Common.h"
#include <time.h>

int main() {
	srand((unsigned)time(0));
	#define NOISE_COUNT 0x80'000
	#define MIN_LEN		((size_t)4)
	#define MAX_LEN		((size_t)8)
	static_assert(MIN_LEN <= MAX_LEN);
	size_t	*const	lens	= (size_t *)calloc(NOISE_COUNT, sizeof(*lens)),
			sum	= 0;
	for (size_t i = 0; i < NOISE_COUNT; i++) {
		lens[i] = MIN_LEN + (size_t)rand() % (MAX_LEN - MIN_LEN + 1); // One extra character is needed for \n
		sum += lens[i] + 1;
	}

	char *const buffer = (char *)calloc(sum, sizeof(*buffer));
	size_t cur_beg = 0;
	for (size_t i = 0; i < NOISE_COUNT; cur_beg += lens[i] + 1, i++) {
		for (size_t j = 0; j < lens[i]; j++) {
			buffer[cur_beg + j] = 'a' + (char)(rand() % ('z' - 'a' + 1));
		}
		buffer[cur_beg + lens[i]] = '\n';
	}

	FILE *output = fopen("data/Noise_test.txt", "w");
	fwrite(buffer, sizeof(*buffer), sum, output);

	fclose(output);
	free(buffer);
	free(lens);
	return 0;
}