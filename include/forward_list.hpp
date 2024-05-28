/**
 * \file forwared_list.hpp
 */

#pragma once

#ifndef FORWARD_LIST_HPP_
#define FORWARD_LIST_HPP_


#include <iterator>         // forward_iterator_tag
#include <utility>          // move, forward
#include <cstddef>          // size_t
#include <initializer_list> // initializer_list
#include <cassert>          // assert
#include <stdexcept>        // out_of_range, logic_error
#include <type_traits>      // enable_if

namespace mystl {



/**
 */
template <typename _T>
class forward_list {
private:
    struct node;

    template <typename ValueType> 
    class forward_iterator_base;

public:
    using value_type      = _T;
    using size_type       = std::size_t;
    using pointer         = _T*;
    using reference       = _T&;
    using const_pointer   = const _T*;
    using const_reference = const _T&;
    using iterator        = forward_iterator_base<_T>;
    using const_iterator  = forward_iterator_base<const _T>;

    using node_pointer    = node*;


private:
    /**
     */
    struct node {
        node() = default;

        node(const _T& data, node* next = nullptr) 
            : data(data), next(next) {}

        node(_T&& data, node* next = nullptr) 
            : data(std::move(data)), next(next) {}

        template <typename... Args>
        node(Args&&... args)
            : data(std::forward<Args> (args)...), next(nullptr) {}

        /**
         */
        _T    data = _T();      // default-inserted value
        node* next = nullptr;
    };


/* Iterators */
private:
    /**
     */
    template <typename ValueType>
    class forward_iterator_base {
        friend class forward_list<_T>;

    public:
        using value_type        = ValueType;
        using pointer           = value_type*;
        using reference         = value_type&;
        using const_pointer     = const value_type*;
        using const_reference   = const value_type&;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

    public:
        /**
         */
        forward_iterator_base(node_pointer ptr = nullptr) : p_ptr(ptr) {}

    public:
        forward_iterator_base& operator++() {
            assert(p_ptr != nullptr && "Attempting to increment an empty iterator");
            p_ptr = p_ptr->next;
            return *this;
        }

        forward_iterator_base operator++(int) {
            assert(p_ptr != nullptr && "Attempting to increment an empty iterator");
            forward_iterator_base old = *this;
            p_ptr = p_ptr->next;
            return old;
        }

        reference operator*() {
            assert(p_ptr != nullptr && "Dereferencing an empty iterator.");
            return p_ptr->data;
        }

        pointer operator->() {
            assert(p_ptr != nullptr && "Dereferencing an empty iterator.");
            return &(p_ptr->data);
        }

        bool operator==(const forward_iterator_base& other) const noexcept {
            return p_ptr == other.p_ptr;
        }

        bool operator!=(const forward_iterator_base& other) const noexcept {
            return p_ptr != other.p_ptr;
        }

    private:
        node_pointer get_node() const noexcept { return p_ptr; };

    private:
        node_pointer p_ptr;
    };


/* Constructor and Destructor*/
public:
    /**
     * Default constructor
     */
    forward_list()
        : m_size(0), p_before_head(new node)
    {
    }

    /**
     * Copy constructor
     */
    forward_list(const forward_list& other) 
        : m_size(0), p_before_head(new node)
    {
        // 
        node_pointer curr = p_before_head;

        // 
        for (node_pointer other_curr = other.p_before_head->next;
             other_curr != nullptr; other_curr = other_curr->next)
        {
            curr->next = new node(other_curr->data);
            curr = curr->next;
            ++m_size;
        }
    }

    /**
     * Move constructor
     */
    forward_list(forward_list&& other) noexcept
        : m_size(other.m_size), p_before_head(std::move(other.p_before_head))
    {
        other.m_size = 0;
        other.p_before_head = new node;   // reset other.p_before_head to a valid empty state
    }

    /**
     * \brief Construct by initializer.
     */
    forward_list(std::initializer_list<value_type> initList) 
        : m_size(0), p_before_head(new node)
    {
        node_pointer curr = p_before_head;
        for (auto& elem : initList) {
            curr->next = new node(elem);
            curr = curr->next;
            ++m_size;
        }
    }

    /**
     * Destructor
     */
    ~forward_list() {
        clear();
        delete p_before_head;
    }


/* Operators */
public:
    /**
     * Copy assignment operator
     */
    // TODO:
    forward_list& operator=(const forward_list& other);

    /**
     * Move assignment operator
     */
    // TODO:
    forward_list& operator=(forward_list&& other) noexcept;

    /**
     */
    // TODO:
    forward_list& operator=(std::initializer_list<value_type> ilist);


/* Element access */
public:
    /**
     */
    reference front() {
        if (p_before_head->next == nullptr) {
            throw std::out_of_range("forward_list is empty");
        }
        return p_before_head->next->data;
    }

    const_reference front() const {
        if (p_before_head->next == nullptr) {
            throw std::out_of_range("forward_list is empty");
        }
        return p_before_head->next->data;
    }


/* Iterators */
public:
    /**
     */
    iterator        before_begin()       noexcept { return iterator(p_before_head); }
    const_iterator cbefore_begin() const noexcept { return const_iterator(p_before_head); }

    /**
     */
    iterator        begin()       noexcept { return iterator(p_before_head->next); }
    const_iterator cbegin() const noexcept { return const_iterator(p_before_head->next); };

    /**
     */
    iterator        end()       noexcept { return iterator(nullptr); };
    const_iterator cend() const noexcept { return const_iterator(nullptr); };


/* Capacity */
public:
    /**
     * \brief Checks whether the container is empty
     * \ref cppreference.com
     */
    bool empty() const noexcept { return m_size == 0; }

