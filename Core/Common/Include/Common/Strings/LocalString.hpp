/*
 *	Name: LocalString
 *	Author: Pawel Mrochen
 */

#pragma once

#if !defined(LOCAL_STRING_WIDE) && !defined(LOCAL_STRING_UTF8)
#ifdef _WIN32
#define LOCAL_STRING_WIDE 1
#define LOCAL_STRING_UTF8 0
#else
#define LOCAL_STRING_WIDE 0
#define LOCAL_STRING_UTF8 1
#endif
#elif !defined(LOCAL_STRING_WIDE)
#define LOCAL_STRING_WIDE (1 - LOCAL_STRING_UTF8)
#elif !defined(LOCAL_STRING_UTF8)
#define LOCAL_STRING_UTF8 (1 - LOCAL_STRING_WIDE)
#endif

#include <string>

namespace common {

//using namespace std::literals::string_literals;

#if LOCAL_STRING_WIDE

using LocalChar = wchar_t;
using LocalString = std::wstring;

#define	LOCAL_CHAR(q) L##q
#define	LOCAL_CSTR(q) L##q
#define	LOCAL_STRING(q) L##q##s

#elif LOCAL_STRING_UTF8

using LocalChar = char8_t;
using LocalString = std::u8string;

#define	LOCAL_CHAR(q) u8##q
#define	LOCAL_CSTR(q) u8##q
#define	LOCAL_STRING(q) u8##q##s

#endif /* LOCAL_STRING_UTF8 */

} // namespace common
