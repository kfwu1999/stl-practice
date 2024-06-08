/**
 * \brief queue.hpp
 */

#pragma once

#ifndef QUEUE_HPP_
#define QUEUE_HPP_


#include "list.hpp"


namespace mystl {


/**
 * \class queue
 *
 * \note : the std::queue use std::deque by default, but mystl::deque has not
 * been implemented yet, so mystl::list is used as the default container here.
 */
template <typename _T, class _Container = mystl::list<_T>>
class queue {
public:
    using container_type  = _Container;
    using value_type      = typename _Container::value_type;
    using size_type       = typename _Container::size_type;
    using reference       = typename _Container::reference;
    using const_reference = typename _Container::const_reference;

/* Constructors and Destructors */
public:
    /**
     * \brief Default contructor
     */
    queue() : m_container(container_type()) {}

    /**
     * \brief Copy construct by the container.
     */
    explicit queue(const container_type& container) : m_container(container) {}

    /**
     * \brief Copy constructor
     */
    queue(const queue& other) {
        m_container = other.m_container;
    }

    /**
     * \brief Move constructor
     */
    queue(queue&& other) noexcept {
        m_container = std::move(other.m_container);
    }

    /**
     * \brief Destructor
     */
    ~queue() {
    }

/* Operators */
public:
    /**
     * \brief Copy assignment operator
     */
    queue& operator=(const queue& other) {
        if (this != &other)
            m_container = other.m_container; 
        return *this;
    }

    /**
     * \brief Move assignment operator
     */
    queue& operator=(queue&& other) noexcept {
        if (this != &other)
            m_container = std::move(other.m_container); 
        return *this;
    }

/* Element access */
public:
    /**
     * \brief Access the first element.
     */
    reference front() { return m_container.front(); }

    /**
     * \brief Access the first element.
     */
    const_reference front() const { return m_container.front(); }

    /**
     * \brief Access the last element.
     */
    reference back() { return m_container.back(); }

    /**
     * \brief Access the last element.
     */
    const_reference back() const { return m_container.back(); }

/* Capacity */
public:
    /**
     * \brief Checks whether the container adaptor is empty.
     */
    bool empty() const { return m_container.empty(); }

    /**
     * \brief Returns the number of elements.
     */
    size_type size() const { return m_container.size(); }

/* Modifiers */
public:
    /**
     * \brief Inserts element at the end.
     */
    void push(const_reference value) { m_container.push_back(value); }

    /**
     * \brief Inserts element at the end.
     */
    void push(value_type&& value) { m_container.push_back(std::move(value)); }

    /**
     */
    template <typename ...Args>
    void emplace(Args&&... args) { m_container.emplace_back(std::forward<Args>(args)...); }

    /**
     * \brief Removes the first element.
     */
    void pop() { m_container.pop_front(); }

    /**
     * \brief Swaps the contents.
     */
    void swap(queue& other) noexcept { m_container.swap(other); }

private:
    container_type m_container;
};


/**
 * \brief Specializes the std::swap algorithm for std::queue.
 */
template <class _T, class _Container>
void swap(queue<_T, _Container>& lhs, queue<_T, _Container>& rhs) {
    lhs.swap(rhs);
}


} // namespace mystl::


#endif // QUEUE_HPP_
