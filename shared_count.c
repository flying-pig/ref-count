#include <stdlib.h>
#include <shared_count.h>

shared_count *shared_count_new()
{
	return (shared_count *)calloc(1, sizeof(shared_count));
}

shared_count *shared_count_init(void *p)
{
	shared_count *count = shared_count_new();
	count->pi_ = sp_counted_impl_p_new(p);

	return count;
}
