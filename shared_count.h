#ifndef __REF_COUNT_SHARED_COUNT_H_
#define __REF_COUNT_SHARED_COUNT_H_

#include <stdlib.h>
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

#if 0
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
#endif

static inline shared_count *shared_count_new()
{
	return (shared_count *)calloc(1, sizeof(shared_count));
}

static inline shared_count *shared_count_init(shared_count *l, void *p)
{
	l->pi_ = sp_counted_impl_p_new(p);
	if (l->pi_ == NULL) return NULL;

	return l;
}

static inline shared_count *shared_count_copy_init(shared_count *l, shared_count *r)
{
	l->pi_ = r->pi_;
	if (l->pi_ != NULL) l->pi_->add_ref_copy(l->pi_);
	return l;
}

static inline shared_count *shared_count_init_from_weak(shared_count *l, weak_count *r)
{
	l->pi_ = r->pi_;
	if (l->pi_ && !l->pi_->add_ref_lock(l->pi_)) {
		l->pi_ = 0;
	}

	return l;
}
		

/* shared_count & operator=(shared_count &r) */
static inline shared_count *shared_count_copy(shared_count *l, shared_count *r)
{
	sp_counted_base *tmp = r->pi_;

	if (tmp != l->pi_) {
		if (tmp != NULL) tmp->add_ref_copy(r->pi_);
		if (l->pi_ != NULL) l->pi_->release(l->pi_);
		l->pi_ = tmp;
	}

	return l;
}

static inline void shared_count_swap(shared_count *l, shared_count *r)
{
	sp_counted_base *tmp = r->pi_;
	r->pi_ = l->pi_;
	l->pi_ = tmp;
}

static inline void shared_count_release(shared_count *l)
{
	if (l && l->pi_ != NULL) l->pi_->release(l->pi_);
}

static inline long shared_count_use_count(shared_count *l)
{
	return l->pi_ != NULL ? l->pi_->use_count(l->pi_) : 0;
}

/*
 * weak_count
 */
static inline weak_count *weak_count_new()
{
	return (weak_count *)calloc(1, sizeof(weak_count));
}

static inline weak_count *weak_count_copy_init(weak_count *l, weak_count *r)
{
	l->pi_ = r->pi_;
	if (l->pi_ != NULL) l->pi_->weak_add_ref(l->pi_);

	return l;
}

static inline weak_count *weak_count_init_from_shared(weak_count *l, shared_count *r)
{
	l->pi_ = r->pi_;
	if (l->pi_ != NULL) {
		l->pi_->weak_add_ref(l->pi_);
	}
	return l;
}

/* weak_count & operator=(weak_count &r) */
static inline weak_count *weak_count_copy(weak_count *l, weak_count *r)
{
	sp_counted_base *tmp = r->pi_;
	if (tmp != l->pi_) {
		if (tmp != NULL) tmp->weak_add_ref(r->pi_);
		if (l->pi_ != NULL) l->pi_->weak_release(l->pi_);
		l->pi_ = tmp;
	}

	return l;
}

/* weak_count & operator= (shared_count const & r) */
static inline weak_count *weak_count_copy_from_shared(weak_count *l, shared_count *r)
{
	sp_counted_base *tmp = r->pi_;
	if (tmp != l->pi_) {
		if (tmp != NULL) tmp->weak_add_ref(r->pi_);
		if (l->pi_ != NULL) l->pi_->weak_release(l->pi_);
		l->pi_ = tmp;
	}
	return l;
}

static inline void weak_count_swap(weak_count *l, weak_count *r)
{
	sp_counted_base *tmp = r->pi_;
	r->pi_ = l->pi_;
	l->pi_ = tmp;
}

static inline void weak_count_release(weak_count *l)
{
	if (l && l->pi_ != NULL) l->pi_->weak_release(l->pi_);
}

#endif /* ifndef __REF_COUNT_SHARED_COUNT_H_ */
