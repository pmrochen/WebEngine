/*
 *	Name: UnorderedVector
 *	Author: Pawel Mrochen
 */

#pragma once

#include <memory>
#include <utility>
#include <algorithm>
#include <vector>

namespace core {

template<typename T, typename Alloc = std::allocator<T>>
class UnorderedVector : public std::vector<T, Alloc>
{
public:
    //using ValueType = typename std::vector<T, Alloc>::value_type;
    //using AllocatorType = typename std::vector<T, Alloc>::allocator_type;
    //using ConstReference = typename std::vector<T, Alloc>::const_reference;
    //using SizeType = typename std::vector<T, Alloc>::size_type;
    //using Iterator = typename std::vector<T, Alloc>::iterator;
    //using ConstIterator = typename std::vector<T, Alloc>::const_iterator;

    using std::vector<T, Alloc>::vector;

    void add(const T& value)
    {
        push_back(value);
    }

    void add(T&& value)
    {
        push_back(std::move(value));
    }

    iterator erase(/*const_*/iterator pos)
    {
        if (++pos == end())
        {
            pop_back();
            return end();
        }

        std::swap(*pos, back());
        pop_back();
        return pos;
    }

    iterator erase(/*const_*/iterator first, /*const_*/iterator last)
    {
        if (first == last)
            return last;

        if (last == end())
            return std::vector<T, Alloc>::erase(first, last);

        auto count = std::distance(first, last);
        auto swapCount = std::min(count, std::distance(last, end()));
        std::swap_ranges(first, first + swapCount, end() - swapCount);
        std::vector<T, Alloc>::erase(end() - count, end());
        return first;
    }
};

} // namespace core
