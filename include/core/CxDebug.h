#ifndef CX_CORE_CXDEBUG_H
#define CX_CORE_CXDEBUG_H

/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxDebug.h
 * @brief Header file with a bunch of debug macros.
 *
 * @author Catlin Zilinski
 * @date May 9, 2015
 */

#if defined(DEBUG)
#  define IS_DEBUG 1
#  include <assert.h>
#  include <stdio.h>

#  define CXD_CRASH(fmt, ...)												\
	do { if (IS_DEBUG) {														\
			fprintf(stderr, "#----> FATAL %s:%d:%s(): " fmt "\n",	\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stderr);													\
			assert(false);														\
		}																			\
	} while (0)

#  define CXD_ERR(fmt, ...)												\
	do { if (IS_DEBUG) {														\
			fprintf(stderr, "#----> ERROR %s:%d:%s(): " fmt "\n",	\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stderr);													\
		}																			\
	} while (0)

#  define CXD_WARN(fmt, ...)												\
	do { if (IS_DEBUG) {														\
			fprintf(stderr, "#--> Warning %s:%d:%s(): " fmt "\n",	\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stderr);													\
		}																			\
	} while (0)

#  define CXD_MSG(fmt, ...)												\
	do { if (IS_DEBUG) {														\
			fprintf(stdout, "#> %s:%d:%s(): " fmt "\n",				\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stdout);													\
		}																			\
	} while (0)

#  define CXD_OUT(fmt, ...)														\
	do { if (IS_DEBUG) fprintf(stdout, fmt, __VA_ARGS__); } while (0)

#  define CXD_IF_ERR(cond, fmt, ...)										\
	do { if (IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stderr, "#----> ERROR %s:%d:%s(): " fmt "\n",	\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stderr);													\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_WARN(cond, fmt, ...)										\
	do { if (IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stderr, "#--> Warning %s:%d:%s(): " fmt "\n",	\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stderr);													\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_MSG(cond, fmt, ...)											\
	do { if (IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stdout, "#> %s:%d:%s(): " fmt "\n",				\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stdout);													\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_OUT(cond, fmt, ...)									\
	do { if (IS_DEBUG) {													\
			if (cond) { fprintf(stdout, fmt, __VA_ARGS__); }	\
		}																		\
	} while (0)

/* Check for NaN */
#  define CXD_ISNUM(x) do { if(IS_DEBUG) assert((x) == (x)); } while (0)

/* Check for infinite float */
#  define CXD_ISFINITE_F32(x) do { if(IS_DEBUG) assert((x) <= CX_MAX_F32 && (x) >= CX_MIN_F32); } while (0)
#  define CXD_ISFINITE_F64(x) do { if(IS_DEBUG) assert((x) <= CX_MAX_F64 && (x) >= CX_MIN_F64); } while (0)
#  define CXD_ISFINITE_REAL(x) do { if(IS_DEBUG) assert((x) <= CX_MAX_REAL && (x) >= CX_MIN_REAL); } while (0)

/* Check for NaN and infinite. */
#  define CXD_ISVALID_F32(x)										\
	do { if(IS_DEBUG) {												\
			assert((x) == (x));										\
			assert((x) <= CX_MAX_F32 && (x) >= CX_MIN_F32);	\
		}																	\
	} while (0)

#  define CXD_ISVALID_F64(x)										\
	do { if(IS_DEBUG) {												\
			assert((x) == (x));										\
			assert((x) <= CX_MAX_F64 && (x) >= CX_MIN_F64);	\
		}																	\
	} while (0)

#  define CXD_ISVALID_REAL(x)											\
	do { if(IS_DEBUG) {													\
			assert((x) == (x));											\
			assert((x) <= CX_MAX_REAL && (x) >= CX_MIN_REAL);	\
		}																		\
	} while (0)

#  define CX_TODO(x)
#else
#  define IS_DEBUG 0
#  define CXD_CRASH(fmt, ...)
#  define CXD_ERR(fmt, ...)
#  define CXD_WARN(fmt, ...)
#  define CXD_MSG(fmt, ...)
#  define CXD_OUT(fmt, ...)
#  define CXD_IF_ERR(cond, fmt, ...)
#  define CXD_IF_WARN(cond, fmt, ...)
#  define CXD_IF_MSG(cond, fmt, ...)
#  define CXD_IF_OUT(cond, fmt, ...)
#  define CXD_ISNUM(x)
#  define CXD_ISFINITE_F32(x)
#  define CXD_ISFINITE_F64(x)
#  define CXD_ISFINITE_REAL(x)
#  define CXD_ISVALID_F32(x)
#  define CXD_ISVALID_F64(x)
#  define CXD_ISVALID_REAL(x)

#endif // defined DEBUG && !defined(STRIP_DEBUG)

#endif // CX_CORE_CXDEBUG_H
