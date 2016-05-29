#ifndef __REF_COUNT_H_
#define __REF_COUNT_H_

#include <shared_count.h>

typedef struct shared_ref_s {
	void *px_;
	shared_count pn_;
} shared_ref;

typedef struct weak_ref_s {
	void *px_;
	weak_count pn_;
} weak_ref;

#endif /* ifndef __REF_COUNT_H_ */
