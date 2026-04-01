/*
 *	Name: OwnedPtr
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
//#include <boost/smart_ptr/scoped_ptr.hpp>

namespace core {

template<typename T> 
class OwnedPtr //: public boost::scoped_ptr<T>
{
public:
    OwnedPtr() noexcept : p_()
    {
    }

    template<typename U>
    OwnedPtr(T* p, U* owner) : p_(p)
    {
        if (p != nullptr)
            p->setOwner(owner);
    }

    OwnedPtr(const OwnedPtr&) = delete;
    OwnedPtr& operator=(const OwnedPtr&) = delete;

    ~OwnedPtr()
    {
        if (p_ != nullptr)
            p_->release();
            
    }

    T& operator*() const noexcept
    {
        return *p_;
    }

    T* operator->() const noexcept
    {
        return p_;
    }

    explicit operator bool() const noexcept
    {
        return (p_ != nullptr);
    }

    T* get() const noexcept
    {
        return p_;
    }

    void reset()
    {
        OwnedPtr().swap(*this);
    }

    template<typename U>
    void reset(T* p, U* owner)
    {
        OwnedPtr(p, owner).swap(*this);
    }

    T* detach()
    {
        if (p_ != nullptr)
            p_->setOwner(nullptr);
        T* result = p_;
        p_ = nullptr;
        return result;
    }

    void swap(OwnedPtr& rhs) noexcept
    {
        T* tmp = p_;
        p_ = rhs.p_;
        rhs.p_ = tmp;
    }

private:
    bool operator==(const OwnedPtr&) const;
    bool operator!=(const OwnedPtr&) const;

    T* p_;
};

template<typename T> 
inline bool operator==(const OwnedPtr<T>& p, std::nullptr_t) noexcept
{
    return (p.get() == nullptr);
}

template<typename T> 
inline bool operator!=(const OwnedPtr<T>& p, std::nullptr_t) noexcept
{
    return (p.get() != nullptr);
}

template<typename T> 
inline bool operator==(std::nullptr_t, const OwnedPtr<T>& p) noexcept
{
    return (p.get() == nullptr);
}

template<typename T> 
inline bool operator!=(std::nullptr_t, const OwnedPtr<T>& p) noexcept
{
    return (p.get() != nullptr);
}

template<typename T> 
inline void swap(OwnedPtr<T>& a, OwnedPtr<T>& b) noexcept
{
    a.swap(b);
}

template<typename T> 
inline T* get_pointer(const OwnedPtr<T>& p) noexcept
{
    return p.get();
}

} // namespace core

namespace std {

template<typename T> 
struct hash;

template<typename T> 
struct hash<::core::OwnedPtr<T>>
{
    size_t operator()(const ::core::OwnedPtr<T>& p) const noexcept
    {
        return hash<T*>()(p.get());
    }
};

} // namespace std
