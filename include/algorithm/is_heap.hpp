/**
 * \file algorithm/is_heap.hpp
 */

#pragma once

#ifndef ALGORITHM_IS_HEAP_HPP_
#define ALGORITHM_IS_HEAP_HPP_

#include <iterator>
#include <functional>

namespace mystl {


/**
 * \brief Verifies if the elements within the specified range of a heap 
 * according to a given comparison function.
 *
 * This function iterates through the range [first, last) and checks if 
 * each element satisfies the heap property relative to its parent, as 
 * determined by the comparison function 'comp'.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 * \tparam _Compare: Type of the comparison functor that determines the heap 
 *         order.
 *
 * \param first: Iterator pointing to the start of the range to be checked.
 * \param last: Iterator pointing the end of the range to be checked.
 * \param comp: Comparison functor that defines the heap order. 
 *
 * \return Returns `true` if the range [first, last) forms a heap as defined
 *         by 'comp'; otherwise, returns `false`.
 */
template <typename _RandomAccessIter, typename _Compare>
bool is_heap(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) {
    //
    using difference_type = typename std::iterator_traits<_RandomAccessIter>::difference_type;

    // 
    difference_type len = std::distance(first, last);
    if (len == 1)
        return true;

    // Iterate over each element in the range, comparing it to its parent by the `comp`.
    for (difference_type idx = 1; idx < len; ++idx) {
        _RandomAccessIter parent = first + (idx - 1) / 2;
        if (comp(*parent, *(first + idx))) {
            return false;
        }
    }
    return true;
}


/**
 * \brief Overload function of `is_heap` to use max heap by default.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 *
 * \param first: Iterator pointing to the start of the range to be checked.
 * \param last: Iterator pointing the end of the range to be checked.
 *
 * \return Returns `true` if the range [first, last) forms a max heap; 
 *         otherwise, returns `false`.
 */
template <typename _RandomAccessIter>
bool is_heap(_RandomAccessIter first, _RandomAccessIter last) {
    using value_type = typename std::iterator_traits<_RandomAccessIter>::value_type;
    return mystl::is_heap(first, last, std::less<value_type>());
}


} // namespace mystl::


#endif // ALGORITHM_IS_HEAP_HPP_
