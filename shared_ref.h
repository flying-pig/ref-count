#ifndef __REF_COUNT_SHARED_REF_H_
#define __REF_COUNT_SHARED_REF_H_ 

#include <shared_count.h>

#define swap(x, y) do{ typeof((x)) tmp = (y); (y) = (x); (x) = tmp; }while(0);

typedef struct shared_ref_s {
	void *px_;
	shared_count pn_;
} shared_ref;

#define SHARED_REF_INIT() { NULL, {NULL}}
#define SHARED_REF_DEF(name) \
	shared_ref name = SHARED_REF_INIT()

static inline shared_ref *shared_ref_new()
{
	return (shared_ref *)calloc(1, sizeof(shared_ref));
}

static inline shared_ref *shared_ref_init(shared_ref *l, void *p)
{
	l->px_ = p;
	void *s = shared_count_init(&l->pn_, p);
	if (s == NULL) return NULL;
	return l;
}

static inline void shared_ref_release(shared_ref *l)
{
	shared_count_release(&l->pn_);
}

static inline void *shared_ref_get(shared_ref *l)
{
	return l->px_;
}

static inline void shared_ref_swap(shared_ref *l, shared_ref *r)
{
	swap(l->px_, r->px_);
	shared_count_swap(&l->pn_, &r->pn_);
}

static inline void shared_ref_reset(shared_ref *l, void *p)
{
	if (p == NULL) {
		SHARED_REF_DEF(tmp);
		shared_ref_swap(&tmp, l);
		shared_ref_release(&tmp);
	} else {
		SHARED_REF_DEF(tmp);
		shared_ref_init(&tmp, p);
		shared_ref_swap(&tmp, l);
		shared_ref_release(&tmp);
	}
}

#endif /* ifndef __REF_COUNT_SHARED_REF_H_ */
