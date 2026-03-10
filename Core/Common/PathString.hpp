/*
 *	Name: PathString
 *	Author: Pawel Mrochen
 */

#pragma once

#ifndef PATH_WIDE
#ifdef _WIN32
#define PATH_WIDE 1
#else
#define PATH_WIDE 0
#endif
#endif

#include <string>

namespace common {

#if PATH_WIDE
using PathString = std::wstring;
using PathChar = wchar_t;
#define	PATH_CSTR(q) L##q
#define	PATH_STRING(q) L##q##s
#else
using PathString = std::string;
using PathChar = char;
#define	PATH_CSTR(q) q
#define	PATH_STRING(q) q##s
#endif

} // namespace core
