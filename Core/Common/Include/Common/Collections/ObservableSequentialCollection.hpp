/*
 *	Name: ObservableSequentialCollection
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
#include <initializer_list>
#include <sigslot/signal.hpp>
#include "../Concepts.hpp"

namespace common {
namespace detail {

template<typename Signal, typename InputIt>
inline void emit(Signal& signal, InputIt first, InputIt last)
{
	if ((first == last) || !signal.slot_count())
		return;
	for (; first != last; ++first)
		signal(*first);
}

template<typename Signal, std::unsigned_integral Size, typename Value>
inline void emit(Signal& signal, Size count, const Value& value)
{
	if (!count || !signal.slot_count())
		return;
	for (Size i = 0; i != count; i++)
		signal(value);
}

} // namespace detail

template<Container T>
class ObservableSequentialCollection
{
public:
    using ThisType = ObservableSequentialCollection;
    using ContainerType = T;
    using ValueType = typename T::value_type;
    using AllocatorType = typename T::allocator_type;
    using ConstReference = typename T::const_reference;
    using SizeType = typename T::size_type;
    //using Iterator = typename T::iterator;
    using ConstIterator = typename T::const_iterator;
    using Signal = sigslot::signal<typename T::value_type&>;

    ObservableSequentialCollection() noexcept(noexcept(AllocatorType())) : 
		c_(AllocatorType()) 
    {
    }

    explicit ObservableSequentialCollection(const AllocatorType& alloc) noexcept : 
		c_(alloc) 
    {
    }

    explicit ObservableSequentialCollection(SizeType count, const AllocatorType& alloc = AllocatorType()) : 
		c_(count, alloc)
    {
    }

    ObservableSequentialCollection(SizeType count, const ValueType& value, const AllocatorType& alloc = AllocatorType()) : 
		c_(count, value, alloc)
    {
    }

    template<typename InputIt>
    ObservableSequentialCollection(InputIt first, InputIt last, const AllocatorType& alloc = AllocatorType()) : 
		c_(first, last, alloc)
    {
    }

    ObservableSequentialCollection(const ThisType& other) : 
		c_(other.c_)
    {
    }

    ObservableSequentialCollection(ThisType&& other) noexcept : 
		c_(other.c_.get_allocator())
    {
        detail::emit(other.onElementRemoving, other.c_.begin(), other.c_.end());
        c_ = std::move(other.c_);
        detail::emit(other.onElementRemoved, c_.begin(), c_.end());
    }

    ObservableSequentialCollection(const ThisType& other, const AllocatorType& alloc) : 
		c_(other.c_, alloc)
    {
    }

    ObservableSequentialCollection(ThisType&& other, const AllocatorType& alloc) : 
		c_(alloc)
    {
        detail::emit(other.onElementRemoving, other.c_.begin(), other.c_.end());
        c_ = std::move(other.c_);
        detail::emit(other.onElementRemoved, c_.begin(), c_.end());
    }

    ObservableSequentialCollection(const ContainerType& other) : 
		c_(other)
    {
    }

    ObservableSequentialCollection(ContainerType&& other) noexcept : 
		c_(other.get_allocator())
    {
        c_ = std::move(other);
    }

    ObservableSequentialCollection(const ContainerType& other, const AllocatorType& alloc) : 
		c_(other, alloc)
    {
    }

    ObservableSequentialCollection(ContainerType&& other, const AllocatorType& alloc) : 
		c_(alloc)
    {
        c_ = std::move(other);
    }

    ObservableSequentialCollection(std::initializer_list<T> list, const AllocatorType& alloc = AllocatorType()) : 
		c_(list, alloc)
    {
    }

    ~ObservableSequentialCollection()
    {
        detail::emit(onElementRemoving, c_.begin(), c_.end());

		if (!empty() && onElementRemoved.slot_count())
		{
			ContainerType removed(c_.get_allocator());
			c_.swap(removed);
			detail::emit(onElementRemoved, removed.begin(), removed.end());
		}
    }

    ObservableSequentialCollection& operator=(const ObservableSequentialCollection& other)
    {
		if (!empty())
			resize(0);

        detail::emit(onElementAdding, other.c_.begin(), other.c_.end());
        c_ = other.c_;
        detail::emit(onElementAdded, c_.begin(), c_.end());
        return *this;
    }

    ObservableSequentialCollection& operator=(ObservableSequentialCollection&& other)
    {
		if (!empty())
			resize(0);
		
        detail::emit(onElementAdding, other.c_.begin(), other.c_.end());
        detail::emit(other.onElementRemoving, other.c_.begin(), other.c_.end());
        c_ = std::move(other.c_);
        detail::emit(onElementAdded, c_.begin(), c_.end());
        detail::emit(other.onElementRemoved, c_.begin(), c_.end());
        return *this;
    }

    ObservableSequentialCollection& operator=(const ContainerType& other)
    {
		if (!empty())
			resize(0);

        detail::emit(onElementAdding, other.begin(), other.end());
        c_ = other;
        detail::emit(onElementAdded, c_.begin(), c_.end());
        return *this;
    }

    ObservableSequentialCollection& operator=(ContainerType&& other)
    {
		if (!empty())
			resize(0);
		
        detail::emit(onElementAdding, other.begin(), other.end());
        c_ = std::move(other);
        detail::emit(onElementAdded, c_.begin(), c_.end());
        return *this;
    }

    ObservableSequentialCollection& operator=(std::initializer_list<ValueType> list)
    {
		if (!empty())
			resize(0);
		
        detail::emit(onElementAdding, list.begin(), list.end());
        c_ = init;
        detail::emit(onElementAdded, c_.begin(), c_.end());
        return *this;
    }

    ConstReference operator[](SizeType pos) const
    {
        return c_[pos];
    }

    ConstReference at(SizeType pos) const
    {
        return c_.at(pos);
    }

    ConstReference get(SizeType pos) const
    {
        return c_.at(pos);
    }

	void set(SizeType pos, const ValueType& value)
	{
		if (pos < c_.size())
		{
			onElementAdding(value);
			onElementRemoving(c_.at(pos));

			if (onElementRemoved.slot_count())
			{
				ValueType prev = c_.at(pos);
				c_.at(pos) = value;
				onElementRemoved(prev);
				onElementAdded(c_.at(pos));
			}
			else
			{
				c_.at(pos) = value;
				onElementAdded(c_.at(pos));
			}
		}
		else
			c_.at(pos) = value;
	}

	void set(SizeType pos, ValueType&& value)
	{
		if (pos < c_.size())
		{
			onElementAdding(value);
			onElementRemoving(c_.at(pos));

			if (onElementRemoved.slot_count())
			{
				ValueType prev = c_.at(pos);
				c_.at(pos) = std::forward<ValueType>(value);
				onElementRemoved(prev);
				onElementAdded(c_.at(pos));
			}
			else
			{
				c_.at(pos) = std::forward<ValueType>(value);
				onElementAdded(c_.at(pos));
			}
		}
		else
			c_.at(pos) = std::forward<ValueType>(value);
	}

    ConstReference front() const
    {
        return c_.front();
    }

    ConstReference back() const
    {
        return c_.back();
    }

    const ValueType* data() const
    {
        return c_.data();
    }

    ConstIterator begin() const
    {
        return c_.begin();
    }

    ConstIterator end() const
    {
        return c_.end();
    }

    void assign(SizeType count, const ValueType& value)
    {
		if (!empty())
			resize(0);

		detail::emit(onElementAdding, count, value);
        c_.assign(count, value);
        detail::emit(onElementAdded, c_.begin(), c_.end());
    }

    template<class InputIt>
    void assign(InputIt first, InputIt last)
    {
		if (!empty())
			resize(0);

        detail::emit(onElementAdding, first, last);
        c_.assign(first, last);
        detail::emit(onElementAdded, c_.begin(), c_.end());
    }

    void assign(std::initializer_list<ValueType> list)
    {
		if (!empty())
			resize(0);

        detail::emit(onElementAdding, list.begin(), list.end());
		c_.assign(list);
        detail::emit(onElementAdded, c_.begin(), c_.end());
    }

    void clear()
    {
        detail::emit(onElementRemoving, c_.begin(), c_.end());

		if (!empty() && onElementRemoved.slot_count())
		{
			ContainerType removed(c_.get_allocator());
			c_.swap(removed);
			detail::emit(onElementRemoved, removed.begin(), removed.end());
		}
		else
		    c_.clear();
	}

    ConstIterator insert(ConstIterator pos, const ValueType& value)
    {
		if ((pos >= c_.begin()) && (pos <= c_.end()))
		{
			onElementAdding(value);
			ConstIterator result = c_.insert(pos, value);
			onElementAdded(*result);
			return result;
		}
		else
			return c_.insert(pos, value);
    }

    ConstIterator insert(ConstIterator pos, ValueType&& value)
    {
		if ((pos >= c_.begin()) && (pos <= c_.end()))
		{
			onElementAdding(value);
			ConstIterator result = c_.insert(pos, std::forward<ValueType>(value));
			onElementAdded(*result);
			return result;
		}
		else
			return c_.insert(pos, value);
    }

    ConstIterator insert(ConstIterator pos, SizeType count, const ValueType& value)
    {
		if ((pos >= c_.begin()) && (pos <= c_.end()))
		{
			detail::emit(onElementAdding, count, value);
			ConstIterator result = c_.insert(pos, count, value);
			detail::emit(onElementAdded, result, result + count);
			return result;
		}
		else
			return c_.insert(pos, count, value);
    }
    
    template<class InputIt>
    ConstIterator insert(ConstIterator pos, InputIt first, InputIt last)
    {
		if ((pos >= c_.begin()) && (pos <= c_.end()))
		{
			detail::emit(onElementAdding, first, last);
			ConstIterator result = c_.insert(pos, first, last);
			detail::emit(onElementAdded, result, result + (last - first));
			return result;
		}
		else
			return c_.insert(pos, first, last);
    }

    ConstIterator insert(ConstIterator pos, std::initializer_list<ValueType> list)
    {
		if ((pos >= c_.begin()) && (pos <= c_.end()))
		{
			detail::emit(onElementAdding, list.begin(), list.end());
			ConstIterator result = c_.insert(pos, list);
			detail::emit(onElementAdded, result, result + init.size());
			return result;
		}
		else
			return c_.insert(pos, list);
    }
    
    template<class... Args>
    ConstIterator emplace(ConstIterator pos, Args&&... args)
    {
		if ((pos >= c_.begin()) && (pos <= c_.end()))
		{
			if (onElementAdding.slot_count())
				onElementAdding({ args... });
			ConstIterator result = c_.emplace(pos, std::forward<Args>(args)...);
			onElementAdded(*result);
			return result;
		}
		else
			return c_.emplace(pos, std::forward<Args>(args)...);
    }

    void push_back(const ValueType& value)
    {
		onElementAdding(value);
        c_.push_back(value);
        onElementAdded(c_.back());
    }

    void push_back(ValueType&& value)
    {
		onElementAdding(value);
        c_.push_back(std::forward<ValueType>(value));
        onElementAdded(c_.back());
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
		if (onElementAdding.slot_count())
			onElementAdding({ args... });
        c_.emplace_back(std::forward<Args>(args)...);
        onElementAdded(c_.back());
    }

    ConstIterator erase(ConstIterator pos)
    {
        if ((pos >= c_.begin()) && (pos < c_.end()))
		{
            onElementRemoving(*pos);
			if (onElementRemoved.slot_count())
			{
				ValueType prev = *pos;
				ConstIterator result = c_.erase(pos);
				onElementRemoved(prev);
				return result;
			}
			else
				return c_.erase(pos);
		}
		else
        	return c_.erase(pos);
    }

    ConstIterator erase(ConstIterator first, ConstIterator last)
    {
        if ((last >= c_.begin()) && (first < c_.end()))
        {
			detail::emit(onElementRemoving, first, last);
			if (onElementRemoved.slot_count())
			{
				ContainerType removed(first, last, c_.get_allocator());
				ConstIterator result = c_.erase(first, last);
				detail::emit(onElementRemoved, removed.begin(), removed.end());
				return result;
			}
			else
				return c_.erase(first, last);
        }
		else
        	return c_.erase(first, last);
    }

    void pop_back()
    {
		if (!empty())
		{
        	onElementRemoving(c_.back());
			if (onElementRemoved.slot_count())
			{
				ValueType prev = c_.back();
				c_.pop_back();
				onElementRemoved(prev);
			}
			else
				c_.pop_back();
		}
		else
			c_.pop_back();
    }

    void resize(SizeType count)
    {
        if (count < c_.size())
        {
			detail::emit(onElementRemoving, c_.begin() + count, c_.end());
			if (onElementRemoved.slot_count())
			{
				ContainerType removed(c_.begin() + count, c_.end(), c_.get_allocator());
            	c_.resize(count);
				detail::emit(onElementRemoved, removed.begin(), removed.end());
			}
			else
            	c_.resize(count);
        }
        else if (count > c_.size())
        {
			if (onElementAdding.slot_count())
				detail::emit(onElementAdding, count, {});
            SizeType prevSize = c_.size();
            c_.resize(count);
			detail::emit(onElementAdded, c_.begin() + prevSize, c_.end())
        }
    }

    void resize(SizeType count, const ValueType& value)
    {
        if (count < c_.size())
        {
			detail::emit(onElementRemoving, c_.begin() + count, c_.end());
			if (onElementRemoved.slot_count())
			{
				ContainerType removed(c_.begin() + count, c_.end(), c_.get_allocator());
            	c_.resize(count, value);
				detail::emit(onElementRemoved, removed.begin(), removed.end());
			}
			else
            	c_.resize(count, value);
        }
        else if (count > c_.size())
        {
			detail::emit(onElementAdding, count, value);
            SizeType prevSize = c_.size();
            c_.resize(count, value);
			detail::emit(onElementAdded, c_.begin() + prevSize, c_.end())
        }
    }

    void swap(ObservableSequentialCollection& other)
    {
       	detail::emit(onElementAdding, other.c_.begin(), other.c_.end());
        detail::emit(other.onElementAdding, c_.begin(), c_.end());
		detail::emit(onElementRemoving, c_.begin(), c_.end());
        detail::emit(other.onElementRemoving, other.c_.begin(), other.c_.end());
        c_.swap(other.c_);
        detail::emit(onElementRemoved, other.c_.begin(), other.c_.end());
		detail::emit(other.onElementRemoved, c_.begin(), c_.end());
		detail::emit(onElementAdded, c_.begin(), c_.end());
        detail::emit(other.onElementAdded, other.c_.begin(), other.c_.end());
    }

    void swap(ContainerType& other)
    {
       	detail::emit(onElementAdding, other.begin(), other.end());
		detail::emit(onElementRemoving, c_.begin(), c_.end());
        c_.swap(other);
        detail::emit(onElementRemoved, other.begin(), other.end());
		detail::emit(onElementAdded, c_.begin(), c_.end());
    }

    Signal onElementAdding;
    Signal onElementAdded;
    Signal onElementRemoving;
    Signal onElementRemoved;

private:
    ContainerType c_;
};

} // namespace common
