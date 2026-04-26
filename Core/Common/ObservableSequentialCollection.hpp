/*
 *	Name: ObservableSequentialCollection
 *	Author: Pawel Mrochen
 */

#pragma once

#include <initializer_list>
#include <sigslot/signal.hpp>

namespace common {

template<typename Container>
class ObservableSequentialCollection
{
public:
    using ThisType = ObservableSequentialCollection;
    using ContainerType = Container;
    using ValueType = typename Container::value_type;
    using AllocatorType = typename Container::allocator_type;
    using ConstReference = typename Container::const_reference;
    using SizeType = typename Container::size_type;
    using Iterator = typename Container::iterator;
    using ConstIterator = typename Container::const_iterator;
    using Signal = sigslot::signal<typename Container::value_type&>;

    ObservableSequentialCollection() noexcept(noexcept(AllocatorType())) : v_(AllocatorType()) 
    {
    }

    explicit ObservableSequentialCollection(const AllocatorType& alloc) noexcept : v_(alloc) 
    {
    }

    explicit ObservableSequentialCollection(SizeType count, const AllocatorType& alloc = AllocatorType()) : v_(count, alloc)
    {
    }

    ObservableSequentialCollection(SizeType count, const ValueType& value, const AllocatorType& alloc = AllocatorType()) : v_(count, value, alloc)
    {
    }

    template<typename InputIt>
    ObservableSequentialCollection(InputIt first, InputIt last, const AllocatorType& alloc = AllocatorType()) : v_(first, last, alloc)
    {
    }

    ObservableSequentialCollection(const ThisType& other) : v_(other.v_)
    {
    }

    ObservableSequentialCollection(ThisType&& other) noexcept : v_(other.get_allocator())
    {
        other.signalRemoveAll();
        v_ = std::move(other.v_);
    }

    ObservableSequentialCollection(const ThisType& other, const AllocatorType& alloc) : v_(other.v_, alloc)
    {
    }

    ObservableSequentialCollection(ThisType&& other, const AllocatorType& alloc) : v_(alloc)
    {
        other.signalRemoveAll();
        v_ = std::move(other.v_);
    }

    ObservableSequentialCollection(std::initializer_list<T> init, const AllocatorType& alloc = AllocatorType()) : v_(init, alloc)
    {
    }

    ~ObservableSequentialCollection()
    {
        signalRemoveAll();
    }

    ObservableSequentialCollection& operator=(const ObservableSequentialCollection& other)
    {
        signalRemoveAll();
        v_ = other.v_;
        signalAddAll();
        return *this;
    }

    ObservableSequentialCollection& operator=(ObservableSequentialCollection&& other)
    {
        signalRemoveAll();
        v_ = std::move(other.v_);
        signalAddAll();
        return *this;
    }

    ObservableSequentialCollection& operator=(std::initializer_list<ValueType> init)
    {
        signalRemoveAll();
        v_ = init;
        signalAddAll();
        return *this;
    }

    ConstReference operator[](SizeType pos) const
    {
        return v_[pos];
    }

    ConstReference at(SizeType pos) const
    {
        return v_.at(pos);
    }

    ConstReference front() const
    {
        return v_.front();
    }

    ConstReference back() const
    {
        return v_.back();
    }

    const ValueType* data() const
    {
        return v_.data();
    }

    ConstIterator begin() const
    {
        return v_.begin();
    }

    ConstIterator end() const
    {
        return v_.end();
    }

    void assign(SizeType count, const ValueType& value)
    {
        signalRemoveAll();
        v_.assign(count, value);
        signalAddAll();
    }

    template<class InputIt>
    void assign(InputIt first, InputIt last)
    {
        signalRemoveAll();
        v_.assign(first, last);
        signalAddAll();
    }

    void assign(std::initializer_list<ValueType> init)
    {
        signalRemoveAll();
        v_.assign(init);
        signalAddAll();
    }

    void clear()
    {
        signalRemoveAll();
        v_.clear();
    }

