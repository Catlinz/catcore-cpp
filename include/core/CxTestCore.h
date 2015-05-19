#ifndef CX_CORE_CXTESTCORE_H
#define CX_CORE_CXTESTCORE_H

#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __func__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __func__ << std::endl << std::endl << std::flush)
#define CXD_ASS_FAILED(msg) (std::cout << "#----> ASSERT FAILED " << __func__ << ":" << __LINE__ << "(): " << msg << std::endl << std::flush)

#define ass_eq(av, ev)																	\
	if ((ev) != (av)) {																	\
		CXD_ASS_FAILED(#av " == " #ev " ] --->> [ " << (av) << " == " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((ev) == (av));

#define ass_zero(value)																	\
	if ((value) != 0) {																	\
		CXD_ASS_FAILED(#value " should be ZERO but is [" << (value) << " instead!"); \
	}																							\
	assert((ev) == (av));

#define ass_neq(av, ev)																	\
	if ((ev) == (av)) {																	\
	   CXD_ASS_FAILED(#av " != " #ev " ] --->> [ " << (av) << " != " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((ev) != (av));
	
#define ass_lt(av, ev)																	\
	if ((av) >= (ev)) {																	\
		CXD_ASS_FAILED(#av " < " #ev " ] --->> [ " << (av) << " < " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) < (ev));

#define ass_le(av, ev)																	\
	if ((av) > (ev)) {																	\
		CXD_ASS_FAILED(#av " <= " #ev " ] --->> [ " << (av) << " <= " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) <= (ev));

#define ass_gt(av, ev)																	\
	if ((av) <= (ev)) {																	\
		CXD_ASS_FAILED(#av " > " #ev " ] --->> [ " << (av) << " > " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) > (ev));											

#define ass_ge(av, ev)																	\
	if ((av) < (ev)) {																	\
		CXD_ASS_FAILED(#av " >= " #ev " ] --->> [ " << (av) << " >= " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) >= (ev));
	

#define ass_len(obj, el)																\
	if ((obj).length() != (el)) {														\
		CXD_ASS_FAILED(#obj ".length() == " #el " ] --->> [ " << (obj).length() << " != " << (el) << " ] is FALSE!"); \
	}																							\
	assert((obj).length() == (el));

#define ass_true(value)													\
	if (!(value)) {														\
		CXD_ASS_FAILED(#value " should be: TRUE, is FALSE");	\
	}																			\
	assert((value));

#define ass_false(value)												\
	if ((value)) {															\
		CXD_ASS_FAILED(#value " should be: FALSE, is TRUE");	\
	}																			\
	assert(!(value));

#endif // CX_CORE_CXTESTCORE_H
