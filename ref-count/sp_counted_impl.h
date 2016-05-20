#ifndef __REF_COUNT_SP_COUNTED_IMPL_H_
#define __REF_COUNT_SP_COUNTED_IMPL_H_

#include <stdlib.h>
#include <ref-count/sp_counted_base.h>

struct sp_counted_impl_p_s {
	struct sp_counted_base_s;
	void *px_;
} sp_counted_impl_p;

static inline sp_counted_base *sp_counted_impl_p_new(void *px)
{
	sp_counted_impl_p *p = (sp_counted_impl_p *)calloc(1, sizeof(sp_counted_base) + sizeof(sp_counted_impl_p));
	p->px_ = px;
}

#endif /* ifndef __REF_COUNT_SP_COUNTED_IMPL_H_ */