    Iterator insert(ConstIterator pos, const ValueType& value)
    {
        Iterator result(v_.insert(pos, value));
        onElementAdded(*result);
        return result;
    }

    Iterator insert(ConstIterator pos, ValueType&& value)
    {
        Iterator result(v_.insert(pos, std::forward<ValueType>(value)));
        onElementAdded(*result);
        return result;
    }

    Iterator insert(ConstIterator pos, SizeType count, const ValueType& value)
    {
        Iterator result(v_.insert(pos, count, value));
        for (Iterator i = result, e = result + count; i != e; ++i)
            onElementAdded(*i);
        return result;
    }
    
    template<class InputIt>
    Iterator insert(ConstIterator pos, InputIt first, InputIt last)
    {
        Iterator result(v_.insert(pos, count, value));
        for (Iterator i = result, e = result + (last - first); i != e; ++i)
            onElementAdded(*i);
        return result;
    }

    Iterator insert(ConstIterator pos, std::initializer_list<ValueType> init)
    {
        Iterator result(v_.insert(pos, count, value));
        for (Iterator i = result, e = result + init.size(); i != e; ++i)
            onElementAdded(*i);
        return result;
    }
    
    template<class... Args>
    Iterator emplace(ConstIterator pos, Args&&... args)
    {
        Iterator result(v_.emplace(pos, std::forward<Args>(args)...));
        onElementAdded(*result);
        return result;
    }

    void push_back(const ValueType& value)
    {
        v_.push_back(value);
        onElementAdded(v_.back());
    }

    void push_back(ValueType&& value)
    {
        v_.push_back(std::forward<ValueType>(value));
        onElementAdded(v_.back());
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        v_.emplace_back(std::forward<Args>(args)...);
        onElementAdded(v_.back());
    }

    Iterator erase(ConstIterator pos)
    {
        if ((pos >= begin()) && (pos < end()))
            onElementRemoving(*pos);
        return v_.erase(pos);
    }

    Iterator erase(ConstIterator first, ConstIterator last)
    {
        if ((last >= begin()) && (first < end()))
        {
            for (ConstIterator i = first; i != last; ++i)
                onElementRemoving(*i);
        }
        return v_.erase(first, last);
    }

    void pop_back()
    {
        onElementRemoving(v_.back());
        v_.pop_back();
    }

    void resize(SizeType count)
    {
        if (count < v_.size())
        {
            for (Iterator i = v_.begin() + count, e = v_.end(); i != e; ++i)
                onElementRemoving(*i);
            v_.resize(count);
        }
        else if (count > v_.size())
        {
            SizeType prevSize = v_.size();
            v_.resize(count);
            for (Iterator i = v_.begin() + prevSize, e = v_.end(); i != e; ++i)
                onElementAdded(*i);
        }
    }

    void resize(SizeType count, const ValueType& value)
    {
        if (count < v_.size())
        {
            for (Iterator i = v_.begin() + count, e = v_.end(); i != e; ++i)
                onElementRemoving(*i);
            v_.resize(count, value);
        }
        else if (count > v_.size())
        {
            SizeType prevSize = v_.size();
            v_.resize(count, value);
            for (Iterator i = v_.begin() + prevSize, e = v_.end(); i != e; ++i)
                onElementAdded(*i);
        }
    }

    void swap(ObservableSequentialCollection& other)
    {
        signalRemoveAll();
        other.signalRemoveAll();
        v_.swap(other.v_);
        signalAddAll();
        other.signalAddAll();
    }

    Signal onElementAdded;
    Signal onElementRemoving;

private:
    void signalAddAll()
    {
        for (Iterator i = v_.begin(), e = v_.end(); i != e; ++i)
            onElementAdded(*i);
    }

    void signalRemoveAll()
    {
        for (typename VectorType::reverse_iterator i = v_.rbegin(), e = v_.rend(); i != e; ++i)
            onElementRemoving(*i);
    }

    Container v_;
};

} // namespace common
