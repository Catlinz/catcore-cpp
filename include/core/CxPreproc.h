#ifndef CX_CORE_CXPREPROC_H
#define CX_CORE_CXPREPROC_H

/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPreproc.h
 * @brief Preprocessor macros for figuring out system information.
 *
 * @author Catlin Zilinski
 * @date May 8, 2015
 */

#include <stddef.h>

/* First, figure out what compiler we're using. */
#ifdef _MSC_VER  /* Check for Microsoft Visual C++ on windows */
#  define CX_MVC
/* Check to version of the visual studio compiler */
#  if _MSC_VER >= 1800
#    define CX_MVC12
#  elif _MSC_VER >= 1700
#    define CX_MVC11
#  elif _MSC_VER >= 1600
#    define CX_MVC10
#  elif _MSC_VER >= 1500
#    define CX_MVC9
#  elif _MSC_VER >= 1400
#    define CX_MVC8
#  elif _MSC_VER >= 1300
#    define CX_MVC7
#  elif _MSC_VERT >= 1200
#    define CX_MCV6
#  else
#    define CX_MCV5
#  endif
#elif defined(__ghs__) /* Check for Green Hill C/C++ compiler (WII U). */
#  define CX_GHS
#elif defined(__clang__) /* Check for Clang (GNUC compatible) */
#  define CX_GNUC_COMPILER
#  define CX_CLANG
#elif defined(__SNC__) /* Check for SNC compiler (consoles) (GNUC compatible) */
#  define CX_GNUC_COMPILER
#  define CX_SNC
#elif defined(__CC_ARM) /* Check for ARM compiler (Mostly compatible with GNUC */
#  define CX_GNUC_COMPILER
#  define CX_ARMCC
#elif defined(__INTEL_COMPILER) /* Check for Intel Compiler (Should be GNUC compatible */
#  define CX_GNUC_COMPILER
#  define CX_INTEL_COMPILER
#elif defined(__MINGW32__) /* Check for MinGW */
#  define CX_GNUC_COMPILER
#  define CX_MINGW
#  define CX_MINGW32
#elif defined(__MINGW64__) /* Check for MinGW 64 bit */
#  define CX_GNUC_COMPILER
#  define CX_MINGW
#  define CX_MINGW64
#elif !defined(__GNUC__) /* If we don't have GNUC then we don't know the compiler */
#  error "Unknown Compiler!" 
#endif

/* Check for GNUC and set if there */
#if defined(__GNUC__)
#  if defined(__GNUC_PATCHLEVEL__)
#    define CX_GNUC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#  else
#    define CX_GNUC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#  endif
#elif defined(CX_GNUC_COMPILER)
#  define CX_GNUC 10000
#endif
#undef CX_GNUC_COMPILER



