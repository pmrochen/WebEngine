/*
 *	Name: NameString
 *	Author: Pawel Mrochen
 */

#pragma once

#ifndef NAME_WIDE
#define NAME_WIDE 0
#endif

#include <string>

namespace common {

#if NAME_WIDE
using NameString = std::wstring;
using NameChar = wchar_t;
#define	NAME_CSTR(q) L##q
#define	NAME_STRING(q) L##q##s
#else
using NameString = std::string;
using NameChar = char;
#define	NAME_CSTR(q) q
#define	NAME_STRING(q) q##s
#endif

} // namespace core
