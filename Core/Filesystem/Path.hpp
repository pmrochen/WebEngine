/*
 *	Name: Path
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <concepts>
#include <string>
#include <filesystem>
#include <PathString.hpp>

namespace filesystem {

//template<typename T>
//concept AnyCharacter = (std::same_as<T, char> || std::same_as<T, wchar_t> || std::same_as<T, char8_t> || std::same_as<T, char16_t> || std::same_as<T, char32_t>);

//template<typename T>
//concept AnyString = (std::same_as<T, std::string> || std::same_as<T, std::wstring> || std::same_as<T, std::u8string> || std::same_as<T, std::u16string> || std::same_as<T, std::u32string>);

using common::PathString;

namespace detail {

template<typename T> const T* empty() noexcept;
template<> const char* empty() noexcept { return ""; }
template<> const wchar_t* empty() noexcept { return L""; }
template<> const char8_t* empty() noexcept { return u8""; }
template<> const char16_t* empty() noexcept { return u""; }
template<> const char32_t* empty() noexcept { return U""; }

template<typename T>
inline std::filesystem::path makePath(const T& source)
{
	if constexpr (std::is_pointer_v<T>)
		return std::filesystem::path(source ? source : empty<std::remove_pointer_t<T>>());
	else
		return std::filesystem::path(source);
}

inline PathString toPathString(const std::filesystem::path& path)
{
#if PATH_WIDE
	return path.wstring();
#else
	return path.string();
#endif
}

inline PathString toGenericPathString(const std::filesystem::path& path)
{
#if PATH_WIDE
	return path.generic_wstring();
#else
	return path.generic_string();
#endif
}

} // namespace detail

struct Path
{
	Path() = delete;

	static PathString normalize(const std::filesystem::path& path)
	{
		return toPathString(path.lexically_normal());
	}

	template<typename T> 
	static PathString normalize(const T& path)
	{
		return toPathString(detail::makePath(path).lexically_normal());
	}

	static PathString combine(const std::filesystem::path& path1, const std::filesystem::path& path2)
	{
		return toPathString(path1/path2);
	}

	template<typename T> 
	static PathString combine(const std::filesystem::path& path1, const T& path2)
	{
		return toPathString(path1/detail::makePath(path2));
	}

	template<typename T> 
	static PathString combine(const T& path1, const std::filesystem::path& path2)
	{
		return toPathString(detail::makePath(path1)/path2);
	}

	template<typename T> 
	static PathString combine(const T& path1, const T& path2)
	{
		return toPathString(detail::makePath(path1)/detail::makePath(path2));
	}

	static bool Path::isAbsolute(const std::filesystem::path& path)
	{
		return path.is_absolute();
	}

	template<typename T> 
	static bool Path::isAbsolute(const T& path)
	{
		return detail::makePath(path).is_absolute();
	}

	static PathString getPathRoot(const std::filesystem::path& path)
	{
		return detail::toPathString(path.root_path());
	}

	template<typename T> 
	static PathString getPathRoot(const T& path)
	{
		return detail::toPathString(detail::makePath(path).root_path());
	}

	static PathString getParentPath(const std::filesystem::path& path)
	{
		return detail::toPathString(path.parent_path());
	}

	template<typename T> 
	static PathString getParentPath(const T& path)
	{
		return detail::toPathString(detail::makePath(path).parent_path());
	}

	static PathString getDirectoryName(const std::filesystem::path& path)
	{ 
		return getParentPath(path);
	}

	template<typename T> 
	static PathString getDirectoryName(const T& path)
	{ 
		return getParentPath(path);
	}

	static PathString getFileName(const std::filesystem::path& path)
	{
		return detail::toPathString(path.filename());
	}

	template<typename T> 
	static PathString getFileName(const T& path)
	{
		return detail::toPathString(detail::makePath(path).filename());
	}

	static PathString getFileNameWithoutExtension(const std::filesystem::path& path)
	{
		return detail::toPathString(path.stem());
	}

	template<typename T> 
	static PathString getFileNameWithoutExtension(const T& path)
	{
		return detail::toPathString(detail::makePath(path).stem());
	}

	static bool Path::hasExtension(const std::filesystem::path& path)
	{
		return path.has_extension();
	}

	template<typename T> 
	static bool Path::hasExtension(const T& path)
	{
		return detail::makePath(path).has_extension();
	}

	static PathString getExtension(const std::filesystem::path& path)
	{
		return detail::toPathString(path.extension());
	}

	template<typename T> 
	static PathString getExtension(const T& path)
	{
		return detail::toPathString(detail::makePath(path).extension());
	}

	template<typename T> 
	static PathString changeExtension(const std::filesystem::path& path, const T& ext)
	{
		if constexpr (std::is_pointer_v<T>)
			return detail::toPathString(path.replace_extension(ext ? ext : detail::empty<std::remove_pointer_t<T>>()));
		else
			return detail::toPathString(path.replace_extension(ext));
	}

	template<typename T> 
	static PathString changeExtension(const T& path, const T& ext)
	{
		if constexpr (std::is_pointer_v<T>)
			return detail::toPathString(detail::makePath(path).replace_extension(ext ? ext : detail::empty<std::remove_pointer_t<T>>()));
		else
			return detail::toPathString(detail::makePath(path).replace_extension(ext));
	}

	static PathString getUniversalPath(const std::filesystem::path& path)
	{
		return detail::toGenericPathString(path);
	}

	template<typename T> 
	static PathString getUniversalPath(const T& path)
	{
		return detail::toGenericPathString(detail::makePath(path));
	}
};

} // namespace filesystem