/* Check for the platform / OS */
/* Check for XBox One */
#if defined(_XBOX_ONE) || defined(XBOXONE)
#  define CX_XBOXONE
#  define CX_X64
/* Check for PS3 */
#elif defined(__CELLOS_LV2__) || defined(__PPU__) || defined(__SPU__)
#  define CX_PS3
#  define CX_VMX
#  if defined(__PPU__)
#    define CX_POWERPC
#    define CX_POWERPC64
#  endif
/* Check for PS4 */
#elif defined(__ORBIS__) /* PS4's Orbis OS */
#  define CX_PS4
#  define CX_X64
/* Check for PS Vita */
#elif defined(__psp2__)
#  define CX_PSVITA
#  define CX_ARM
#  define CX_NEON /* Assume uses NEON SIMD instructions */
/* Check for WII U */
#elif defined(CX_GHS)
#  define CX_WIIU
#  define CX_POWERPC
/* Check for visual C/C++ microsoft platforms */
#elif defined(CX_MVC)  
#  if defined(_M_PPC) || defined(_XENON) /* Check for Xbox 360 (power PC architecture) */
#    define CX_XBOX360
#    define CX_POWERPC
#    define CX_POWERPC64
#    define CX_VMX   /* XBox 360 implements IBM's enhanced VMX instruction set. */
#    define CX_VMX128
#  elif defined(_M_ARM)  /* Check for microsoft mobile devices */
#    define CX_ARM
#    define CX_NEON      /* Assume the platform has the NEON SIMD instructions */
#  elif defined(_M_X64) || defined(_M_AMD64) /* Check for 64 bit windows OS */
#    define CX_X64
#    define CX_WINDOWS
#  elif defined(_M_I86) || defined(_M_IX86) /* check for x86 windows OS */
#    define CX_X86
#    define CX_WINDOWS
#  endif
/* Check for generic windows */
#elif defined(_WIN64)
#  define CX_X64
#  define CX_WINDOWS
#elif defined(_WIN32) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__)
#  define CX_X86
#  define CX_WINDOWS
/* Check for generic ARM */
#elif defined(__arm) || defined(__arm__) || defined(_ARM) || defined(__aarch64__)
#  define CX_ARM
#  if defined(CX_SNC) && !defined(CX_PSVITA) /* SN Systems compiler + ARM = playstaion Vita */
#    define CX_PSVITA
#  endif
#  if defined(__ARM_NEON__) /* Use the NEON SIMD instructions */
#    define CX_NEON
#  endif
/* Check for generic PowerPC */
#elif defined(__powerpc64__) || defined(__ppc64__) || defined(_ARCH_PPC64)
#  define CX_POWERPC
#  define CX_POWERPC64
#elif defined(__powerpc) || defined(__powerpc__) || defined(__POWERPC__) || defined(__ppc) || defined(__ppc__) || defined(_ARCH_PPC) || defined(__PPCGECKO__) || defined(__PPCBROADWAY__) || defined(_XENON)
#  define CX_POWERPC
/* Check for generic 64 bit platform */
#elif defined(__amd64) || defined(__amd64__) || defined(__x86_64) || defined(__x86_64__)
#  define CX_X64
/* Check for generic 32 bit platform */
#elif defined(__i386) || defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(_X86_)
#  define CX_X86
#endif

/* Check for 'modern' windows (Metro styled / windows store apps) */
#if defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_PARTITION_APP)
#  define CX_WINDOWS
#  define CX_WINMODERN_API
#endif

/* Check for SPU and PPU for PS3 */
#if defined(__SPU__)
#  define CX_PS3_SPU
#endif

#if defined(__PPU__) || (defined(CX_PS3) && !defined CX_PS3_SPU)
#  define CX_PS3_PPU
#endif

/* Check for 64 bit ARM */
#if defined(CX_ARM)
#  if defined(__aarch64__)
#    define CX_ARM64
#  elif defined(__sizeof_ptr) && __sizeof_ptr == 8
#    define CX_ARM64
#  endif
#endif

/* Check for platforms */
/* Check for android */
#if defined(ANDROID) || defined(__ANDROID__)
#  define PX_UNIX
#  define PX_ANDROID
/* Check for Apple (OSX and iOS) */
#elif defined(__APPLE__) || defined(__MACH__)
#  define PX_UNIX
#  define PX_APPLE
#  if defined(CX_ARM) /* Apple + ARM = iOS device */
#    define PX_IOS
#  else
#    define PX_OSX
#  endif
/* Check for BSD */
#elif defined(__FreeBSD__) || defined(__NetBSD__) || defined(__OpenBSD__) || defined(__bsdi__) || defined(__DragonFly__) || defined(BSD)
#  define CX_UNIX
#  define CX_BSD
/* Check for a CygWin environment */
#elif defined(__CYGWIN__)
#  define CX_UNIX
#  define CX_LINUX
#  define CX_CYGWIN
/* Check for a linux system */
#elif defined(__gnu_linux__) || defined(__linux__)
#  define CX_UNIX
#  define CX_LINUX
/* If all else fails, assume a unix based system */
#else
#  define CX_UNIX
#endif

/* If don't have an archtiecture, assume 32 bit */
#if !defined(CX_X64) && !defined(CX_X86)
#  define CX_X86
#endif

/* Give more control over the inlining of functions. */
#if defined(CX_WINDOWS) || defined(CX_XBOX360) || defined(CX_XBOXONE)
#  pragma inline_depth(255)
#endif

#if defined(CX_MVC)
#  define CX_FORCE_INLINE __forceinline
#elif defined(PX_GNUC) || defined(PX_GHS)
#  define CX_FORCE_INLINE __attribute__((always_inline))
#else
#  define CX_FORCE_INLINE inline
#endif

