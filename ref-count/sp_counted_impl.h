#ifndef __REF_COUNT_SP_COUNTED_IMPL_H_
#define __REF_COUNT_SP_COUNTED_IMPL_H_

#include <stdlib.h>
#include <ref-count/sp_counted_base.h>

typedef struct sp_counted_impl_p_s {
	sp_counted_base base;
	void *px_;
} sp_counted_impl_p;

static inline sp_counted_base *sp_counted_impl_p_new(void *px)
{
	sp_counted_impl_p *p = (sp_counted_impl_p *)calloc(1, sizeof(sp_counted_impl_p));
	sp_counted_init((sp_counted_base *)p);
	p->px_ = px;
}

#endif /* ifndef __REF_COUNT_SP_COUNTED_IMPL_H_ */
