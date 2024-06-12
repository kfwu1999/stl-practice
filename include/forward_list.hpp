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

    template <typename _Iter_val, typename _Iter_ptr, typename _Iter_ref>
    class forward_iterator_base;

public:
    using value_type      = _T;
    using size_type       = std::size_t;
    using pointer         = _T*;
    using reference       = _T&;
    using const_pointer   = const _T*;
    using const_reference = const _T&;
    using iterator        = forward_iterator_base<_T, pointer, reference>;
    using const_iterator  = forward_iterator_base<_T, const_pointer, const_reference>;

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
        value_type   data = value_type();      // default-inserted value
        node_pointer next = nullptr;
    };


/* Iterators */
private:
    /**
     * \brief Iterator for the mystl::list class template that supports forward iterator operations.
     *
     * \tparam _Iter_val Type of the value that the iterator points to.
     * \tparam _Iter_ptr Type of the pointer to the value (const or non-const).
     * \tparam _Iter_ref Type of the reference to the value (const or non-const).
     */
    template <typename _Iter_val, typename _Iter_ptr, typename _Iter_ref>
    class forward_iterator_base {
        friend class forward_list<_T>;

    public:
        using value_type        = _Iter_val;
        using pointer           = _Iter_ptr;
        using reference         = _Iter_ref;
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
     * \brief Constructs the container with count copies of elements with value value.
     */
    explicit forward_list(size_type count, const_reference value = value_type())
        : m_size(0), p_before_head(new node)
    {
        insert_after(cbefore_begin(), count, value);
    }

    /**
     * \brief Constructs the container with the contents of the range `[first, last)`.
     */
    template <typename InputIt,
              typename std::enable_if<!std::is_integral<InputIt>::value, InputIt>::type* = nullptr>
    forward_list(InputIt first, InputIt last)
        : m_size(0), p_before_head(new node)
    {
        insert_after(cbefore_begin(), first, last);
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
        insert_after(cbefore_begin(), initList);
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
    forward_list& operator=(const forward_list& other) {
        if (this != &other) {
            // 
            clear();
            m_size = 0;

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

        // 
        return *this;
    }

    /**
     * Move assignment operator
     */
    forward_list& operator=(forward_list&& other) noexcept {
        if (this != &other) {
            // 
            clear();
            delete p_before_head;

            // 
            m_size = other.m_size;
            p_before_head = std::move(other.p_before_head);

            // 
            other.m_size = 0;
            other.p_before_head = new node;   // reset other.p_before_head to a valid empty state
        }

        // 
        return *this;
    }

    /**
     */
    forward_list& operator=(std::initializer_list<value_type> initList) {
        // 
        clear();
        m_size = 0;

        // 
        node_pointer curr = p_before_head;
        for (auto& elem : initList) {
            curr->next = new node(elem);
            curr = curr->next;
            ++m_size;
        }

        // 
        return *this;
    }


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
    bool empty() const noexcept { return p_before_head->next == nullptr; }

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


    template <typename InputIt,
              typename std::enable_if<!std::is_integral<InputIt>::value, InputIt>::type* = nullptr>
    iterator insert_after(const_iterator pos, InputIt first, InputIt last) {
        iterator nonConstPos = iterator(pos.get_node());
        for (auto it = first; it != last; ++it) {
            nonConstPos = insert_after(const_iterator(nonConstPos.get_node()), *it);
        }
        return nonConstPos;
    }


    iterator insert_after(const_iterator pos, std::initializer_list<value_type> iList) {
        return insert_after(pos, iList.begin(), iList.end());
    }


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


/* Operations */
public:
    /**
     * \brief Merge two sorted list
     *
     * \param other: another comtainer to merge
     *
     * \note No iterators or references become invalidated.
     * \note Undefined behavior if the `this` or `other` is not sorted
     */
    void merge(forward_list& other) {
        // This function does nothing if `other` refers to the same object as `this`
        if (this == &other)
            return;

        // 
        node_pointer dummy = new node;
        node_pointer tail = dummy;
        node_pointer curr1 = p_before_head->next;
        node_pointer curr2 = other.p_before_head->next;

        // 
        while (curr1 != nullptr && curr2 != nullptr) {
            if (curr1->data < curr2->data) {
                tail->next = curr1;
                curr1 = curr1->next;
            } else {
                tail->next = curr2;
                curr2 = curr2->next;
            }
            tail = tail->next;
        }

        // 
        if (curr1 != nullptr)
            tail->next = curr1;
        if (curr2 != nullptr)
            tail->next = curr2;

        // 
        p_before_head->next = dummy->next;
        m_size += other.m_size;

        other.p_before_head->next = nullptr;
        other.m_size = 0;

        delete dummy;
    }


    /**
     * \brief Move elements from another forward_list
     *
     * Moves elements from antoher forward_list to `this`. 
     *
     * \param pos: element after which the content will be inserted.
     * \param other: anothe rcontainer to move the content from
     *
     * \note No iterators or references become invalidated.
     */
    void splice_after(const_iterator pos, forward_list& other) {
        // 
        if (pos == cbefore_begin() || pos == cend())
            throw std::logic_error("splice_after(): Attempting to splice after end or before_begin.");

        // 
        node_pointer pos_ptr = pos.get_node();

        // 
        if (pos_ptr->next != nullptr) {
            // Find the last node in the other list
            node_pointer other_tail = other.p_before_head->next;
            while (other_tail->next != nullptr)
                other_tail = other_tail->next;
            other_tail->next = pos_ptr->next;
        }

        // 
        pos_ptr->next = other.p_before_head->next;
        m_size += other.m_size;

        // 
        other.p_before_head->next = nullptr;
        other.m_size = 0;
    }


    /**
     * \brief Removes elements satisfying specific criteria
     *
     * \param value: valud of the elements to remove
     */
    void remove(const_reference value) {
        // 
        node_pointer prev = p_before_head;
        node_pointer curr = p_before_head->next;

        // 
        while (curr != nullptr) {
            if (curr->data == value) {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
                --m_size;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }

    /**
     * \brief Reverse the order of the elements
     */
    void reverse() noexcept {
        // 
        node_pointer prev = nullptr;
        node_pointer curr = p_before_head->next;
        node_pointer next = nullptr;

        // 
        while (curr != nullptr) {
            next = curr->next;
            curr->next = prev;
            prev = curr;
            curr = next;
        }

        // 
        p_before_head->next = prev;
    }

    /**
     * \brief Removes consecutive duplicate elements
     */
    void unique() {
        // 
        node_pointer prev = p_before_head;
        node_pointer curr = p_before_head->next;

        // 
        while (curr != nullptr) {
            // skip at beginning because the `data` of `prev` is undefined
            if (prev != p_before_head && prev->data == curr->data) {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
                --m_size;
            } else {
                prev = curr;
                curr = curr->next;
            }
        }
    }

    /**
     * \brief Sorts the elements
     *
     * \note use merge sort
     */
    void sort() {
        p_before_head->next = merge_sort(p_before_head->next);
    }


private:
    /**
     * \brief Merge sort
     */
    node_pointer merge_sort(node_pointer head) {
        // 
        if (head == nullptr || head->next == nullptr)
            return head;

        //
        node_pointer mid = get_middle_node(head);
        node_pointer mid_next = mid->next;
        mid->next = nullptr;

        // 
        node_pointer left_part = merge_sort(head);
        node_pointer right_part = merge_sort(mid_next);

        return merge(left_part, right_part);
    }


    /**
     * \brief Get middle node for merge sort
     */
    node_pointer get_middle_node(node_pointer head) {
        // 
        node_pointer fast = head;
        node_pointer slow = head;

        // 
        while (fast->next != nullptr && fast->next->next != nullptr) {
            fast = fast->next->next;
            slow = slow->next;
        }

        return slow;
    }


    /**
     * \brief merge for merge_sort
     *
     * \param head1: head of sorted list 1
     * \param head2: head of sorted list 2
     */
    node_pointer merge(node_pointer head1, node_pointer head2) {
        // 
        node_pointer dummy = new node;
        node_pointer tail = dummy;

        // 
        while (head1 != nullptr && head2 != nullptr) {
            if (head1->data < head2->data) {
                tail->next = head1;
                head1 = head1->next;
            } else {
                tail->next = head2;
                head2 = head2->next;
            }
            tail = tail->next;
        }

        // 
        if (head1 != nullptr)
            tail->next = head1;
        if (head2 != nullptr)
            tail->next = head2;

        // 
        node_pointer head = dummy->next;
        delete dummy;

        return head;
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
