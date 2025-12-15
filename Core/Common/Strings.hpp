/*
 *	Name: Strings
 *	Author: Pawel Mrochen
 */

#pragma once
#ifndef CORE_STRINGS_HPP
#define CORE_STRINGS_HPP

#ifndef NAME_WIDE
#define NAME_WIDE 1
#endif

#ifndef PATH_WIDE
#define PATH_WIDE 1
#endif

#ifndef TEXT_WIDE
#define TEXT_WIDE 1
#endif

#include <string>

namespace core {
namespace strings {

#if NAME_WIDE
using NameString = std::wstring;
using NameChar = wchar_t;
#define	NAME(q) L##q
#else
using NameString = std::string;
using NameChar = char;
#define	NAME(q) q
#endif

#if PATH_WIDE
using PathString = std::wstring;
using PathChar = wchar_t;
#define	PATH(q) L##q
#else
using PathString = std::string;
using PathChar = char;
#define	PATH(q) q
#endif

#if TEXT_WIDE
using Text = std::wstring;
#define	TEXT(q) L##q
#else
using Text = std::string;
#define	TEXT(q) q
#endif

} // namespace strings
} // namespace core

#endif /* CORE_STRINGS_HPP */
