#ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED
#define BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <stdlib.h>

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
	char impl[];
};

typedef enum SP_COUNT_TYPE {
	SP_COUNT_UNKNOWN = 0,
	SP_COUNT_GCC,
} SP_COUNT_TYPE;

void defult_free();
sp_counted_base *sp_counted_new(SP_COUNT_TYPE type);
void destroy(sp_counted_base *base);
void add_ref_copy(sp_counted_base *base);
int add_ref_lock(sp_counted_base *base);
void weak_add_ref(sp_counted_base *base);
void release(sp_counted_base *base);
void weak_release(sp_counted_base *base);
long use_count(sp_counted_base *base);

#endif  // #ifndef BOOST_SMART_PTR_DETAIL_SP_COUNTED_BASE_GCC_X86_HPP_INCLUDED
