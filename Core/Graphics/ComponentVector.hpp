/*
 *	Name: ComponentVector
 *	Author: Pawel Mrochen
 */

#pragma once

#include <type_traits>
#include <concepts>
#include <memory>
#include <vector>
#include <tuple>
#include <iterator>
#include <Vector2.hpp>
#include <Vector3.hpp>
#include <Vector4.hpp>

namespace graphics {

template<typename T>
concept TupleLike = requires { std::tuple_size<T>::value; };

template<typename T>
struct ComponentCount : std::integral_constant<std::size_t, 1> {};

template<TupleLike T>
struct ComponentCount<T> : std::integral_constant<std::size_t, std::tuple_size<T>::value> {};

template<typename T, typename Alloc = std::allocator<T>>
class ComponentVector : public std::vector<T, Alloc>
{
public:
    using BaseType = std::vector<T, Alloc>;
    using ValueType = typename BaseType::value_type;
    using AllocatorType = typename BaseType::allocator_type;
    using ConstReference = typename BaseType::const_reference;
    using SizeType = typename BaseType::size_type;
    using Iterator = typename BaseType::iterator;
    using ConstIterator = typename BaseType::const_iterator;

    // #TODO Constructors

    using BaseType::size;

    template<TupleLike U>
    SizeType size() const
    {
        if constexpr (ComponentCount<U>::value == 1)
            return BaseType::size();
        else
            return BaseType::size()/ComponentCount<U>::value;
    }

