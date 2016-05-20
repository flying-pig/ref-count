#ifndef __REF_COUNT_SHARED_COUNT_H_
#define __REF_COUNT_SHARED_COUNT_H_

#include <ref-count/sp_counted_base.h>
#include <ref-count/sp_counted_impl.h>

typedef struct shared_count_s {
	sp_counted_base *pi_;
} shared_count;

shared_count *shared_count_new();

#endif /* ifndef __REF_COUNT_SHARED_COUNT_H_ */
