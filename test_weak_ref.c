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

void test2()
{
	printf("======test2 start=======\n");
	char *data = (char *)calloc(1024, 1);
	SHARED_REF_DEF(test1);
	shared_ref_init(&test1, data);
	WEAK_REF_DEF(test2);
	weak_ref_init(&test2, &test1);

	SHARED_REF_DEF(tmp);
	WEAK_REF_DEF(weak_a);
	weak_ref_copy_init(&weak_a, &test2);
	char *d = (char *)shared_ref_get(weak_ref_lock(&tmp, &weak_a));
	if (d == NULL) {
		printf("get data error, get no data here, Failed\n");
	} else {
		printf("get data OK\n");
	}

	shared_ref_release(&test1);
	shared_ref_reset(&tmp, NULL);
	WEAK_REF_DEF(weak_c);
	weak_ref_copy_init(&weak_c, &test2);
	d = (char *)shared_ref_get(weak_ref_lock(&tmp, &weak_c));
	if (d == NULL) {
		printf("get no data here, OK\n");
	} else {
		printf("get data here, ref must be zero, ERROR\n");
	}
	weak_ref_release(&test2);
	weak_ref_release(&weak_c);
	weak_ref_release(&weak_a);
	printf("=====test2 end=======\n");
}

void test3()
{
	printf("======test3 start=======\n");
	char *data = (char *)calloc(1024, 1);
	char *data2 = (char *)calloc(2048, 1);
	SHARED_REF_DEF(test1);
	shared_ref_init(&test1, data);
	WEAK_REF_DEF(weak_a);
	weak_ref_init(&weak_a, &test1);

	SHARED_REF_DEF(test2);
	shared_ref_init(&test2, data2);
	WEAK_REF_DEF(weak_b);
	weak_ref_init(&weak_b, &test2);

	WEAK_REF_DEF(weak_c);
	weak_ref_init(&weak_c, &test2);

	weak_ref_copy(&weak_a, &weak_b);
	SHARED_REF_DEF(tmp);
	char *d = (char *)shared_ref_get(weak_ref_lock(&tmp, &weak_a));
	if (d == NULL) {
		printf("get data error, get no data here, Failed\n");
	} else {
		printf("get data OK\n");
		if (d != data2) {
			printf("weak_ref copy result error, d must be equal to data2\n");
		} else {
			printf("weak_ref copy OK\n");
		}
	}
	shared_ref_reset(&tmp, NULL);

	d = (char *)shared_ref_get(weak_ref_lock(&tmp, &weak_b));
	if (d == NULL) {
		printf("get data error, get no data here, Failed\n");
	} else {
		printf("get data OK\n");
		if (d != data2) {
			printf("weak_ref copy result error, d must be equal to data2\n");
		} else {
			printf("weak_ref copy OK\n");
		}
	}
	shared_ref_reset(&tmp, NULL);

	shared_ref_release(&test1);
	shared_ref_release(&test2);
	weak_ref_release(&weak_a);
	weak_ref_release(&weak_b);
	weak_ref_release(&weak_c);
	printf("=====test3 end=======\n");
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	test1();
	test2();
	test3();
	return 0;
}
