#ifndef __REF_COUNT_SP_COUNTED_IMPL_H_
#define __REF_COUNT_SP_COUNTED_IMPL_H_

#include <stdlib.h>
#include <ref-count/sp_counted_base.h>

typedef struct sp_counted_impl_p_s {
	sp_counted_base base;
	void *px_;
} sp_counted_impl_p;

static inline void sp_counted_impl_despose(sp_counted_base *pi)
{
  if (pi != NULL) free(((sp_counted_impl_p *)pi)->px_);
}

static inline void *sp_counted_impl_get_deleter(sp_counted_base *pi)
{
  (void)pi;
  return NULL;
}

static inline void *sp_counted_impl_get_untyped_deleter(sp_counted_base *pi)
{
  (void)pi;
  return NULL;
}

static inline sp_counted_base *sp_counted_impl_p_new(void *px)
{
	sp_counted_impl_p *p = (sp_counted_impl_p *)calloc(1, sizeof(sp_counted_impl_p));
	sp_counted_init((sp_counted_base *)p);
	p->px_ = px;
  p->base.dispose = sp_counted_impl_despose;
  p->base.get_deleter = sp_counted_impl_get_deleter;
  p->base.get_untyped_deleter = sp_counted_impl_get_untyped_deleter;
  return (sp_counted_base *)p;
}

#endif /* ifndef __REF_COUNT_SP_COUNTED_IMPL_H_ */
