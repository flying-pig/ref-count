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

shared_count *shared_count_new();

typedef struct weak_count_s {
	sp_counted_base *pi_;
} weak_count;

weak_count *weak_count_new();

#endif /* ifndef __REF_COUNT_SHARED_COUNT_H_ */
