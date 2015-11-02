#include "core/common/CxSys.h"
#include "core/CxTestCore.h"

#include "core/common/CxStr.h"

#if defined(CX_WINDOWS)
#  include <Windows.h>
#else
#  include <unistd.h>
#endif

namespace cat {

	/**
	 * @tests sys::newline()
	 * @tests sys::pathSep()
	 */
	void testCxSysNewlineAndPathSep() {
		BEGIN_TEST;

		const CxChar *nl = sys::newline();
#if defined (CX_WINDOWS)
		ass_eq(sys::pathSep(), '\\');
	   ass_eq(nl[0], '\r');
		ass_eq(nl[1], '\n');
		ass_eq(nl[2], '\0');
#else
		ass_eq(sys::pathSep(), '/');
		ass_eq(nl[0], '\n');
		ass_eq(nl[1], '\0');
#endif

		FINISH_TEST;
	}

	/**
	 * @tests sys::cwd()
	 * @tests sys::rootDir()
	 */
	void testCxSysCwd() {
		BEGIN_TEST;

		const CxChar *cwd = sys::cwd();
		CxI32 cwd_len = str::len(cwd);
		const CxChar *rdir = sys::rootDir();
		CxI32 rdir_len = str::len(rdir);
		
		ass_true(cwd != 0 && cwd_len > 0);
		ass_true(rdir != 0 && rdir_len > 0);
		ass_true(str::startsWith(cwd, rdir));
		
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(cwd, "\\catcore-cpp\\tests\\unit\\Core\\test_bin\\"));
		CxChar *ptr = (CxChar *)mem::find(cwd, '\\', sizeof(CxChar)*cwd_len);
		ass_eq(*ptr, '\\');
		ass_eq(*(ptr - 1), ':');
		ass_eq(rdir[rdir_len], '\0');
		ass_eq(rdir[rdir_len - 1], '\\');
		ass_eq(rdir[rdir_len - 2], ':');
		ass_true(rdir_len > 2);
#else
		ass_true(str::endsWith(cwd, "/catcore-cpp/tests/unit/Core/test_bin/"));
		ass_eq(cwd[0], '/');
		ass_eq(rdir[0], '/');
		ass_eq(rdir[1], '\0');
#endif
		
		FINISH_TEST;
	}

	/**
	 * @tests sys::setCwd(const CxChar *)
	 */
	void testCxSysSetCwd() {
		BEGIN_TEST;

		CxChar *root = str::copy(sys::rootDir());
		CxChar *orig = str::copy(sys::cwd());
		
		CxChar tmp[512];
		sys::setCwd("../../../../include");
		ass_true(str::startsWith(sys::cwd(), root));
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(sys::cwd(), "\\catcore-cpp\\include\\"));
		GetCurrentDirectory(512, tmp);
		ass_true(str::startsWith(sys::cwd(), tmp));
#else
		ass_true(str::endsWith(sys::cwd(), "/catcore-cpp/include/"));
		getcwd(tmp, 512);
		ass_true(str::startsWith(sys::cwd(), tmp));
#endif

		sys::setCwd("../doc///");
		ass_true(str::startsWith(sys::cwd(), root));
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(sys::cwd(), "\\catcore-cpp\\doc\\"));
		GetCurrentDirectory(512, tmp);
		ass_true(str::startsWith(sys::cwd(), tmp));
#else
		ass_true(str::endsWith(sys::cwd(), "/catcore-cpp/doc/"));
		getcwd(tmp, 512);
		ass_true(str::startsWith(sys::cwd(), tmp));
#endif

		sys::setCwd("../src/core/common/../math");
		ass_true(str::startsWith(sys::cwd(), root));
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(sys::cwd(), "\\catcore-cpp\\src\\core\\math\\"));
		GetCurrentDirectory(512, tmp);
		ass_true(str::startsWith(sys::cwd(), tmp));
#else
		ass_true(str::endsWith(sys::cwd(), "/catcore-cpp/src/core/math/"));
		getcwd(tmp, 512);
		ass_true(str::startsWith(sys::cwd(), tmp));
#endif
		
		sys::setCwd("/");
		ass_true(str::eq(sys::cwd(), root));

		sys::setCwd(orig);
		mem::free(orig);
		mem::free(root);
		FINISH_TEST;
	}

	/**
	 * @tests sys::getPath(const CxChar *, CxI32 *)
	 */
	void testCxSysGetPath() {
		BEGIN_TEST;

		CxChar *p0 = sys::getPath("../../../../include");
		ass_true(str::startsWith(p0, sys::rootDir()));
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(p0, "\\catcore-cpp\\include"));
#else
		ass_true(str::endsWith(sys::cwd(), "/catcore-cpp/include"));
#endif
		mem::free(p0);

		sys::setCwd("../../../../");
		p0 = sys::getPath("include");
		ass_true(str::startsWith(p0, sys::cwd()));
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(p0, "\\catcore-cpp\\include"));
#else
		ass_true(str::endsWith(sys::cwd(), "/catcore-cpp/include"));
#endif
		mem::free(p0);

		p0 = sys::getPath("src/core/common/../math///");
		ass_true(str::startsWith(p0, sys::cwd()));
#if defined (CX_WINDOWS)
		ass_true(str::endsWith(p0, "\\catcore-cpp\\src\\core\\math\\"));
#else
		ass_true(str::endsWith(sys::cwd(), "/catcore-cpp/src/core/math/"));
#endif
		mem::free(p0);

		FINISH_TEST;
	}
	
} // namespace cat


int main(int argc, char **argv) {
	cat::testCxSysNewlineAndPathSep();
	cat::testCxSysCwd();
	cat::testCxSysSetCwd();
	cat::testCxSysGetPath();

	return 0;
}
