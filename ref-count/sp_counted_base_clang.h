#ifndef REF_COUNT_SMART_PTR_DETAIL_SP_COUNTED_BASE_CLANG_HPP_INCLUDED
#define REF_COUNT_SMART_PTR_DETAIL_SP_COUNTED_BASE_CLANG_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <stdint.h>
#include <stdlib.h>


typedef _Atomic( int_least32_t ) atomic_int_least32_t;

inline void atomic_increment( atomic_int_least32_t * pw )
{
    __c11_atomic_fetch_add( pw, 1, __ATOMIC_RELAXED );
}

inline int_least32_t atomic_decrement( atomic_int_least32_t * pw )
{
    return __c11_atomic_fetch_sub( pw, 1, __ATOMIC_ACQ_REL );
}

inline int_least32_t atomic_conditional_increment( atomic_int_least32_t * pw )
{
    // long r = *pw;
    // if( r != 0 ) ++*pw;
    // return r;

    int_least32_t r = __c11_atomic_load( pw, __ATOMIC_RELAXED );

    for( ;; )
    {
        if( r == 0 )
        {
            return r;
        }

        if( __c11_atomic_compare_exchange_weak( pw, &r, r + 1, __ATOMIC_RELAXED, __ATOMIC_RELAXED ) )
        {
            return r;
        }
    }    
}

typedef enum SP_COUNT_TYPE {
	SP_COUNT_UNKNOWN = 0,
	SP_COUNT_GCC,
	SP_COUNT_CLANG
} SP_COUNT_TYPE;

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
    atomic_int_least32_t use_count_;	// #shared
    atomic_int_least32_t weak_count_;	// #weak + (#shared != 0)

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

static inline int add_ref_lock(sp_counted_base *base) // true on success
{
	return atomic_conditional_increment( &base->use_count_ ) != 0;
}

static inline void release(sp_counted_base *base) // nothrow
{
	if( atomic_decrement( &base->use_count_ ) == 1 )
	{
		base->dispose(base);
		base->weak_release(base);
	}
}

static inline void weak_add_ref(sp_counted_base *base) // nothrow
{
	atomic_increment( &base->weak_count_ );
}

static inline void weak_release(sp_counted_base *base) // nothrow
{
	if( atomic_decrement( &base->weak_count_ ) == 1 )
	{
		base->destroy(base);
	}
}

static inline long use_count(sp_counted_base *base)// nothrow
{
	return __c11_atomic_load( (atomic_int_least32_t* )( &base->use_count_ ), __ATOMIC_ACQUIRE );
}

static inline sp_counted_base *sp_counted_new(SP_COUNT_TYPE type)
{
	(void) type;
	sp_counted_base *base = (sp_counted_base *)malloc(sizeof(sp_counted_base));
	__c11_atomic_init( &base->use_count_, 1 );
	__c11_atomic_init( &base->weak_count_, 1 );

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
	__c11_atomic_init( &base->use_count_, 1 );
	__c11_atomic_init( &base->weak_count_, 1 );

	base->free_ = defult_free;
	base->destroy = destroy;
	base->add_ref_copy = add_ref_copy;
	base->add_ref_lock = add_ref_lock;
	base->release = release;
	base->weak_add_ref = weak_add_ref;
	base->weak_release = weak_release;
	base->use_count = use_count;
}


#endif  // #ifndef REF_COUNT_SMART_PTR_DETAIL_SP_COUNTED_BASE_CLANG_HPP_INCLUDED
