/*
 *	Name: PathString
 *	Author: Pawel Mrochen
 */

#pragma once

#ifndef PATH_STRING_WIDE
#ifdef _WIN32
#define PATH_STRING_WIDE 1
#else
#define PATH_STRING_WIDE 0
#endif
#endif

#include <string>
#include <string_view>

namespace common {

//using namespace std::literals::string_literals;
//using namespace std::literals::string_view_literals;

#if PATH_STRING_WIDE

using PathChar = wchar_t;
using PathString = std::wstring;
using PathStringView = std::wstring_view;

#define	PATH_CHAR(q) L##q
#define	PATH_CSTR(q) L##q
#define	PATH_STRING(q) L##q##s
#define	PATH_STRING_VIEW(q) L##q##sv

#else /* !PATH_STRING_WIDE */

using PathChar = char;
using PathString = std::string;
using PathStringView = std::string_view;

#define	PATH_CHAR(q) q
#define	PATH_CSTR(q) q
#define	PATH_STRING(q) q##s
#define	PATH_STRING_VIEW(q) q##sv

#endif /* PATH_STRING_WIDE */

} // namespace common
