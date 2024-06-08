/**
 * \brief stack.hpp
 */

#pragma once

#ifndef STACK_HPP_
#define STACK_HPP_

#include "vector.hpp"

namespace mystl {


/**
 * \brief stack
 *
 * \note : the std::stack use std::deque by default, but mystl::deque has not
 * been implemented yet, so mystl::vector is used as the default container here.
 */
template <class _T, class _Container = mystl::vector<_T>>
class stack {
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
    stack() : m_container(container_type()) {}

    /**
     * \brief Copy construct by the container.
     */
    explicit stack(const container_type& container) : m_container(container) {}

    /**
     * \brief Copy constructor
     */
    stack(const stack& other) {
        m_container = other.m_container;
    }

    /**
     * \brief Move constructor
     */
    stack(stack&& other) noexcept {
        m_container = std::move(other.m_container);
    }

    /**
     * \brief Destructor
     */
    ~stack() {
    }

/* Operators */
public:
    /**
     * \brief Copy assignment operator
     */
    stack& operator=(const stack& other) { 
        if (this != &other)
            m_container = other.m_container; 
        return *this;
    }

    /**
     * \brief Move assignment operator
     */
    stack& operator=(stack&& other) noexcept {
        if (this != &other)
            m_container = std::move(other.m_container); 
        return *this;
    }

/* Element access */
public:
    /**
     * \brief Accesses the top element.
     */
    reference top() { return m_container.back(); }

    /**
     * \brief Accesses the top element.
     */
    const_reference top() const { return m_container.back(); }

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
     * \brief inserts element at the top
     */
    void push(const_reference value) { m_container.push_back(value); }

    /**
     * \brief inserts element at the top
     */
    void push(value_type&& value) { m_container.push_back(std::move(value)); }

    /**
     * \brief Constructs elements in-place at the top.
     */
    template <typename ...Args>
    void emplace(Args&&... args) { m_container.emplace_back(std::forward<Args>(args)...); }

    /**
     * \brief Removes the top elements.
     */
    void pop() { m_container.pop_back(); }

    /**
     * \brief Swaps the contents.
     */
    void swap(stack& other) noexcept { m_container.swap(other); }

private:
    container_type m_container;
};


/**
 * \brief Specializes the std::swap algorithm for std::stack.
 */
template <class _T, class _Container>
void swap(stack<_T, _Container>& lhs, stack<_T, _Container>& rhs) {
    lhs.swap(rhs);
}


} // namespace mystl::


#endif // STACK_HPP_
