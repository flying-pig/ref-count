#ifndef __REF_COUNT_WEAK_REF_H_
#define __REF_COUNT_WEAK_REF_H_

#include <ref_count.h>
#include <shared_ref.h>

#define WEAK_REF_INIT() { NULL, {NULL}} 
#define WEAK_REF_DEF(name) \
	weak_ref name = WEAK_REF_INIT()

static inline weak_ref *weak_ref_new()
{
	return (weak_ref *)calloc(1, sizeof(weak_ref));
}

static inline weak_ref *weak_ref_init(weak_ref *l, shared_ref *r)
{
	l->px_ = r->px_;
	weak_count_init_from_shared(&l->pn_, &r->pn_);
	return l;
}

static inline shared_ref *weak_ref_lock(shared_ref *l, weak_ref *r)
{
	return shared_ref_init_from_weak(l, r);
}

static inline void weak_ref_release(weak_ref *l)
{
	weak_count_release(&l->pn_);
}

static inline weak_ref *weak_ref_copy_init(weak_ref *l, weak_ref *r)
{
	l->px_ = r->px_;
	weak_count_copy_init(&l->pn_, &r->pn_);
	return l;
}

static inline weak_ref *weak_ref_copy(weak_ref *l, weak_ref *r)
{
	l->px_ = r->px_;
	weak_count_copy(&l->pn_, &r->pn_);
	return l;
}

#endif /* ifndef __REF_COUNT_WEAK_REF_H_ */
