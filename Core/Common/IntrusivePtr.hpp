/*
 *	Name: IntrusivePtr
 *	Author: Pawel Mrochen
 */

#pragma once

#include <functional>
//#include <boost/smart_ptr/intrusive_ptr.hpp>
//#include <boost/serialization/split_free.hpp>
//#include <boost/serialization/level.hpp>
//#include <boost/serialization/tracking.hpp>

//#define BOOST_SERIALIZATION_INTRUSIVE_PTR(T)                                  \
//    BOOST_CLASS_IMPLEMENTATION(::boost::intrusive_ptr<T>, 					  \
//		::boost::serialization::object_serializable)						  \
//    BOOST_CLASS_TRACKING(::boost::intrusive_ptr<T>,                           \
//        ::boost::serialization::track_never)

//namespace boost::serialization {

//template<typename Archive, typename T>
//inline void save(Archive& ar, boost::intrusive_ptr<T> const& t, unsigned int const /*version*/)
//{
//    T const* ptr = t.get();
//    ar << ptr;
//}

//template<typename Archive, typename T>
//inline void load(Archive& ar, boost::intrusive_ptr<T>& t, unsigned int const /*version*/)
//{
//    T* ptr;
//    ar >> ptr;
//    t.reset(ptr);
//}

//template<typename Archive, typename T>
//inline void serialize(Archive& ar, boost::intrusive_ptr<T>& t, unsigned int const version)
//{
//    static_assert(tracking_level<T>::value != track_never);
//    split_free(ar, t, version);
//}

//} // namespace boost::serialization

namespace core {

//using IntrusivePtr = ::boost::intrusive_ptr;

template<typename T> 
class IntrusivePtr
{
    template<class U> 
    friend class IntrusivePtr<U>;

public:
    IntrusivePtr() noexcept : p_()
    {
    }

    IntrusivePtr(T* p, bool addRef = true) : p_(p)
    {
        if ((p != nullptr) && addRef)
            p->addReference();
    }

    IntrusivePtr(const IntrusivePtr& p) : p_(p.p_)
    {
        if (p_ != nullptr)
            p_->addReference();
    }

    template<class U>
    IntrusivePtr(const IntrusivePtr<U>& p/*, typename boost::detail::sp_enable_if_convertible<U,T>::type = boost::detail::sp_empty()*/) : p_(p.p_)
    {
        if (p_ != nullptr)
            p_->addReference();
    }

    IntrusivePtr(IntrusivePtr&& p) noexcept : p_(p.p_)
    {
        p.p_ = nullptr;
    }

    template<class U>
    IntrusivePtr(IntrusivePtr<U>&& p/*, typename boost::detail::sp_enable_if_convertible<U,T>::type = boost::detail::sp_empty()*/) : p_(p.p_)
    {
        p.p_ = nullptr;
    }

    ~IntrusivePtr()
    {
        if (p_ != nullptr)
            p_->release();
    }

    IntrusivePtr& operator=(const IntrusivePtr& p)
    {
        IntrusivePtr(p).swap(*this);
        return *this;
    }

    template<class U>
    IntrusivePtr& operator=(const IntrusivePtr<U>& p)
    {
        IntrusivePtr(p).swap(*this);
        return *this;
    }

    IntrusivePtr& operator=(IntrusivePtr&& p) noexcept
    {
        IntrusivePtr(static_cast<IntrusivePtr&&>(p)).swap(*this);
        return *this;
    }

    template<class U>
    IntrusivePtr& operator=(IntrusivePtr<U>&& p) noexcept
    {
        IntrusivePtr(static_cast<IntrusivePtr<U>&&>(p)).swap(*this);
        return *this;
    }

    IntrusivePtr& operator=(T* p)
    {
        IntrusivePtr(p).swap(*this);
        return *this;
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
        IntrusivePtr().swap(*this);
    }

    void reset(T* p)
    {
        IntrusivePtr(p).swap(*this);
    }

    void reset(T* p, bool addRef)
    {
        IntrusivePtr(p, addRef).swap(*this);
    }

    T* detach()
    {
        T* result = p_;
        p_ = nullptr;
        return result;
    }

    void swap(IntrusivePtr& p) noexcept
    {
        T* tmp = p_;
        p_ = p.p_;
        p.p_ = tmp;
    }

private:
    T* p_;
};

template<typename T, typename U> 
inline bool operator==(const IntrusivePtr<T>& a, const IntrusivePtr<U>& b) noexcept
{
    return (a.get() == b.get());
}

template<typename T, typename U> 
inline bool operator!=(const IntrusivePtr<T>& a, const IntrusivePtr<U>& b) noexcept
{
    return (a.get() != b.get());
}

template<typename T, typename U> 
inline bool operator==(const IntrusivePtr<T>& a, U* b) noexcept
{
    return (a.get() == b);
}

template<typename T, typename U> 
inline bool operator!=(const IntrusivePtr<T>& a, U* b) noexcept
{
    return (a.get() != b);
}

template<typename T, typename U> 
inline bool operator==(T* a, const IntrusivePtr<U& b) noexcept
{
    return (a == b.get());
}

template<typename T, typename U> 
inline bool operator!=(T* a, const IntrusivePtr<U>& b) noexcept
{
    return (a != b.get());
}

template<typename T> 
inline bool operator==(const IntrusivePtr<T>& p, std::nullptr_t) noexcept
{
    return (p.get() == nullptr);
}

template<typename T> 
inline bool operator!=(const IntrusivePtr<T>& p, std::nullptr_t) noexcept
{
    return (p.get() != nullptr);
}

template<typename T> 
inline bool operator==(std::nullptr_t, const IntrusivePtr<T>& p) noexcept
{
    return (p.get() == nullptr);
}

template<typename T> 
inline bool operator!=(std::nullptr_t, const IntrusivePtr<T>& p) noexcept
{
    return (p.get() != nullptr);
}

template<typename T> 
inline bool operator<(const IntrusivePtr<T>& a, const IntrusivePtr<T>& b) noexcept
{
    return std::less<T*>()(a.get(), b.get());
}

template<typename T> 
inline void swap(IntrusivePtr<T>& a, IntrusivePtr<T>& b) noexcept
{
    a.swap(b);
}

template<typename T> 
inline T* get_pointer(const IntrusivePtr<T>& p) noexcept
{
    return p.get();
}

} // namespace core

namespace std {

template<typename T> 
struct hash;

template<typename T> 
struct hash<::core::IntrusivePtr<T>>
{
    std::size_t operator()(const ::core::IntrusivePtr<T>& p) const noexcept
    {
        return std::hash<T*>()(p.get());
    }
};

} // namespace std
