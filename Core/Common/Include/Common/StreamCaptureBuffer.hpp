/*
 *	Name: StreamCaptureBuffer
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <concepts>
#include <streambuf>
#include <iostream>
#include <string>

namespace common {
namespace templates {

template<typename T, typename F>
	requires (std::is_same_v<T, char> || std::is_same_v<T, wchar_t>)
class StreamCaptureBuffer : public std::basic_streambuf<T>
{
	StreamCaptureBuffer(bool captureCout, bool captureCerr, bool captureClog)
	{
		if constexpr (std::is_same_v<T, char>)
		{
			if (captureCout) 
				coutBuffer_ = std::cout.rdbuf(this);
			if (captureCerr) 
				cerrBuffer_ = std::cerr.rdbuf(this);
			if (captureClog) 
				clogBuffer_ = std::clog.rdbuf(this);
		}
		else // wchar_t
		{
			if (captureCout) 
				coutBuffer_ = std::wcout.rdbuf(this);
			if (captureCerr) 
				cerrBuffer_ = std::wcerr.rdbuf(this);
			if (captureClog) 
				clogBuffer_ = std::wclog.rdbuf(this);
		}
	}

	StreamCaptureBuffer(bool captureCout, bool captureCerr, bool captureClog, F callback) :
		StreamCaptureBuffer(captureCout, captureCerr, captureClog),
		callback_(callback)
	{
	}

	virtual ~StreamCaptureBuffer()
	{
		if constexpr (std::is_same_v<T, char>)
		{
			if (clogBuffer_) 
				std::clog.rdbuf(clogBuffer_);
			if (cerrBuffer_) 
				std::cerr.rdbuf(cerrBuffer_);
			if (coutBuffer_) 
				std::cout.rdbuf(coutBuffer_);
		}
		else // wchar_t
		{
			if (clogBuffer_) 
				std::wclog.rdbuf(clogBuffer_);
			if (cerrBuffer_) 
				std::wcerr.rdbuf(cerrBuffer_);
			if (coutBuffer_) 
				std::wcout.rdbuf(coutBuffer_);
		}
	}

	F getCallback() const noexcept { return callback_; }
	void setCallback(F callback) noexcept { callback_ = callback; }

protected:
	virtual int_type overflow(int_type meta = std::streambuf::traits_type::eof()) override
	{
		if (!traits_type::eq_int_type(meta, traits_type::eof()))
		{
			line_ += T(meta);
			if (meta == int_type('\n')) 
				sync();
		}
		
		return traits_type::not_eof(meta);
	}

	virtual int sync() override
	{
		if (callback_)
			callback_(line_);

		line_.resize(0);
		return 0;
	}

private:
	StreamCaptureBuffer(const StreamCaptureBuffer&) = delete;
	StreamCaptureBuffer& operator=(const StreamCaptureBuffer&) = delete;

	std::basic_streambuf<T>* coutBuffer_ = nullptr;
	std::basic_streambuf<T>* cerrBuffer_ = nullptr;
	std::basic_streambuf<T>* clogBuffer_ = nullptr;
	std::basic_string<T> line_;
	F callback_ = {};
};

} // namespace templates

#ifdef _WIN32
template<typename F>
using StreamCaptureBuffer = templates::StreamCaptureBuffer<wchar_t, F>;
#else
template<typename F>
using StreamCaptureBuffer = templates::StreamCaptureBuffer<char, F>;
#endif

} // namespace common
