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

namespace common {

//using namespace std::literals::string_literals;

#if PATH_STRING_WIDE

using PathChar = wchar_t;
using PathString = std::wstring;

#define	PATH_CHAR(q) L##q
#define	PATH_CSTR(q) L##q
#define	PATH_STRING(q) L##q##s

#else /* !PATH_STRING_WIDE */

using PathChar = char;
using PathString = std::string;

#define	PATH_CHAR(q) q
#define	PATH_CSTR(q) q
#define	PATH_STRING(q) q##s

#endif /* PATH_STRING_WIDE */

} // namespace common
