#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsThread.cpp"
#else // POSIX
#  include "posix/CxPOSIXThread.cpp"
#endif

namespace cat {
	void * cx_thread_start_entry__(void *in_data) {
		CxThread *t = reinterpret_cast<CxThread *>(in_data);
		t->setStatus(CxThread::kRunning);
		CxI32 e = t->run();

		if (t->status() == CxThread::kWillTerminate) {
			t->setStatus(CxThread::kTerminated);
		}
		else if (t->status() < CxThread::kFinishedStatusMark) {
			t->setStatus(CxThread::kFinished);
		}
		exit(e);
	}
}






