#include "core/common/CxSys.h"
#include "core/common/CxStr.h"
#include <Windows.h>

namespace cat {

	namespace sys {
		CxI32 init_sp_cwd(CxChar *in_str) {
			mem::set(in_str, 0, sizeof(CxChar)*512);
			const CxI32 len = GetCurrentDirectory(511, in_str);
			
			if (len == 0) { /* error */
				const CxI32 err_no = GetLastError();
				CXD_CRASH("GetCurrentDirectory() failed with %d... terminating.", err_no);
				return -1;
			}

			if (in_str[len-1] != '\\') {
				in_str[len] = '\\';  in_str[len + 1] = '\0';
				return len + 1;
			}
			else { return len; }
		}
		
		CxI32 init_sp_rootDir(CxChar *in_str, const CxChar *in_cwd) {
			/* Find the root part of the current directory */
		   CxI32 i = 0;
			while(in_cwd[i] != 0) {
				in_str[i] = in_cwd[i];
				if (in_cwd[i] == '\\') { ++i;  break; }
				else { ++i; }
			}
			in_str[i] = '\0';
			return i;
		}
		CxChar sp_cwd[512];
		CxChar sp_rootDir[64];
		CxI32 s_cwd_len = init_sp_cwd(sp_cwd);
		CxI32 s_rootDir_len = init_sp_rootDir(sp_rootDir, sp_cwd);
		
		const CxChar * cwd() { return sp_cwd; }
		
		CxChar * getPath(const CxChar *in_fdPath, CxI32 *out_len) {
			/* First, check for null */
			if (in_fdPath == 0 || in_fdPath[0] == 0) {
				if (out_len != 0) { *out_len = 0; }
				return 0;
			}
			CxChar tmp[300];
			CxI32 len = GetFullPathName(in_fdPath, 300, tmp, 0);

			CxChar *path = str::copy(tmp, len);
			if (out_len != 0) { *out_len = len; }
			return path;
		}

		const CxChar * newline() { return "\r\n"; }

		const CxChar * rootDir() { return sp_rootDir; }
		
		void setCwd(const CxChar *in_dir) {
			CXD_IF_CRASH((in_dir == 0 || in_dir[0] == 0),
							 "Cannot set null cwd...terminating.");

			CxI32 len = GetFullPathName(in_dir, 511, sp_cwd, NULL);
			if (sp_cwd[len-1] != '\\') {
				sp_cwd[len] = '\\';  sp_cwd[len + 1] = '\0';
				s_cwd_len = len + 1;
			}
			else { s_cwd_len = len; }

			/* Find the root part of the current directory */
		   CxI32 i = 0;
			while(sp_cwd[i] != 0) {
				sp_rootDir[i] = sp_cwd[i];
				if (sp_cwd[i] == '\\') { ++i;  break; }
				else { ++i; }
			}
			sp_rootDir[i] = '\0';

			/* Now actually change the working directory */
			if (SetCurrentDirectory(sp_cwd) == 0) { /* error */
				const CxI32 err_no = GetLastError();
				CXD_CRASH("SetCurrentDirectory() failed with %d... terminating.", err_no);
			}
		}
		 
	} // namespace sys

} // namespace cat
