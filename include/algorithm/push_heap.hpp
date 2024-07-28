/**
 * \file algorithm/push_heap.hpp
 */

#pragma once

#ifndef ALGORITHM_PUSH_HEAP_HPP_
#define ALGORITHM_PUSH_HEAP_HPP_

#include <iterator>
#include <algorithm>   // for iter_swap   TODO: implement my own iter_swap someday for avoid cyclic dependencies.

namespace mystl {


/**
 * \brief Integrates a newly added element into an existing heap, maintaining the heap property.
 *
 * Performing "sift-up" operation to the newly added element in the heap, ensuring 
 * the heap property is maintained according to a specified comparison functor.
 * After calling push_heap, the entire range [first, last) will satisfy the heap property.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 * \tparam _Compare: Type of the comparison functor that determines the heap 
 *         order.
 *
 * \param first: Iterator to the beginning of the range of the heap.
 * \param last: Iterator to the last element of the range, which is the newly added element.
 * \param comp: Comparison functor that defines the heap order. 
 */
template <typename _RandomAccessIter, typename _Compare>
void push_heap(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) {
    //
    using difference_type = typename std::iterator_traits<_RandomAccessIter>::difference_type;

    // 
    difference_type len = last - first;
    if (len <= 1)
        return;

    // 
    difference_type childIdx = len - 1;               // last element in the range of the heap (newly added element).
    difference_type parentIdx = (childIdx - 1) / 2;   // parent of the childIdx

    // 
    while (childIdx > 0 && comp(*(first + parentIdx), *(first + childIdx))) {
        // swap if the child didn't statisfy the property of the heap
        std::iter_swap(first + parentIdx, first + childIdx);

        // 
        childIdx = parentIdx;
        parentIdx = (childIdx - 1) / 2;
    }
}


/**
 * \brief Overload function of `push_heap` to use max heap by default.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 *
 * \param first: Iterator to the beginning of the range of the heap.
 * \param last: Iterator to the last element of the range, which is the newly added element.
 */
template <typename _RandomAccessIter>
void push_heap(_RandomAccessIter first, _RandomAccessIter last) {
    using value_type = typename std::iterator_traits<_RandomAccessIter>::value_type;
    return mystl::push_heap(first, last, std::less<value_type>());
}


} // namespace mystl::

#endif // ALGORITHM_PUSH_HEAP_HPP_
