//
// file:			internal_header.h
// path:			include/jsonpacker/internal_header.h
// created on:		2022 May 11
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef INCLUDE_JSONPACKER_INTERNAL_HEADER_H
#define INCLUDE_JSONPACKER_INTERNAL_HEADER_H

#include <stddef.h>

#define jsonpacker_alloca	alloca

#ifdef _MSC_VER

	#undef jsonpacker_alloca
	#define jsonpacker_alloca	_alloca
	#define JSONPACKER_UNREACHABLE_CODE(_code)
	//#if _MSC_FULL_VER
	#if (_MSC_VER>1900) // 1900 is VS2015
		#pragma warning (disable:5045)
	#endif
	// assignment within conditional expression (https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4706?view=msvc-160)
	#pragma warning (disable:4706) 
	// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4711?view=msvc-160
	#pragma warning (disable:4711)
	// Function not inlined althou defined to be inline. 
	// this can happen also with library functions, so this should be disabled
	// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4711?view=msvc-160
	#pragma warning (disable:4710)
	#define JSONPACKER_BEFORE_CPP_17_FALL_THR
    #define JSONPACKER_DLL_PUBLIC		__declspec(dllexport)
    #define JSONPACKER_DLL_PRIVATE
    #define JSONPACKER_IMPORT_FROM_DLL	__declspec(dllimport)
	#define JSONPACKER_THREAD_LOCAL		__declspec(thread)
	#if !defined(_WIN64) && !defined(_M_ARM64)
		#define CPPUTLS_32_BIT
	#endif
#elif defined(__GNUC__) || defined(__clang__) || defined(LINUX_GCC)
    #define JSONPACKER_MAY_ALIAS  __attribute__ ((__may_alias__))
	#define JSONPACKER_UNREACHABLE_CODE(_code)	_code ;
	#define JSONPACKER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
    //#define JSONPACKER_DLL_PUBLIC		__attribute__((visibility("default")))
    #define JSONPACKER_DLL_PUBLIC
    #define JSONPACKER_DLL_PRIVATE		__attribute__((visibility("hidden")))
    #define JSONPACKER_IMPORT_FROM_DLL
	#define JSONPACKER_THREAD_LOCAL		__thread
#elif defined(__CYGWIN__)
	#define JSONPACKER_UNREACHABLE_CODE(_code)	_code ;
	#define JSONPACKER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
    #define JSONPACKER_DLL_PUBLIC		__attribute__((dllexport))
    #define JSONPACKER_DLL_PRIVATE
    #define JSONPACKER_IMPORT_FROM_DLL	__attribute__((dllimport))
#elif defined(__MINGW64__) || defined(__MINGW32__)
	#define JSONPACKER_UNREACHABLE_CODE(_code)	_code ;
	#define JSONPACKER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ;
    #define JSONPACKER_DLL_PUBLIC		_declspec(dllexport)
    #define JSONPACKER_DLL_PRIVATE
    #define JSONPACKER_IMPORT_FROM_DLL	_declspec(dllimport)
#elif defined(__SUNPRO_CC)
	#define JSONPACKER_UNREACHABLE_CODE(_code)	_code ;
	// #define JSONPACKER_BEFORE_CPP_17_FALL_THR	__attribute__ ((fallthrough)) ; // ???
	#define JSONPACKER_BEFORE_CPP_17_FALL_THR
    #define JSONPACKER_DLL_PUBLIC
    #define JSONPACKER_DLL_PRIVATE		__hidden
    #define JSONPACKER_IMPORT_FROM_DLL
#endif  // #ifdef _MSC_VER

#if defined(_MSC_VER) && defined(_MSVC_LANG)
	#if (_MSVC_LANG>=201100L)
		#define JSONPACKER_CPP_11_DEFINED		1
	#endif
	#if (_MSVC_LANG>=201400L)
		#define JSONPACKER_CPP_14_DEFINED		1
	#endif
	#if (_MSVC_LANG>=201700L)
		#define JSONPACKER_CPP_17_DEFINED		1
	#endif
	#if (_MSVC_LANG>=202000L)
		#define JSONPACKER_CPP_20_DEFINED		1
	#endif
	#if (_MSVC_LANG>=202300L)
		#define JSONPACKER_CPP_23_DEFINED		1
	#endif
