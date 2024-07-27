/**
 * \file priority_queue.hpp
 */

#pragma once

#ifndef PRIORITY_QUEUE_HPP_
#define PRIORITY_QUEUE_HPP_

#include <functional>   // std::less
#include <utility>      // std::move

#include "vector.hpp"
#include "algorithm/push_heap.hpp"
#include "algorithm/pop_heap.hpp"
#include "algorithm/make_heap.hpp"

namespace mystl {

/**
 * \class priority_queue
 *
 * \tparam _T: Type of the elements.
 * \tparam _Container: Type of the underlying container, must support random 
 * access iterators (defaults to `mystl::vector<_T>`). 
 * \tparam _Compare: Comparison functor type, defaults to `std::less<>` for max heap.
 */
template <class _T, class _Container = mystl::vector<_T>,
          class _Compare = std::less<typename _Container::value_type> >
class priority_queue {
public:
    using container_type  = _Container;
    using value_compare   = _Compare;
    using value_type      = typename _Container::value_type;
    using size_type       = typename _Container::size_type;
    using reference       = typename _Container::reference;
    using const_reference = typename _Container::const_reference;

/* Constructor and Destructor */
public:
    /**
     * \brief Construct an empty priority_queue.
     */
    priority_queue()
        : m_container(container_type()), m_comp(value_compare()) {}


    /**
     * \brief Construct an empty priority_queue with a custom comparison functor
     *
     * \param comp: The comparison functor to use for element ordering.
     */
    explicit priority_queue(const value_compare& comp)
        : m_container(container_type()), m_comp(comp) {}


    /**
     * \brief Construct the priority_queue with a custom comparison functor and
     *        an existing container.
     *
     * \param comp: The comparison functor to use for element ordering.
     * \param container: A container to be used as the underlying container.
     */
    priority_queue(const value_compare& comp, const container_type& container)
        : m_container(container), m_comp(comp) 
    {
        mystl::make_heap(m_container.begin(), m_container.end(), m_comp);
    }


    /**
     * \brief Construct the priority_queue by move-construct a container and 
     *        copy-construct a comparison functor.
     *
     * \param comp: The comparison functor to use for element ordering.
     * \param container: A container to be moved as the underlying container.
     */
    priority_queue(const value_compare& comp, container_type&& container)
        : m_container(std::move(container)), m_comp(comp) 
    {
        mystl::make_heap(m_container.begin(), m_container.end(), m_comp);
    }


    /**
     * \brief Copy constructor
     */
    priority_queue(const priority_queue& other)
        : m_container(other.m_container), m_comp(other.m_comp) {}


    /**
     * \brief Move constructor
     */
    priority_queue(priority_queue&& other)
        : m_container(std::move(other.m_container)), m_comp(std::move(other.m_comp)) {}


    /**
     * \brief Constructs the priority_queue from a range and an optional comparison functor.
     *
     * \param first, last: Input iterators to the initial range.
     * \param comp: Comparison functor, optional.
     */
    template <typename InputIt,
              typename std::enable_if<!std::is_integral<InputIt>::value, InputIt>::type* = nullptr>
    priority_queue(InputIt first, InputIt last, const value_compare& comp = value_compare()) 
        : m_container(first, last), m_comp(comp)
    {
        mystl::make_heap(m_container.begin(), m_container.end(), m_comp);
    }


    /**
     * \brief Destructor
     */
    ~priority_queue() {
    }


/* Operators */
public:
    /**
     * \brief Copy assignment operator.
     */
    priority_queue& operator=(const priority_queue& other) {
        m_container = other.m_container;
        m_comp = other.m_comp;
        return *this;
    }


    /**
     * \brief Move assignment operator.
     */
    priority_queue& operator=(priority_queue&& other) noexcept {
        m_container = std::move(other.m_container);
        m_comp = std::move(other.m_comp);
        return *this;
    }


/* Element access */
public:
    /**
     * \brief Access the top element.
     *
     * \return A constant reference to the top element, which is the highest 
     *         priority element.
     */
    const_reference top() const { return m_container.front(); }


/* Capacity */
public:
    /**
     * \brief Checks whether the priority_queue is empty.
     */
    bool empty() const { return m_container.empty(); }


    /**
     * \brief Returns the number of elements.
     */
    size_type size() const { return m_container.size(); }


/* Modifiers */
public:
    /**
     * \brief Inserts an element and reorders the priority_queue.
     *
     * \param value: The value to insert.
     */
    void push(const_reference value) {
        m_container.push_back(value);
        mystl::push_heap(m_container.begin(), m_container.end(), m_comp);
    }


    /**
     * \brief Inserts a movable element and reorders the priority_queue.
     *
     * \param value The value to move into the queue.
     */
    void push(value_type&& value) {
        m_container.push_back(std::move(value));
        mystl::push_heap(m_container.begin(), m_container.end(), m_comp);
    }


    /**
     * \brief Constructs an element in-place and reorders the priority_queue.
     *
     * \tparam Args: Variadic template arguments corresponding to the constructor 
     *               of the element type.
     * \param args: Arguments to forward to the constructor of the element.
     */
    template <typename... Args>
    void emplace(Args&&... args) {
        m_container.emplace_back(std::forward<Args>(args)...);
        mystl::push_heap(m_container.begin(), m_container.end(), m_comp);
    }


    /**
     * \brief Removes the top element of the priority_queue.
     */
    void pop() {
        mystl::pop_heap(m_container.begin(), m_container.end(), m_comp);
        m_container.pop_back();
    }


    /**
     * \brief Swaps this priority_queue with another priority_queue.
     *
     * \param other: Another priority_queue to swap with.
     */
    void swap(priority_queue& other) noexcept {
        std::swap(m_container, other.m_container);
        std::swap(m_comp, other.m_comp);
    }


/**/
private:
    container_type m_container;
    value_compare  m_comp;
};


} // namespace mystl::


#endif // PRIORITY_QUEUE_HPP_
