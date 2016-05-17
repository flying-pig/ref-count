#include "sp_counted_base_gcc_x86.h"

void defult_free()
{
}

sp_counted_base *sp_counted_new(SP_COUNT_TYPE type)
{
	(void) type;
	sp_counted_base *base = (sp_counted_base *)malloc(sizeof(sp_counted_base));
	base->use_count_ = 1;
   	base->weak_count_ = 1;

	base->free_ = defult_free;
	base->destroy = destroy;
	return base;
}

void destroy(sp_counted_base *base)
{
	base->free_();
	free(base);
}

void add_ref_copy(sp_counted_base *base)
{
	atomic_increment( &base->use_count_ );
}

int add_ref_lock(sp_counted_base *base)
{
	return atomic_conditional_increment( &base->use_count_ ) != 0;
}

void release(sp_counted_base *base)
{
	if( atomic_exchange_and_add( &base->use_count_, -1 ) == 1 )
	{
		base->dispose(base);
		base->weak_release(base);
	}
}

void weak_add_ref(sp_counted_base *base)
{
	atomic_increment( &base->weak_count_ );
}

void weak_release(sp_counted_base *base)
{
	if( atomic_exchange_and_add( &base->weak_count_, -1 ) == 1 )
	{
		base->destroy(base);
	}
}

long use_count(const sp_counted_base *base)
{
	return (int const volatile)(base->use_count_ );
}
