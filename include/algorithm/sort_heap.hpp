/**
 * \file algorithm/sort_heap.hpp
 */

#pragma once

#ifndef ALGORITHM_SORT_HEAP_HPP_
#define ALGORITHM_SORT_HEAP_HPP_

#include <iterator>
#include <functional>
#include <algorithm>   // for iter_swap   TODO: implement my own iter_swap someday for avoid cyclic dependencies.

#include "algorithm/pop_heap.hpp"

namespace mystl {


/**
 * \brief Turns a heap to sorted order.
 *
 * Converts the heap [first, last) into a sorted range. After calling this function, 
 * the heap property is no longer maintained.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 * \tparam _Compare: Type of the comparison functor that determines the heap 
 *         order.
 *
 * \param first: Iterator to the beginning of the range of the heap.
 * \param last: Iterator of the end of the range of the heap.
 * \param comp: Comparison functor that defines the sorted order. 
 */
template <typename _RandomAccessIter, typename _Compare>
void sort_heap(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) {
    // 
    if (last - first <= 1)
        return;

    // 
    while (last != first)
        mystl::pop_heap(first, last--, comp);
}


/**
 * \brief Overload function of `sort_heap` to use max heap by default
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 *
 * \param first: Iterator to the beginning of the range of the heap.
 * \param last: Iterator of the end of the range of the heap.
 */
template <typename _RandomAccessIter>
void sort_heap(_RandomAccessIter first, _RandomAccessIter last) {
    using value_type = typename std::iterator_traits<_RandomAccessIter>::value_type;
    mystl::sort_heap(first, last, std::less<value_type>());
}


} // namespace mystl::

#endif // ALGORITHM_SORT_HEAP_HPP_