#if defined(CX_WINDOWS) || defined(CX_XBOXONE)
#  define CX_NOINLINE __declspec(noinline)
#  if !defined(WIN32_LEAN_AND_MEAN)
#    define WIN32_LEAN_AND_MEAN
#  endif
#elif defined(CX_GNUC) || defined(CX_GHS)
#  define CX_NOINLINE __attribute__((noinline))
#else
#  define CX_NOINLINE
#endif

/* Some simple macros for easy alignment of data structures. */
#if defined(CX_MVC)
#  define CX_ALIGN(alignment,structure) __declspec(align(alignment)) structure
#  define CX_ALIGN_START(aligment) __declspec(align(alignment))
#  define CX_ALIGN_END(alignment)
#elif defined(CX_GNUC) || defined(CX_GHS)
#  define CX_ALIGN(alignment,structure) structure __attribute__((aligned(alignment)))
#  define CX_ALIGN_START(alignment)
#  define CX_ALIGN_END(alignment) __attribute__((aligned(alignment)))
#else
#  define CX_ALIGN(alignment,structure)
#  define CX_ALIGN_START(alignment)
#  define CX_ALIGN_END(alignment)
#endif

/* Restrict macro.  All current compilers support, but incase add one that doesn't. */
#define CX_RESTRICT __restrict

/* Tells Windows that a function doesn't modify anything except through the pointer params. */
#if defined(CX_WINDOWS) || defined(CX_XBOX360) || defined(CX_XBOXONE)
#  define CX_NOALIAS __declspec(noalias)
#else
#  define CX_NOALIAS
#endif

/* Define a static (compile time) assertion function using invalid datatypes */
#define CX_STATIC_ASSERT(condition,msg) typedef char msg[(condition) ? 1 : -1]

/* Definintion for a macro to supress unused variable warnings. */
#define CX_UNUSED(unused_var_name) (void)(unused_var_name)

/* Protected against older compilers not having __func__ macro */
#ifndef __func__
#  if defined __FUNCTION__
#    define __func__ __FUNCTION__
#  else
#    define __func__ "Undefined"
#  endif
#endif

/* Define macro's to 'know' if platform is 64 bit or not. */
#if defined(CX_X86) || (defined(CX_POWERPC) && !defined(CX_POWERPC64)) || (defined(CX_ARM) && !defined(CX_ARM64))
#  define CX_32BIT
#else
#  define CX_64BIT
#endif

/* Define macros to use to determine platform specs during runtime. */

#if defined(CX_32BIT)
#  define CX_IS_32BIT 1
#else
#  define CX_IS_32BIT 0
#endif

#if defined(CX_64BIT)
#  define CX_IS_64BIT 1
#else
#  define CX_IS_64BIT 0
#endif

#if defined(CX_PSVITA)
#  define CX_IS_PSVITA 1
#else
#  define CX_IS_PSVITA 0
#endif

#if defined(CX_PS3)
#  define CX_IS_PS3 1
#else
#  define CX_IS_PS3 0
#endif

#if defined(CX_PS4)
#  define CX_IS_PS4 1
#else
#  define CX_IS_PS4 0
#endif

#if defined(CX_XBOX360)
#  define CX_IS_XBOX360 1
#else
#  define CX_IS_XBOX360 0
#endif

#if defined CX_XBOXONE
#  define CX_IS_XBOXONE 1
#else
#  define CX_IS_XBOXONE 0
#endif

#if defined(CX_WIIU)
#  define CX_IS_WIIU 1
#else
#  define CX_IS_WIIU 0
#endif

#if defined(CX_PS3_PPU)
#  define CX_IS_PS3_PPU 1
#else
#  define CX_IS_PS3_PPU 0
#endif

#if defined(CX_PS3_SPU)
#  define CX_IS_PS3_SPU 1
#else
#  define CX_IS_PS3_SPU 0
#endif

#if defined(CX_WINDOWS)
#  define CX_IS_WINDOWS 1
#else
#  define CX_IS_WINDOWS 0
#endif

#if defined(CX_X64)
#  define CX_IS_X64 1
#else
#  define CX_IS_X64 0
#endif

#if defined(CX_X86)
#  define CX_IS_X86 1
#else
#  define CX_IS_X86 0
#endif

#endif // CX_CORE_CXPREPROC_H
