#ifndef CX_CORE_CXTESTCORE_H
#define CX_CORE_CXTESTCORE_H

#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>

#define BEGIN_TEST (std::cout << ">>> BEGINNING " << __FUNCTION__ << std::endl << std::flush)
#define FINISH_TEST (std::cout << ">>> FINISHED " << __FUNCTION__ << std::endl << std::endl << std::flush)

#define ass_eq(av, ev)																	\
	if ((ev) != (av)) {																	\
		DERR("ASSERT FAILED: [ " #av " == " #ev " ] --->> [ " << (av) << " == " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((ev) == (av));											

#define ass_neq(av, ev)																	\
	if ((ev) == (av)) {																	\
		DERR("ASSERT FAILED: [ " #av " != " #ev " ] --->> [ " << (av) << " != " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((ev) != (av));											
	
#define ass_lt(av, ev)																	\
	if ((av) >= (ev)) {																	\
		DERR("ASSERT FAILED: [ " #av " < " #ev " ] --->> [ " << (av) << " < " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) < (ev));

#define ass_le(av, ev)																	\
	if ((av) > (ev)) {																	\
		DERR("ASSERT FAILED: [ " #av " <= " #ev " ] --->> [ " << (av) << " <= " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) <= (ev));

#define ass_gt(av, ev)																	\
	if ((av) <= (ev)) {																	\
		DERR("ASSERT FAILED: [ " #av " > " #ev " ] --->> [ " << (av) << " > " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) > (ev));											

#define ass_ge(av, ev)																	\
	if ((av) < (ev)) {																	\
		DERR("ASSERT FAILED: [ " #av " >= " #ev " ] --->> [ " << (av) << " >= " << (ev) << " ] is FALSE!"); \
	}																							\
	assert((av) >= (ev));
	

#define ass_len(obj, el)																\
	if ((obj).length() != (el)) {														\
		DERR("ASSERT FAILED: [ " #obj ".length() == " #el " ] --->> [ " << (obj).length() << " != " << (el) << " ] is FALSE!"); \
	}																							\
	assert((obj).length() == (el));

#define ass_true(value)																\
	if (!(value)) {																	\
		DERR("ASSERT FAILED: " #value " should be: TRUE, is FALSE");	\
	}																						\
	assert((value));

#define ass_false(value)															\
	if ((value)) {																		\
		DERR("ASSERT FAILED: " #value " should be: FALSE, is TRUE");	\
	}																						\
	assert(!(value));

#endif // CX_CORE_CXTESTCORE_H