#elif defined(__cplusplus)  // #if defined(_MSC_VER) && defined(_MSVC_LANG)
	#if (__cplusplus>=201100L)
		#define JSONPACKER_CPP_11_DEFINED		1
	#endif
	#if (__cplusplus>=201400L)
		#define JSONPACKER_CPP_14_DEFINED		1
	#endif
	#if (__cplusplus>=201700L)
		#define JSONPACKER_CPP_17_DEFINED		1
	#endif
	#if (__cplusplus>=202000L)
		#define JSONPACKER_CPP_20_DEFINED		1
	#endif
	#if (__cplusplus>=202300L)
		#define JSONPACKER_CPP_23_DEFINED		1
	#endif
#endif // #if defined(_MSC_VER) && defined(_MSVC_LANG)


//#undef JSONPACKER_CPP_11_DEFINED


//#define JSONPACKER_RESTRICT   restrict
#define JSONPACKER_RESTRICT

#ifndef JSONPACKER_MAY_ALIAS
#define JSONPACKER_MAY_ALIAS
#endif

#if defined(JSONPACKER_COMPILING_SHARED_LIB)
    #define JSONPACKER_EXPORT JSONPACKER_DLL_PUBLIC
#elif defined(JSONPACKER_USING_STATIC_LIB_OR_OBJECTS)
    #define JSONPACKER_EXPORT
#elif defined(JSONPACKER_LOAD_FROM_DLL)
    #define JSONPACKER_EXPORT JSONPACKER_IMPORT_FROM_DLL
#else
	#define JSONPACKER_EXPORT JSONPACKER_DLL_PRIVATE
#endif

#ifdef __cplusplus
#define JSONPACKER_STATIC_CAST(_type,_data)	static_cast<_type>(_data)
#define JSONPACKER_GLOBAL	   ::
#define JSONPACKER_BEGIN_C   extern "C" {
#define JSONPACKER_END_C     }
#define JSONPACKER_EXTERN_C  extern "C"
#else
#define JSONPACKER_STATIC_CAST(_type,_data)	((_type)(_data))
#define JSONPACKER_GLOBAL
#define JSONPACKER_BEGIN_C
#define JSONPACKER_END_C
#define JSONPACKER_EXTERN_C
#endif


#ifdef JSONPACKER_CPP_17_DEFINED
#define JSONPACKER_FALLTHROUGH	[[fallthrough]] ;
#else
#define JSONPACKER_FALLTHROUGH	JSONPACKER_BEFORE_CPP_17_FALL_THR
//#define JSONPACKER_FALLTHROUGH
#endif

//#undef JSONPACKER_CPP_11_DEFINED
//#undef JSONPACKER_CPP_14_DEFINED

#ifdef JSONPACKER_CPP_11_DEFINED
#define JSONPACKER_STD_MOVE_DEFINED   1
#define JSONPACKER_NOEXCEPT           noexcept
#define JSONPACKER_NULL               nullptr
#define JSONPACKER_OVERRIDE           override
//#define JSONPACKER_CONSTEXPR          constexpr
//#define JSONPACKER_CONSTEXPR_CONS     constexpr
#define JSONPACKER_CONSTEXPR          constexpr
#define JSONPACKER_CONSTEXPR_FNC          constexpr
#define JSONPACKER_CONSTEXPR_EQ(_exp)	= (_exp)
#define JSONPACKER_DELETE             =delete;
#define JSONPACKER_CONSTEXPR_STACK_ARRAY(_type,_name,_count)	_type _name[_count]
#else
#define JSONPACKER_NOEXCEPT	throw()
#define JSONPACKER_NULL		NULL
#define JSONPACKER_OVERRIDE
#define JSONPACKER_CONSTEXPR	const
#define JSONPACKER_CONSTEXPR_FNC
#define JSONPACKER_CONSTEXPR_EQ(_exp)
#define JSONPACKER_DELETE   {}
#define JSONPACKER_CONSTEXPR_STACK_ARRAY(_type,_name,_count)	_type * _name = JSONPACKER_STATIC_CAST(  _type *,jsonpacker_alloca((_count)*sizeof(_type))  )
#endif

