//
// file:            disable_utils_warnings.h
// path:			include/qtutils/disable_utils_warnings.h
// created on:		2021 Nov 15
// created by:		Davit Kalantaryan (davit.kalantaryan@gmail.com)
//

#ifndef QTUTILS_INCLUDE_QTUTILS_CORE_DISABLE_UTILS_WARNINGS_H
#define QTUTILS_INCLUDE_QTUTILS_CORE_DISABLE_UTILS_WARNINGS_H

//#include <qtutils/qtutils_internal_header.h>
//#include <cpputils/disable_compiler_warnings.h>

// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warnings-c4800-through-c4999?view=msvc-160 (type convertion)
// https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-1-c4946?view=msvc-160  (reinterpret cast is not proper)
#ifdef _MSC_VER
#pragma warning (disable:5219)
#pragma warning (disable:4946)
// warning: C4866: compiler may not enforce left-to-right evaluation order for call to 'operator<<<enum QNetworkReply::NetworkError>'
#pragma warning (disable:4866)
#pragma warning (disable:5204)
#pragma warning (disable:5045)  // warning C5045: Compiler will insert Spectre mitigation
#pragma warning (disable:4371)
#pragma warning (disable:4242)
#endif


#endif  // #ifndef QTUTILS_INCLUDE_QTUTILS_CORE_DISABLE_UTILS_WARNINGS_H
