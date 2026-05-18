/*
 *	Name: ObservableSequentialCollection
 *	Author: Pawel Mrochen
 */

#pragma once

#include <concepts>
#include <initializer_list>
#include <sigslot/signal.hpp>

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
			Container removed(c_.get_allocator());
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

    ObservableSequentialCollection& operator=(const Container& other)
    {
		if (!empty())
			resize(0);

        detail::emit(onElementAdding, other.begin(), other.end());
        c_ = other;
        detail::emit(onElementAdded, c_.begin(), c_.end());
        return *this;
    }

    ObservableSequentialCollection& operator=(Container&& other)
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
			Container removed(c_.get_allocator());
			c_.swap(removed);
			detail::emit(onElementRemoved, removed.begin(), removed.end());
		}
		else
		    c_.clear();
	}

    Iterator insert(ConstIterator pos, const ValueType& value)
    {
		onElementAdding(value);
        Iterator result = c_.insert(pos, value);
        onElementAdded(*result);
        return result;
    }

    Iterator insert(ConstIterator pos, ValueType&& value)
    {
		onElementAdding(value);
        Iterator result = c_.insert(pos, std::forward<ValueType>(value));
        onElementAdded(*result);
        return result;
    }

    Iterator insert(ConstIterator pos, SizeType count, const ValueType& value)
    {
		detail::emit(onElementAdding, count, value);
        Iterator result = c_.insert(pos, count, value);
		detail::emit(onElementAdded, result, result + count);
        return result;
    }
    
    template<class InputIt>
    Iterator insert(ConstIterator pos, InputIt first, InputIt last)
    {
		detail::emit(onElementAdding, first, last);
        Iterator result = c_.insert(pos, first, last);
		detail::emit(onElementAdded, result, result + (last - first));
        return result;
    }

    Iterator insert(ConstIterator pos, std::initializer_list<ValueType> list)
    {
		detail::emit(onElementAdding, list.begin(), list.end());
        Iterator result = c_.insert(pos, list);
		detail::emit(onElementAdded, result, result + init.size());
        return result;
    }
    
    template<class... Args>
    Iterator emplace(ConstIterator pos, Args&&... args)
    {
		if (onElementAdding.slot_count())
			onElementAdding({ args... });
        Iterator result = c_.emplace(pos, std::forward<Args>(args)...);
        onElementAdded(*result);
        return result;
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

    Iterator erase(ConstIterator pos)
    {
        if ((pos >= begin()) && (pos < end()))
		{
            onElementRemoving(*pos);
			if (onElementRemoved.slot_count())
			{
				ValueType prev = *pos;
				Iterator result = c_.erase(pos);
				onElementRemoved(prev);
				return result;
			}
			else
				return c_.erase(pos);
		}
		else
        	return c_.erase(pos);
    }

    Iterator erase(ConstIterator first, ConstIterator last)
    {
        if ((last >= begin()) && (first < end()))
        {
			detail::emit(onElementRemoving, first, last);
			if (onElementRemoved.slot_count())
			{
				Container removed(first, last, c_.get_allocator());
				Iterator result = c_.erase(first, last);
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
				Container removed(c_.begin() + count, c_.end(), c_.get_allocator());
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
				Container removed(c_.begin() + count, c_.end(), c_.get_allocator());
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

    void swap(Container& other)
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
    Container c_;
};

} // namespace common