#ifdef JSONPACKER_CPP_14_DEFINED
#define JSONPACKER_CONSTEXPR_FNC_CPP14     constexpr
#else
#define JSONPACKER_CONSTEXPR_FNC_CPP14
#endif

#ifdef __cplusplus
#define JSONPACKER_REGISTER
#else
#define JSONPACKER_REGISTER	register
#endif


//#define JSONPACKER_IS_LITTLE_ENDIAN (((union { unsigned x; unsigned char c; }){1}).c)

#if defined(_DEBUG) || defined(JSONPACKER_DEBUG)
// we have debug compilation
#else
// we have release
#ifndef NDEBUG
// let's define NDEBUG (No DEBUG)
#define NDEBUG
#endif
#endif

#ifdef NDEBUG
#define JSONPACKER_DO_DEBUG_EXP(_exp)
#define JSONPACKER_SAFE_CAST(_type,_val)	static_cast<_type>(_val)
#else
#define JSONPACKER_DO_DEBUG_EXP(_exp)              _exp ;
#define JSONPACKER_SAFE_CAST(_type,_val)	dynamic_cast<_type>(_val)
#endif


#define JSONPACKER_NO_NULL

#define JSONPACKER_STRINGIFY(_x)                JSONPACKER_STRINGIFY_PRIV_RAW(_x)
#define JSONPACKER_STRINGIFY_PRIV_RAW(_x)		#_x


#if defined(__EMSCRIPTEN__) || defined(EMSCRIPTEN) || defined(JSONPACKER_WASM)
#define JSONPACKER_EMSCRIPTEN_IS_USED
#endif
//#define JSONPACKER_EMSCRIPTEN_IS_USED

#ifdef JSONPACKER_EMSCRIPTEN_IS_USED
#ifdef JSONPACKER_DLL_PUBLIC
#undef JSONPACKER_DLL_PUBLIC
#endif
#include <emscripten/emscripten.h>
#define JSONPACKER_DLL_PUBLIC     EMSCRIPTEN_KEEPALIVE
#endif

// todo: make better multithreading decision
#if !defined(JSONPACKER_EMSCRIPTEN_IS_USED) && !defined(JSONPACKER_FORCE_SINGLE_THREADED)
#define JSONPACKER_MULTITHREADED
#endif

// todo: make better possible no file dfecision
#ifdef JSONPACKER_EMSCRIPTEN_IS_USED
#define JSONPACKER_POSSIBLE_NO_PERS_FILE
#endif

#ifdef JSONPACKER_MULTITHREADED
#define JSONPACKER_TRY_CATCH(_expression)	(_expression) ;
#else
#define JSONPACKER_TRY_CATCH(_expression)   try{(_expression);}catch(...){return;}
#endif

#define JSONPACKER_IN_OUT

// https://gcc.gnu.org/bugzilla/show_bug.cgi?id=56480
#ifdef __GNUC__
#define JSONPACKER_GCC_BUG_56480
#endif

#ifdef JSONPACKER_CPP_20_DEFINED
#define JSONPACKER_NODISCARD	[[nodiscard]]
#elif defined(JSONPACKER_CPP_11_DEFINED)
#define JSONPACKER_NODISCARD	noexcept
#else
#define JSONPACKER_NODISCARD	throw()
#endif


#endif  // #ifndef INCLUDE_JSONPACKER_JSONPACKER_INTERNAL_HEADER_H