	template<TupleLike U>
    U get(SizeType pos) const
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            return { BaseType::at(pos)) };
        }
        else
        {
            Iterator i = begin() + (pos*ComponentCount<U>::value);

            if constexpr (ComponentCount<U>::value == 2)
                return { i[0], i[1] };
            else if constexpr (ComponentCount<U>::value == 3)
                return { i[0], i[1], i[2] };
            else if constexpr (ComponentCount<U>::value == 4)
                return { i[0], i[1], i[2], i[3] };
            else
                return {}; // #TODO
        }
    }

    template<TupleLike U>
    void set(SizeType pos, const U& value)
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            BaseType::at(pos) = value;
        }
        else
        {
            Iterator i = begin() + (pos*ComponentCount<U>::value);

            if constexpr (ComponentCount<U>::value == 2)
            {
                i[0] = value.get<0>();
                i[1] = value.get<1>();
            }
            else if constexpr (ComponentCount<U>::value == 3)
            {
                i[0] = value.get<0>();
                i[1] = value.get<1>();
                i[2] = value.get<2>();
            }
            else if constexpr (ComponentCount<U>::value == 4)
            {
                i[0] = value.get<0>();
                i[1] = value.get<1>();
                i[2] = value.get<2>();
                i[3] = value.get<3>();
            }
            else
            {
                // #TODO
            }
        }
    }

    //template<TupleLike U>
    //void assign(SizeType count, const U& value)
    //{
    //}

    using BaseType::assign;

    template<std::input_iterator I>
        requires TupleLike<std::iter_value_t<I>>
    void assign(I first, I last)
    {
        if constexpr (std::tuple_size<std::iter_value_t<I>>::value == 1)
        {
            BaseType::assign(first, last);
        }
        else
        {
            BaseType::resize(0); // clear???
            BaseType::reserve(std::distance(first, last)*std::tuple_size<std::iter_value_t<I>>::value);
            for (; first != last; ++first)
                pushBack(*first);
        }
    }

    //template<TupleLike U>
    //void assign(std::initializer_list<U> init)
    //{
    //}

    template<std::input_iterator I>
        requires TupleLike<std::iter_value_t<I>>
    void append(I first, I last)
    {
        if constexpr (std::tuple_size<std::iter_value_t<I>>::value == 1)
        {
            BaseType::assign(first, last);
        }
        else
        {
            BaseType::reserve(BaseType::size() + std::distance(first, last)*std::tuple_size<std::iter_value_t<I>>::value);
            for (; first != last; ++first)
                pushBack(*first);
        }
    }

    using BaseType::insert;

    template<TupleLike U>
    Iterator insert(ConstIterator pos, const U& value)
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            BaseType::insert(pos, value);
        }
        else if constexpr (ComponentCount<U>::value == 2)
        {
            BaseType::insert(pos, value.get<0>());
            BaseType::insert(pos + 1, value.get<1>());
        }
        else if constexpr (ComponentCount<U>::value == 3)
        {
            BaseType::insert(pos, value.get<0>());
            BaseType::insert(pos + 1, value.get<1>());
            BaseType::insert(pos + 2, value.get<2>());
        }
        else if constexpr (ComponentCount<U>::value == 4)
        {
            BaseType::insert(pos, value.get<0>());
            BaseType::insert(pos + 1, value.get<1>());
            BaseType::insert(pos + 2, value.get<2>());
            BaseType::insert(pos + 3, value.get<3>());
        }
        else
        {
            // #TODO
        }
    }

    template<TupleLike U>
    Iterator insert(SizeType pos, const U& value)
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            BaseType::insert(begin() + pos, value);
        }
        else
        {
            Iterator i = begin() + (pos*ComponentCount<U>::value);
            
            if constexpr (ComponentCount<U>::value == 2)
            {
                BaseType::insert(i, value.get<0>());
                BaseType::insert(i + 1, value.get<1>());
            }
            else if constexpr (ComponentCount<U>::value == 3)
            {
                BaseType::insert(i, value.get<0>());
                BaseType::insert(i + 1, value.get<1>());
                BaseType::insert(i + 2, value.get<2>());
            }
            else if constexpr (ComponentCount<U>::value == 4)
            {
                BaseType::insert(i, value.get<0>());
                BaseType::insert(i + 1, value.get<1>());
                BaseType::insert(i + 2, value.get<2>());
                BaseType::insert(i + 3, value.get<3>());
            }
            else
            {
                // #TODO
            }
        }
    }

    //template<TupleLike U>
    //Iterator insert(ConstIterator pos, U&& value)
    //{
    //}

    //template<TupleLike U>
    //Iterator insert(ConstIterator pos, SizeType count, const U& value)
    //{
    //}

    //template<std::input_iterator I>
    //Iterator insert(ConstIterator pos, I first, I last)
    //{
    //}

    //template<TupleLike U>
    //Iterator insert(ConstIterator pos, std::initializer_list<U> init)
    //{
    //}

    //template<typename... Args>
    //Iterator emplace(ConstIterator pos, Args&&... args)
    //{
    //}

    template<TupleLike U>
    void pushBack(const U& value)
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            BaseType::push_back(value);
        }
        else if constexpr (ComponentCount<U>::value == 2)
        {
            BaseType::push_back(value.get<0>());
            BaseType::push_back(value.get<1>());
        }
        else if constexpr (ComponentCount<U>::value == 3)
        {
            BaseType::push_back(value.get<0>());
            BaseType::push_back(value.get<1>());
            BaseType::push_back(value.get<2>());
        }
        else if constexpr (ComponentCount<U>::value == 4)
        {
            BaseType::push_back(value.get<0>());
            BaseType::push_back(value.get<1>());
            BaseType::push_back(value.get<2>());
            BaseType::push_back(value.get<3>());
        }
        else
        {
            // #TODO
        }
    }

    //template<TupleLike U>
    //void push_back(U&& value)
    //{
    //}

    //template<typename... Args>
    //void emplace_back(Args&&... args)
    //{
    //}

    //template<TupleLike U>
    //Iterator erase(ConstIterator pos)
    //{
    //}

    //template<TupleLike U>
    //Iterator erase(ConstIterator first, ConstIterator last)
    //{
    //}

    using BaseType::erase;

    template<TupleLike U>
    Iterator erase(SizeType pos)
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            BaseType::erase(begin() + pos);
        }
        else
        {
            Iterator i = begin() + (pos*ComponentCount<U>::value);
            BaseType::erase(i, i + ComponentCount<U>::value);
        }
    }

    template<TupleLike U>
    void popBack()
    {
        if constexpr (ComponentCount<U>::value == 1)
        {
            BaseType::pop_back();
        }
        else if constexpr (ComponentCount<U>::value == 2)
        {
            BaseType::pop_back();
            BaseType::pop_back();
        }
        else if constexpr (ComponentCount<U>::value == 3)
        {
            BaseType::pop_back();
            BaseType::pop_back();
            BaseType::pop_back();
        }
        else if constexpr (ComponentCount<U>::value == 4)
        {
            BaseType::pop_back();
            BaseType::pop_back();
            BaseType::pop_back();
            BaseType::pop_back();
        }
        else
        {
            for (SizeType i = 0; i < ComponentCount<U>::value; i++)
                BaseType::pop_back();
        }
    }

    using BaseType::resize;

    template<TupleLike U>
    void resize(SizeType count)
    {
        if constexpr (ComponentCount<U>::value == 1)
            BaseType::resize(count);
        else
            BaseType::resize(count*ComponentCount<U>::value);
    }

    //template<TupleLike U>
    //void resize(SizeType count, const U& value)
    //{
    //}
};

template<typename T>
struct ComponentVectorOf
{
    using Type = ComponentVector<T>;
};

template<TupleLike T>
struct ComponentVectorOf<T>
{
    using Type = ComponentVector<typename std::tuple_element<0, T>::type>;
};

} // namespace graphics
