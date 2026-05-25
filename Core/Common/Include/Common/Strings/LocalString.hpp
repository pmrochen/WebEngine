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
#include <string_view>

namespace common {

//using namespace std::literals::string_literals;
//using namespace std::literals::string_view_literals;

#if LOCAL_STRING_WIDE

using LocalChar = wchar_t;
using LocalString = std::wstring;
using LocalStringView = std::wstring_view;

#define	LOCAL_CHAR(q) L##q
#define	LOCAL_CSTR(q) L##q
#define	LOCAL_STRING(q) L##q##s
#define	LOCAL_STRING_VIEW(q) L##q##sv

#elif LOCAL_STRING_UTF8

using LocalChar = char8_t;
using LocalString = std::u8string;
using LocalStringView = std::u8string_view;

#define	LOCAL_CHAR(q) u8##q
#define	LOCAL_CSTR(q) u8##q
#define	LOCAL_STRING(q) u8##q##s
#define	LOCAL_STRING_VIEW(q) u8##q##sv

#endif /* LOCAL_STRING_UTF8 */

} // namespace common