    /**
     * \brief Return the number of elements.
     *
     * \note The std::forward_list does not have `size()`. I implement this
     * method for testing and debugging.
     *
     * \note The size does not include the sentinel node (`p_before_head`).
     */
    size_type size() const noexcept { return m_size; }


/* Modifiers */
public:
    /**
     * \brief Clear elements in container.
     */
    void clear() noexcept {
        // 
        node_pointer curr = p_before_head->next;
        while (curr != nullptr) {
            node_pointer next = curr->next;
            delete curr;
            curr = next;
        }

        // 
        p_before_head->next = nullptr;
        m_size = 0;
    }


    /**
     * \brief inserts elements after an element
     */
    iterator insert_after(const_iterator pos, const_reference val) {
        // 
        if (pos == cend()) {
            throw std::logic_error("mystl::insert_after: Attempting to insert after the end iterator");
        }

        // 
        node_pointer curr = pos.get_node();
        node_pointer newNode = new node(val, curr->next);
        curr->next = newNode;
        ++m_size;

        return iterator(newNode);
    }


    iterator insert_after(const_iterator pos, value_type&& val) {
        // 
        if (pos == cend()) {
            throw std::logic_error("mystl::insert_after: Attempting to insert after the end iterator");
        }

        // 
        node_pointer curr = pos.get_node();
        node_pointer newNode = new node(std::move(val), curr->next);
        curr->next = newNode;
        ++m_size;

        return iterator(newNode);
    }


    iterator insert_after(const_iterator pos, size_type count, const_reference value) {
        // 
        if (pos == cend()) {
            throw std::logic_error("mystl::insert_after: Attempting to insert after the end iterator");
        }

        // 
        iterator it = iterator(pos.get_node());
        while (count--) {
            it = insert_after(const_iterator(it.get_node()), value);
        }

        return it;
    }


    // TODO: 
    template <typename InputIt,
              typename std::enable_if<!std::is_integral<InputIt>::value, InputIt>::type* = nullptr>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last);

    // TODO: 
    iterator insert_after(const_iterator pos, std::initializer_list<value_type> iList);


    /**
     * \brief constructs elements in-place after an element
     */
    template <typename... Args>
    iterator emplace_after(const_iterator pos, Args&&... args) {
        // 
        if (pos == cend()) {
            throw std::logic_error("mystl::emplace_after: Attempting to emplace after the end iterator");
        }

        // 
        node_pointer curr = pos.get_node();
        node_pointer newNode = new node(std::forward<Args>(args)...);
        newNode->next = curr->next;
        curr->next = newNode;
        ++m_size;

        return iterator(newNode);
    }


    /**
     * \brief erases an element after an element
     */
    iterator erase_after(const_iterator pos) {
        // 
        node_pointer curr = pos.get_node();

        // 
        if (curr == nullptr)
            throw std::logic_error("erase_after(): Invalid iterator");
        else if (curr->next == nullptr)
            throw std::logic_error("erase_after(): no element after pos");

        // 
        node_pointer node_to_delete = curr->next;
        curr->next = curr->next->next;
        delete node_to_delete;

        --m_size;

        // 
        return iterator(curr->next);
    }

    iterator erase_after(const_iterator first, const_iterator last) {
        // 
        node_pointer curr = first.get_node();

        // 
        if (curr == nullptr)
            throw std::logic_error("erase_after(): Invalid iterator");
        else if (curr->next == nullptr)
            throw std::logic_error("erase_after(): no element after pos");

        // 
        while (curr->next != nullptr && curr->next != last.get_node()) {
            node_pointer node_to_delete = curr->next;
            curr->next = curr->next->next;
            delete node_to_delete;
            --m_size;
        }

        return iterator(curr->next);
    }


    /**
     * \brief inserts an element to the beginning
     */
    void push_front(const_reference value) {
        p_before_head->next = new node(value, p_before_head->next);
        ++m_size;
    }

    void push_front(value_type&& value) {
        p_before_head->next = new node(std::move(value), p_before_head->next);
        ++m_size;
    }


    /**
     * \brief constructs an element in-place at the beginning
     */
    template <typename... Args>
    void emplace_front(Args&&... args) {
        node_pointer originalFirstNode = p_before_head->next;
        p_before_head->next = new node(std::forward<Args>(args)...);
        p_before_head->next->next = originalFirstNode;
        ++m_size;
    }


    /**
     * \brief removes the first element
     */
    void pop_front() {
        if (p_before_head->next == nullptr) {
            throw std::out_of_range("pop_front(): List is empty");
        }
        node_pointer node_to_delete = p_before_head->next;
        p_before_head->next = node_to_delete->next;
        delete node_to_delete;
        --m_size;
    }


    /**
     * \brief changes the number of elements stored
     */
    void resize(size_type count) {
        resize(count, value_type());
    }

    void resize(size_type count, const_reference value) {
        if (m_size < count) {                                   // append copies of value
            iterator it = before_begin();
            std::advance(it, m_size);
            insert_after(const_iterator(it.get_node()), (count - m_size), value);
        }
        else if (m_size > count) {                              // reduce to the first `count` elements
            iterator it = before_begin();
            std::advance(it, count);
            erase_after(const_iterator(it.get_node()), cend());
        }
    }


    /**
     * \brief swaps the contents
     */
    void swap(forward_list& other) {
        std::swap(m_size, other.m_size);
        std::swap(p_before_head, other.p_before_head);
    }


private:
    size_type m_size;
    node*     p_before_head;   // sentinel node
};


/**
 * \brief Specializes the std::swap algorithm for mystl::forward_list.
 */
template <typename T>
void swap(forward_list<T>& lhs, forward_list<T>& rhs) {
    lhs.swap(rhs);
}


}


#endif // FORWARD_LIST_HPP_
