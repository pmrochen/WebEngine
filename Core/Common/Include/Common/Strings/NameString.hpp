/*
 *	Name: NameString
 *	Author: Pawel Mrochen
 */

#pragma once

#ifndef NAME_STRING_WIDE
#define NAME_STRING_WIDE 0
#endif

#include <string>
#include <string_view>

namespace common {

//using namespace std::literals::string_literals;
//using namespace std::literals::string_view_literals;

#if NAME_STRING_WIDE

using NameChar = wchar_t;
using NameString = std::wstring;
using NameStringView = std::wstring_view;

#define	NAME_CHAR(q) L##q
#define	NAME_CSTR(q) L##q
#define	NAME_STRING(q) L##q##s
#define	NAME_STRING_VIEW(q) L##q##sv

#else /* ! NAME_STRING_WIDE */

using NameChar = char;
using NameString = std::string;
using NameStringView = std::string_view;

#define	NAME_CHAR(q) q
#define	NAME_CSTR(q) q
#define	NAME_STRING(q) q##s
#define	NAME_STRING_VIEW(q) q##sv

#endif /* NAME_STRING_WIDE */

} // namespace common
