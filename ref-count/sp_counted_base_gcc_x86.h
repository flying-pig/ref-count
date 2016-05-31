#ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <stdlib.h>

static inline int atomic_exchange_and_add( int * pw, int dv )
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

static inline void atomic_increment( int * pw )
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

static inline int atomic_conditional_increment( int * pw )
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


typedef struct sp_counted_base sp_counted_base;

typedef sp_counted_base* (*new_func)();

typedef void(* free_func)();
typedef void(* dispose_func)(sp_counted_base *);
typedef void(* destroy_func)(sp_counted_base *);
typedef void(* operate_ref_count)(sp_counted_base *);
typedef int(* operate_ref_lock)(sp_counted_base *);
typedef void(* release_func)(sp_counted_base *);
typedef long (*use_count_func)(sp_counted_base *);
typedef void *(*get_deleter_func)(sp_counted_base *);


struct sp_counted_base
{
    int use_count_;        // #shared
    int weak_count_;       // #weak + (#shared != 0)

	free_func free_;
	new_func  new_;

	// dispose() is called when use_count_ drops to zero, to release
	// the resources managed by *this.
    dispose_func dispose;

    // destroy() is called when weak_count_ drops to zero.
	destroy_func destroy;

	get_deleter_func get_deleter;
	get_deleter_func get_untyped_deleter;

	operate_ref_count add_ref_copy;
	operate_ref_lock add_ref_lock;
	release_func release;

	operate_ref_count weak_add_ref;
	release_func weak_release;

	use_count_func use_count;
};

typedef enum SP_COUNT_TYPE {
	SP_COUNT_UNKNOWN = 0,
	SP_COUNT_GCC,
} SP_COUNT_TYPE;

static inline void defult_free()
{
}

static inline void destroy(sp_counted_base *base)
{
	base->free_();
	free(base);
}

static inline void add_ref_copy(sp_counted_base *base)
{
	atomic_increment( &base->use_count_ );
}

static inline int add_ref_lock(sp_counted_base *base)
{
	return atomic_conditional_increment( &base->use_count_ ) != 0;
}

static inline void release(sp_counted_base *base)
{
	if( atomic_exchange_and_add( &base->use_count_, -1 ) == 1 )
	{
		base->dispose(base);
		base->weak_release(base);
	}
}

static inline void weak_add_ref(sp_counted_base *base)
{
	atomic_increment( &base->weak_count_ );
}

static inline void weak_release(sp_counted_base *base)
{
	if( atomic_exchange_and_add( &base->weak_count_, -1 ) == 1 )
	{
		base->destroy(base);
	}
}

static inline long use_count(sp_counted_base *base)
{
	return (int const volatile)(base->use_count_ );
}

static inline sp_counted_base *sp_counted_new(SP_COUNT_TYPE type)
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

static inline void sp_counted_init(sp_counted_base *base)
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


#endif  // #ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED
