#include "core/common/CxSys.h"
#include "core/common/CxStr.h"
#include <unistd.h>
#include <errno.h>

namespace cat {

	namespace sys {
		CxI32 init_sp_cwd(CxChar *in_str) {
			mem::set(in_str, 0, sizeof(CxChar)*512);
			if (getcwd(in_str, 511) == 0) { /* error */
				const CxI32 err_no = errno;
				CxChar err_str[16];
				switch(err_no) {
				case EACCES: err_str = "EACCES";  break;
				case EFAULT: err_str = "EFAULT";  break;
				case EINVAL: err_str = "EINVAL";  break;
				case ENAMETOOLONG: err_str = "ENAMETOOLONG";  break;
				case ENOMEM: err_str = "ENOMEM";  break;
				case ENOENT: err_str = "ENOENT";  break;
				case ERANGE: err_str = "ERANGE";  break;
				default: err_str = "Unknown error";  break;
				}
				CXD_CRASH("getcwd() failed with %s... terminating program.", err_str);
				return -1;
			}
			const CxI32 len = str::len(in_str);
			if (in_str[len-1] != '/') {
				in_str[len] = '/';  in_str[len + 1] = '\0';
				return len + 1;
			}
			else { return len; }
		}
		CxChar sp_cwd[512];
		CxI32 s_cwd_len = init_sp_cwd(sp_cwd);

		
		const CxChar * cwd() { return sp_cwd; }
		
		CxChar * getPath(const CxChar *in_fdPath, CxI32 *out_len) {
			/* First, check for null */
			if (in_fdPath == 0 || in_fdPath[0] == 0) {
				if (out_len != 0) { *out_len = 0; }
				return 0;
			}
			CxI32 len = str::len(in_fdPath);

			/* Check to see if need to remove any trailing slashes */
			CxI32 i = len - 1;
			while (i > 0 && (in_fdPath[i] == '/' && in_fdPath[i-1] == '/')) {
				--len;  --i;
			}

			CxChar *path = 0;
			CxI32 new_len = 0;
			/* Check to see if path is absolute or relative. */
			if (in_fdPath[0] != '/') { /* relative */
				const CxI32 cwd_len = s_cwd_len;
				new_len = len + cwd_len;
			   path = (CxChar *)mem::alloc(sizeof(CxChar)*new_len);
				mem::copy(path, sp_cwd, sizeof(CxChar)*cwd_len);
				mem::copy(path + cwd_len, in_fdPath, sizeof(CxChar)*len);
			}
			else { /* absolute */
				new_len = len;
			   path = (CxChar *)mem::alloc(sizeof(CxChar)*len);
				mem::copy(path, in_fdPath, sizeof(CxChar)*len);
			}
			path[new_len] = '\0';
			if (out_len != 0) { *out_len = new_len; }
			return path;
		}

		const CxChar * newline() { return "\n"; }

		const CxChar * rootDir() { return "/"; }

		void setCwd(const CxChar *in_dir) {
			CXD_IF_CRASH((in_dir == 0 || in_dir[0] == 0),
							 "Cannot set null cwd...terminating.");
			const CxI32 len = str::len(in_dir);
			CXD_IF_CRASH((len > 510), "cwd path too long... terminating.");

			mem::copy(sp_cwd, in_dir, sizeof(CxChar)*len);
			if (in_dir[len-1] != '/') {
				sp_cwd[len] = '/';  sp_cwd[len + 1] = '\0';
				s_cwd_len = len + 1;
			}
			else { s_cwd_len = len; }

			/* Now actually change the working directory */
			if (chdir(sp_cwd) != 0) { /* error */
				const CxI32 err_no = errno;
				CxChar err_str[16];
				switch(err_no) {
				case EACCES: err_str = "EACCES";  break;
				case EFAULT: err_str = "EFAULT";  break;
				case EIO: err_str = "EIO";  break;
				case ENAMETOOLONG: err_str = "ENAMETOOLONG";  break;
				case ELOOP: err_str = "ELOOP";  break;
				case ENOMEM: err_str = "ENOMEM";  break;
				case ENOENT: err_str = "ENOENT";  break;
				case ENOTDIR: err_str = "ENOTDIR";  break;
				default: err_str = "Unknown error";  break;
				}
				CXD_CRASH("chdir() failed with %s... terminating program.", err_str);
			}
		}
		 
	} // namespace sys

} // namespace cat
