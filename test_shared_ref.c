#include <stdio.h>
#include "shared_ref.h"

void test1()
{
	printf("======test1 start=======\n");
	char *data = (char *)calloc(1024, 1);
	SHARED_REF_DEF(test1);
	shared_ref_init(&test1, data);
	char *d = (char *)shared_ref_get(&test1);
	if (d != NULL) {
		snprintf(d, 1024, "%s", "this is a test");
	}
	printf("%s\n", d);
	shared_ref_reset(&test1, NULL);
	d = (char *)shared_ref_get(&test1);
	if (d != NULL) {
		printf("has been reset to NULL, must be NULL here\n");
	}
	//shared_ref_release(&test1);

	printf("=====test1 end=======\n");
}

void test2()
{
	printf("======test2 start=======\n");
	char *data = (char *)calloc(1024, 1);
	SHARED_REF_DEF(test1);
	shared_ref_init(&test1, data);
	SHARED_REF_DEF(test2);
	shared_ref_copy_init(&test2, &test1);
	shared_ref_release(&test2);
	shared_ref_release(&test1);
	printf("=====test2 end=======\n");
}

void test3()
{
	printf("======test3 start=======\n");
	char *data = (char *)calloc(1024, 1);
	char *data2 = (char *)calloc(2048, 1);
	SHARED_REF_DEF(test1);
	shared_ref_init(&test1, data);
	SHARED_REF_DEF(test2);
	shared_ref_init(&test2, data2);
	shared_ref_copy(&test2, &test1);
	SHARED_REF_DEF(test3);
	shared_ref_copy(&test3, &test1);
	shared_ref_release(&test2);
	shared_ref_release(&test1);

	char *d = (char *)shared_ref_get(&test3);
	if (d == data) {
		printf("get data ok?\n");
	} else {
		printf("get null, should be error here\n");
	}
	long use_count = shared_ref_use_count(&test3);
	printf("use count = %ld (count = 1 is OK, others is error)\n", use_count);
	shared_ref_release(&test3);
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
