#ifndef CAT_CORE_TESTCORE_H
#define CAT_CORE_TESTCORE_H

#include <assert.h>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <unistd.h>
#include <cstring>

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

namespace Cat {
	/**
	 * @brief Get an absolute filename relative to the /tests/ dir.
	 * @param filename The filename of the filename to get.
	 * @return An absolute path of the filename.
	 */
	inline char* getFilenameUnderTestDir(const char* filename) {
		char cwd[1024];
		getcwd(cwd, 1024);

		char testDir[] = "CatzToy/tests/";		

		char* ptr = strstr(cwd, testDir);
		ptr += strlen(testDir);
		*ptr = '\0';
		size_t cwdLen = strlen(cwd);
		size_t filenameLen = strlen(filename);		
		char* absFilename = new char[cwdLen + filenameLen + 1];
		memcpy(absFilename, cwd, sizeof(char)*cwdLen);
		memcpy(&(absFilename[cwdLen]), filename, sizeof(char)*filenameLen);
		absFilename[cwdLen + filenameLen] = '\0';
		
		return absFilename;
	}	
		
		
		
}



#endif // CAT_CORE_TESTCORE_H
