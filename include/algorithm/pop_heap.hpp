/**
 * \file algorithm/pop_heap.hpp
 */

#pragma once

#ifndef ALGORITHM_POP_HEAP_HPP_
#define ALGORITHM_POP_HEAP_HPP_

#include <iterator>
#include <__algorithm/iter_swap.h>

namespace mystl {


/**
 * \brief Restores the heap property in a subtree rooted at 'start'.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 * \tparam _Compare: Type of the comparison functor that determines the heap 
 *         order.
 *
 * \param first: Iterator to the beginning of the range of the heap.
 * \param comp: Comparison functor that defines the heap order. 
 * \param len: Total number of elements in the heap.
 * \param start: Iterator pointing to the starting node where the heapify process begins.
 */
template <typename _RandomAccessIter, typename _Compare>
void __heapify(_RandomAccessIter first, _Compare comp,
        typename std::iterator_traits<_RandomAccessIter>::difference_type len, _RandomAccessIter start)
{
    // 
    using difference_type = typename std::iterator_traits<_RandomAccessIter>::difference_type;

    // 
    difference_type parentIdx = start - first;
    difference_type largestIdx, leftIdx, rightIdx;

    while (true) {
        // 
        largestIdx = parentIdx;

        // 
        leftIdx = 2 * parentIdx + 1;
        rightIdx = 2 * parentIdx + 2;

        // 
        if (leftIdx < len && comp(*(first + largestIdx), *(first + leftIdx)))
            largestIdx = leftIdx;

        // 
        if (rightIdx < len && comp(*(first + largestIdx), *(first + rightIdx)))
            largestIdx = rightIdx;

        // 
        if (largestIdx != parentIdx) {
            std::iter_swap((first + parentIdx), (first + largestIdx));
            parentIdx = largestIdx;
        } else {
            break;
        }
    }
}


/**
 * \brief Removes the largest element from a max heap
 *
 * Swaps the value in the position `first` and the value in the position `last - 1`,
 * and makes the subrange `[first, last - 1)` into a heap.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 * \tparam _Compare: Type of the comparison functor that determines the heap 
 *         order.
 *
 * \param first: Iterator to the beginning of the range of the heap.
 * \param last: Iterator of the end of the range of the heap.
 * \param comp: Comparison functor that defines the heap order. 
 */
template <typename _RandomAccessIter, typename _Compare>
void pop_heap(_RandomAccessIter first, _RandomAccessIter last, _Compare comp) {
    // 
    using difference_type = typename std::iterator_traits<_RandomAccessIter>::difference_type;

    // 
    if (last - first <= 1) 
        return;

    // 
    std::iter_swap(first, --last);
    difference_type len = last - first;

    // 
    __heapify(first, comp, len, first);
}


/**
 * \brief Overload function of `pop_heap` to use max heap by default.
 *
 * \tparam _RandomAccessIter: Type of the iterator used, must support random 
 *         access to elements (e.g., iterators of std::vector).
 */
template <typename _RandomAccessIter>
void pop_heap(_RandomAccessIter first, _RandomAccessIter last) {
    using value_type = typename std::iterator_traits<_RandomAccessIter>::value_type;
    return mystl::pop_heap(first, last, std::less<value_type>());
}


} // namespace mystl::


#endif // ALGORITHM_POP_HEAP_HPP_
