#include <stdio.h>
#include "shared_ref.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

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
	shared_ref_release(&test1);
	return 0;
}
