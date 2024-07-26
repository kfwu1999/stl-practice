/**
 * \file algorithm/make_heap.hpp
 */

#pragma once

#ifndef ALGORITHM_MAKE_HEAP_HPP_
#define ALGORITHM_MAKE_HEAP_HPP_

#include <iterator>
#include "algorithm/pop_heap.hpp"   // for __heapify()

namespace mystl {


/**
 * \brief Constructs a heap in the range [first, last).
 *
 * Rearrange elements in [first, last) so that they satisfy the hap property 
 * according to the specified comparison function.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 * \tparam _Compare: Type of the comparison functor that determines the heap 
 *         order.
 *
 * \param first: Iterator pointing to the start of the range to be heapified.
 * \param last: Iterator pointing past the end of the range to be heapified.
 */
template <typename _RandomAccessIter, typename _Compare>
void make_heap(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) {
    // 
    using difference_type = typename std::iterator_traits<_RandomAccessIter>::difference_type;
    difference_type len = last - first;

    // 
    if (len <= 1) 
        return;

    // 
    for (difference_type start = len / 2 - 1; start >= 0; --start) {
        __heapify(first, comp, len, first + start);
    }
}


/**
 * \brief Overload function of `make_heap` to use max heap by default.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 */
template <typename _RandomAccessIter>
void make_heap(_RandomAccessIter first, _RandomAccessIter last) {
    using value_type = typename std::iterator_traits<_RandomAccessIter>::value_type;
    return mystl::make_heap(first, last, std::less<value_type>());
}


} // namespace mystl::

#endif // ALGORITHM_MAKE_HEAP_HPP_
