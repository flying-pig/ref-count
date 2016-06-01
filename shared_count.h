#ifndef __REF_COUNT_SHARED_COUNT_H_
#define __REF_COUNT_SHARED_COUNT_H_

#include <ref-count/sp_counted_base.h>
#include <ref-count/sp_counted_impl.h>

/* Note: Don't reference an shared_count and weak_count instance in multithread,
 * use it's copy, otherwise may produce an unknown runtime error
 */

typedef struct shared_count_s {
	sp_counted_base *pi_;
} shared_count;

typedef struct weak_count_s {
	sp_counted_base *pi_;
} weak_count;

shared_count *shared_count_new();
shared_count *shared_count_init(shared_count *l, void *p);
shared_count *shared_count_copy_init(shared_count *l, shared_count *r);
shared_count *shared_count_init_from_weak(shared_count *l, weak_count *r);
shared_count *shared_count_copy(shared_count *l, shared_count *r);
void shared_count_swap(shared_count *l, shared_count *r);
void shared_count_release(shared_count *l);
long shared_count_use_count(shared_count *l);


weak_count *weak_count_new();
weak_count *weak_count_copy_init(weak_count *l, weak_count *r);
weak_count *weak_count_init_from_shared(weak_count *l, shared_count *r);
weak_count *weak_count_copy(weak_count *l, weak_count *r);
weak_count *weak_count_copy_from_shared(weak_count *l, shared_count *r);
void weak_count_swap(weak_count *l, weak_count *r);
void weak_count_release(weak_count *l);

#endif /* ifndef __REF_COUNT_SHARED_COUNT_H_ */
