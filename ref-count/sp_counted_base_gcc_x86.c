#include "sp_counted_base_gcc_x86.h"

inline int atomic_exchange_and_add( int * pw, int dv )
{
    // int r = *pw;
    // *pw += dv;
    // return r;

    int r;

    __asm__ __volatile__
    (
        "lock\n\t"
        "xadd %1, %0":
        "=m"( *pw ), "=r"( r ): // outputs (%0, %1)
        "m"( *pw ), "1"( dv ): // inputs (%2, %3 == %1)
        "memory", "cc" // clobbers
    );

    return r;
}

inline void atomic_increment( int * pw )
{
    //atomic_exchange_and_add( pw, 1 );

    __asm__
    (
        "lock\n\t"
        "incl %0":
        "=m"( *pw ): // output (%0)
        "m"( *pw ): // input (%1)
        "cc" // clobbers
    );
}

inline int atomic_conditional_increment( int * pw )
{
    // int rv = *pw;
    // if( rv != 0 ) ++*pw;
    // return rv;

    int rv, tmp;

    __asm__
    (
        "movl %0, %%eax\n\t"
        "0:\n\t"
        "test %%eax, %%eax\n\t"
        "je 1f\n\t"
        "movl %%eax, %2\n\t"
        "incl %2\n\t"
        "lock\n\t"
        "cmpxchgl %2, %0\n\t"
        "jne 0b\n\t"
        "1:":
        "=m"( *pw ), "=&a"( rv ), "=&r"( tmp ): // outputs (%0, %1, %2)
        "m"( *pw ): // input (%3)
        "cc" // clobbers
    );

    return rv;
}


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
	base->add_ref_copy = add_ref_copy;
	base->add_ref_lock = add_ref_lock;
	base->release = release;
	base->weak_add_ref = weak_add_ref;
	base->weak_release = weak_release;
	base->use_count = use_count;
	return base;
}

void sp_counted_init(sp_counted_base *base)
{
	base->use_count_ = 1;
	base->weak_count_ = 1;

	base->free_ = defult_free;
	base->destroy = destroy;
	base->add_ref_copy = add_ref_copy;
	base->add_ref_lock = add_ref_lock;
	base->release = release;
	base->weak_add_ref = weak_add_ref;
	base->weak_release = weak_release;
	base->use_count = use_count;
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

long use_count(sp_counted_base *base)
{
	return (int const volatile)(base->use_count_ );
}
