/*
 *	Name: NameString
 *	Author: Pawel Mrochen
 */

#pragma once

#ifndef NAME_STRING_WIDE
#define NAME_STRING_WIDE 0
#endif

#include <string>

namespace common {

//using namespace std::literals::string_literals;

#if NAME_STRING_WIDE

using NameChar = wchar_t;
using NameString = std::wstring;

#define	NAME_CHAR(q) L##q
#define	NAME_CSTR(q) L##q
#define	NAME_STRING(q) L##q##s

#else /* ! NAME_STRING_WIDE */

using NameChar = char;
using NameString = std::string;

#define	NAME_CHAR(q) q
#define	NAME_CSTR(q) q
#define	NAME_STRING(q) q##s

#endif /* NAME_STRING_WIDE */

} // namespace common
