/**
 * \file array.hpp
 */

#pragma once

#ifndef ARRAY_HPP_
#define ARRAY_HPP_

#include <cstddef>     // size_t
#include <stdexcept>   // out_of_range
#include <iterator>    // reverse_iterator


namespace mystl {


/**
 * \class array
 * 
 * \brief A fixed-size array container
 *
 * This array does not implement custom constructors or destructors,
 * allowing for direct initialization of its plain data elements by a list of {}.
 */
template <typename _T, std::size_t _Size>
class array {
public:
    using value_type             = _T;
    using size_type              = std::size_t;
    using pointer                = value_type*;
    using const_pointer          = const value_type*;
    using reference              = value_type&;
    using const_reference        = const value_type&;
    using iterator               = value_type*;
    using const_iterator         = const value_type*;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;


/* Operators */
public:
    /**
     */
    reference operator[](size_type index) {
        return p_elem[index];
    }

    /**
     */
    const_reference operator[](size_type index) const {
        return p_elem[index];
    }


/* Element access */
public:
    /**
     * \brief Access specified element with bounds checking
     *
     * Returns a reference to the element at specified location pos, with 
     * bounds checking. If pos is not within the range of the container, an 
     * exception of type std::out_of_range is thrown.
     *
     * \ref cppreference.com
     */
    reference at(size_type pos) { 
        if (pos >= _Size)
            throw std::out_of_range("array::at");
        return p_elem[pos]; 
    }

    const_reference at(size_type pos) const {
        if (pos >= _Size)
            throw std::out_of_range("array::at");
        return p_elem[pos]; 
    }

    /**
     * \brief Access the first element
     */
    reference front() noexcept {
        return p_elem[0];
    }

    const_reference front() const noexcept {
        return p_elem[0];
    }

    /**
     * \brief Access the last element
     */
    reference back() noexcept {
        return p_elem[_Size - 1];
    }

    const_reference back() const noexcept {
        return p_elem[_Size - 1];
    }

    /**
     * \brief Direct access to the underlying contiguous storage
     */
    pointer data() noexcept { return p_elem; }
    const_pointer data() const noexcept { return p_elem; }


/* iterator */
public:
    /**
     */
    iterator begin() noexcept { return iterator(data()); }
    const_iterator cbegin() const noexcept { return const_iterator(data()); }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    /**
     */
    iterator end() noexcept { return iterator(data() + _Size); }
    const_iterator cend() const noexcept { return const_iterator(data() + _Size); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }


/* Capacity */
public:
    /**
     * \brief Checks if the container has no elements. 
     *
     * \ref: cppreference.com
     */
    constexpr bool empty() const noexcept { return _Size == 0; }

    /**
     * \brief Returns the number of elements in the container. 
     *
     * \ref: cppreference.com
     */
    constexpr size_type size() const noexcept { return _Size; }

    /**
     * \brief Returns the maximum number of elements the container is able to
     * hold due to system or library implementation limitations. 
     *
     * \ref: cppreference.com
     */
    constexpr size_type max_size() const noexcept { return _Size; }


/* Operations */
public:
    /**
     * \brief Fill the container by given value
     */
    void fill(const_reference val) {
        for (size_type i = 0; i < _Size; ++i) {
            p_elem[i] = val;
        }
    }

    /**
     * \brief swaps the contents
     */
    void swap(array& other) noexcept {
        for (size_type i = 0; i < _Size; ++i) {
            std::swap(p_elem[i], other[i]);
        }
    }


public:
    value_type p_elem[_Size];
};


/**
 * \brief Specializes the std::swap algorithm for mystl::vector.
 */
template <typename _T, std::size_t _Size>
void swap(array<_T, _Size>& lhs, array<_T, _Size>& rhs) noexcept {
    lhs.swap(rhs);
}


} // namespace mystl::

#endif // ARRAY_HPP_
