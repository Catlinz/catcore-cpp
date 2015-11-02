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

#if !defined(DEBUG)
#  if defined(_DEBUG) || defined(_DEBUG_) || defined(DEBUG_)
#    define DEBUG
#  endif
#endif

#if defined(DEBUG_2)
#  if !defined(DEBUG)
#    define DEBUG
#  endif
#endif

#if defined(DEBUG)
#  define CX_DEBUG
#  define CX_IS_DEBUG 1
#  include <assert.h>
#  include <stdio.h>

#  define CXD_ASSERT(exp) assert(exp)

#  define CXD_CRASH(fmt, ...)												\
	do { if (CX_IS_DEBUG) {													\
			fprintf(stderr, "#----> FATAL %s:%d:%s(): " fmt "\n",	\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stderr);													\
			assert(false);														\
		}																			\
	} while (0)

#  define CXD_ERR(fmt, ...)												\
	do { if (CX_IS_DEBUG) {														\
			fprintf(stderr, "#----> ERROR %s:%d:%s(): " fmt "\n",	\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stderr);													\
		}																			\
	} while (0)

#  define CXD_WARN(fmt, ...)												\
	do { if (CX_IS_DEBUG) {														\
			fprintf(stderr, "#--> Warning %s:%d:%s(): " fmt "\n",	\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stderr);													\
		}																			\
	} while (0)

#  define CXD_MSG(fmt, ...)												\
	do { if (CX_IS_DEBUG) {														\
			fprintf(stdout, "#> %s:%d:%s(): " fmt "\n",				\
					  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
			fflush(stdout);													\
		}																			\
	} while (0)

#  define CXD_OUT(fmt, ...)														\
	do { if (CX_IS_DEBUG) fprintf(stdout, fmt, __VA_ARGS__); } while (0)

#  define CXD_IF_CRASH(cond, fmt, ...)										\
	do { if (CX_IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stderr, "#----> FATAL %s:%d:%s(): " fmt "\n",	\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stderr);													\
				assert(false);														\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_ERR(cond, fmt, ...)										\
	do { if (CX_IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stderr, "#----> ERROR %s:%d:%s(): " fmt "\n",	\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stderr);													\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_WARN(cond, fmt, ...)										\
	do { if (CX_IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stderr, "#--> Warning %s:%d:%s(): " fmt "\n",	\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stderr);													\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_MSG(cond, fmt, ...)											\
	do { if (CX_IS_DEBUG) {															\
			if (cond) {																\
				fprintf(stdout, "#> %s:%d:%s(): " fmt "\n",				\
						  __FILE__, __LINE__, __func__, __VA_ARGS__);	\
				fflush(stdout);													\
			}																			\
		}																				\
	} while (0)

#  define CXD_IF_OUT(cond, fmt, ...)									\
	do { if (CX_IS_DEBUG) {													\
			if (cond) { fprintf(stdout, fmt, __VA_ARGS__); }	\
		}																		\
	} while (0)

/* Check for NaN */
#  define CXD_ISNUM(x) do { if(CX_IS_DEBUG) assert((x) == (x)); } while (0)

/* Check for infinite float */
#  define CXD_ISFINITE_F32(x) do { if(CX_IS_DEBUG) assert((x) <= CX_MAX_F32 && (x) >= CX_MIN_F32); } while (0)
#  define CXD_ISFINITE_F64(x) do { if(CX_IS_DEBUG) assert((x) <= CX_MAX_F64 && (x) >= CX_MIN_F64); } while (0)
#  define CXD_ISFINITE_REAL(x) do { if(CX_IS_DEBUG) assert((x) <= CX_MAX_REAL && (x) >= CX_MIN_REAL); } while (0)

/* Check for NaN and infinite. */
#  define CXD_ISVALID_F32(x)										\
	do { if(CX_IS_DEBUG) {												\
			assert((x) == (x));										\
			assert((x) <= CX_MAX_F32 && (x) >= CX_MIN_F32);	\
		}																	\
	} while (0)

#  define CXD_ISVALID_F64(x)										\
	do { if(CX_IS_DEBUG) {												\
			assert((x) == (x));										\
			assert((x) <= CX_MAX_F64 && (x) >= CX_MIN_F64);	\
		}																	\
	} while (0)

#  define CXD_ISVALID_REAL(x)											\
	do { if(CX_IS_DEBUG) {													\
			assert((x) == (x));											\
			assert((x) <= CX_MAX_REAL && (x) >= CX_MIN_REAL);	\
		}																		\
	} while (0)

#  define CX_TODO(x)
#  if defined(DEBUG_2)
#    define CXD2_ASSERT(exp) assert(exp)
#  else
#    define CXD2_ASSERT(exp)
#  endif // DEBUG_2
#else
#  define CX_IS_DEBUG 0
#  define CXD_ASSERT(exp)
#  define CXD_CRASH(fmt, ...)
#  define CXD_ERR(fmt, ...)
#  define CXD_WARN(fmt, ...)
#  define CXD_MSG(fmt, ...)
#  define CXD_OUT(fmt, ...)
#  define CXD_IF_CRASH(cond, fmt, ...)
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

#  define CXD2_ASSERT(exp)

#endif // defined DEBUG && !defined(STRIP_DEBUG)

#endif // CX_CORE_CXDEBUG_H
