#include <stdio.h>
#include "weak_ref.h"

void test1()
{
	printf("======test1 start=======\n");
	char *data = (char *)calloc(1024, 1);
	SHARED_REF_DEF(test1);
	shared_ref_init(&test1, data);
	WEAK_REF_DEF(test2);
	weak_ref_init(&test2, &test1);
	SHARED_REF_DEF(tmp);
	char *d = (char *)shared_ref_get(weak_ref_lock(&tmp, &test2));
	if (d == NULL) {
		printf("get data error, get no data here, Failed\n");
	} else {
		printf("get data OK\n");
	}
	shared_ref_release(&test1);
	shared_ref_reset(&tmp, NULL);
	d = (char *)shared_ref_get(weak_ref_lock(&tmp, &test2));
	if (d == NULL) {
		printf("get no data here, OK\n");
	} else {
		printf("get data here, ref must be zero, ERROR\n");
	}
	weak_ref_release(&test2);
	printf("=====test1 end=======\n");
}
int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	test1();
	return 0;
}
