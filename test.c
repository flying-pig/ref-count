#include <stdio.h>
#include <pthread.h>
#include "ref-count/sp_counted_base.h"

#define NUM_THREADS 10

struct thread_info {    /* Used as argument to thread_start() */
	pthread_t thread_id;        /* ID returned by pthread_create() */
	int       thread_num;       /* Application-defined thread # */
	char     *argv_string;      /* From command-line argument */
};

void *test_process(void *arg)
{
	int i;
	sp_counted_base *b = (sp_counted_base *)arg;
	for (i = 0; i < 1000; i++) {
		b->add_ref_copy(b);
	}
}

int main(int argc, char *argv[])
{
	int num_threads;
	int tnum;

	sp_counted_base *b = sp_counted_new(0);
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	num_threads = NUM_THREADS;
	struct thread_info tinfo[NUM_THREADS] = {0};
	for (tnum = 0; tnum < num_threads; tnum++) {
		pthread_create(&tinfo[tnum].thread_id, &attr, test_process, b);
	}

	int s;
	for (tnum = 0; tnum < num_threads; tnum++) {
		s = pthread_join(tinfo[tnum].thread_id, NULL);
		if (s != 0) {
			printf("pthread_jon: %d\n", s);
		}

		printf("Joined with thread %d; returned value was -",
				tinfo[tnum].thread_id);
	}

	printf("use count: %ld\n", b->use_count(b));

	return 0;
}
