/**
 * \file list.hpp
 */

#pragma once

#ifndef LIST_HPP_
#define LIST_HPP_

#include <iterator>         // bidirectional_iterator_tag
#include <utility>          // move, forward
#include <cstddef>          // size_t
#include <initializer_list> // initializer_list
#include <stdexcept>        // out_of_range, logic_error


namespace mystl {


/**
 */
template <typename _T>
class list {
private:
    struct node;
    template <typename _Iter_val, typename _Iter_ptr, typename _Iter_ref>
    class list_iterator_base;

public:
    using value_type             = _T;
    using size_type              = std::size_t;
    using pointer                = _T*;
    using reference              = _T&;
    using const_pointer          = const _T*;
    using const_reference        = const _T&;
    using iterator               = list_iterator_base<_T, pointer, reference>;
    using const_iterator         = list_iterator_base<_T, const_pointer, const_reference>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using node_pointer    = node*;

private:
    /**
     */
    struct node {
        /**
         */
        node() = default;

        node(const_reference data, node_pointer prev = nullptr, node_pointer next = nullptr)
            : data(data), prev(prev), next(next) {}

        node(value_type&& data, node_pointer prev = nullptr, node_pointer next = nullptr)
            : data(std::move(data)), prev(prev), next(next) {}

        template <typename... Args>
        node(Args&&... args)
            : data(std::forward<Args>(args)...), prev(nullptr), next(nullptr) {}

        /**/
        value_type   data = value_type();
        node_pointer prev = nullptr;
        node_pointer next = nullptr;
    };

/* Iterator */
private:
    /**
     * \brief Iterator for the mystl::list class template that supports bidirectional iterator operations.
     *
     * \tparam _Iter_val Type of the value that the iterator points to.
     * \tparam _Iter_ptr Type of the pointer to the value (const or non-const).
     * \tparam _Iter_ref Type of the reference to the value (const or non-const).
     */
    template <typename _Iter_val, typename _Iter_ptr, typename _Iter_ref>
    class list_iterator_base {
        friend class list<_T>;

    public:
        using value_type        = _Iter_val;
        using pointer           = _Iter_ptr;
        using reference         = _Iter_ref;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

    public:
        list_iterator_base(node_pointer ptr = nullptr) : p_ptr(ptr) {}

    public:
        list_iterator_base& operator++() {
            assert(p_ptr != nullptr && "Attempting to increment an empty iterator");
            p_ptr = p_ptr->next;
            return *this;
        }

        list_iterator_base operator++(int) {
            assert(p_ptr != nullptr && "Attempting to increment an empty iterator");
            list_iterator_base old = *this;
            p_ptr = p_ptr->next;
            return old;
        }

        list_iterator_base& operator--() {
            assert(p_ptr != nullptr && "Attempting to increment an empty iterator");
            p_ptr = p_ptr->prev;
            return *this;
        }

        list_iterator_base operator--(int) {
            assert(p_ptr != nullptr && "Attempting to increment an empty iterator");
            list_iterator_base old = *this;
            p_ptr = p_ptr->prev;
            return old;
        }

        reference operator*() const {
            assert(p_ptr != nullptr && "Dereferencing an empty iterator.");
            return p_ptr->data;
        }

        pointer operator->() const {
            assert(p_ptr != nullptr && "Dereferencing an empty iterator.");
            return &(p_ptr->data);
        }

        bool operator==(const list_iterator_base& other) const noexcept {
            return p_ptr == other.p_ptr;
        }

        bool operator!=(const list_iterator_base& other) const noexcept {
            return p_ptr != other.p_ptr;
        }

    private:
        /**
         * \brief Retrieves the pointer to the current node.
         *
         * \return A pointer to the node.
         *
         * \note This function is private and can be use only within the `list` class.
         */
        node_pointer get_node() const noexcept { return p_ptr; }

    private:
        node_pointer p_ptr;
    };


/* Constructor and Destructor*/
public:
    /**
     * \brief Default Constructor
     */
    list() 
        : m_size(0), p_end(new node)
    {
        init_sentinel_node();
    }

    /**
     * \brief Constructs the container with count copies of elements with value value.
     *
     * \param count: The number of elements to insert.
     * \param value: The value of the elements to insert. Defaults to a default-constructed value.
     */
    explicit list(size_type count, const_reference value = value_type()) 
        : m_size(0), p_end(new node)
    {
        init_sentinel_node();
        insert(cbegin(), count, value);
    }

    /**
     * \brief Constructs the container with the contents of the range `[first, last)`.
     *
     * \note The `InputIt` must satisfy at least the requirements of `std::input_iterator`.
     *
     * \param first: An iterator to the beginning of the range to copy.
     * \param last: An iterator to the end of the range to copy.
     */
    template <std::input_iterator InputIt>
    list(InputIt first, InputIt last) 
        : m_size(0), p_end(new node)
    {
        init_sentinel_node();
        insert(cbegin(), first, last);
    }

    /**
     * \brief Copy constructor.
     *
     * \param other: Another list to copy the elements from.
     */
    list(const list& other) 
        : m_size(0), p_end(new node)
    {
        init_sentinel_node();
        insert(cbegin(), other.cbegin(), other.cend());
    }

    /**
     * \brief Move constructor
     *
     * Constructs the container by acquiring the contents of another list using move semantics. 
     * The `other` list is left in a valid but unspecified state.
     *
     * \param other: Another list to move data from. This list will be empty after the move.
     */
    list(list&& other) noexcept
        : m_size(0), p_end(new node)
    {
        init_sentinel_node();
        std::swap(p_end, other.p_end);
        std::swap(m_size, other.m_size);
    }

    /**
     * \brief Constructs the container with the contents of the initializer list `init`.
     *
     * \param ilist: An initializer list containing elements to be added to the list.
     */
    list(std::initializer_list<value_type> ilist)
        : m_size(0), p_end(new node) 
    {
        init_sentinel_node();
        insert(cbegin(), ilist);
    }

    /**
     * \brief Destructor.
     */
    ~list() {
        clear();
        delete p_end;
    }


/* Operators */
public:
    /**
     * \brief Copy assignment operator
     *
     * \param other: A reference to the list from which to copy the elements.
     * \return A reference to this list (*this), allowing for chained assignments.
     */
    list& operator=(const list& other) {
        if (this != &other) {
            // 
            clear();
            m_size = 0;

            // 
            insert(cbegin(), other.cbegin(), other.cend());
        }
        return *this;
    }

    /**
     * \brief Move assignment operator
     *
     * \param other: An rvalue reference to another list to move from.
     * \return A reference to this list (*this), allowing for chained assignments.
     */
    list& operator=(list&& other) noexcept {
        if (this != &other) {
            // 
            clear();
            m_size = 0;

            // 
            std::swap(p_end, other.p_end);
            std::swap(m_size, other.m_size);
        }
        return *this;
    }

    /**
     * \brief initializer list assign operator
     *
     * \param ilist: An initializer list containing elements to assign to this list.
     * \return A reference to this list (*this), allowing for chained assignments.
     */
    list& operator=(std::initializer_list<value_type> ilist) {
        // 
        clear();
        m_size = 0;

        // 
        insert(cbegin(), ilist);

        // 
        return *this;
    }


/* Element access */
public:
    /**
     * \brief Access the first element.
     */
    reference front() { 
        if (p_end->next == p_end)
            throw std::out_of_range("front(): list is empty");
        return p_end->next->data;
    }
    const_reference front() const {
        if (p_end->next == p_end)
            throw std::out_of_range("front(): list is empty");
        return p_end->next->data; 
    }

    /**
     * \brief Access the last element.
     */
    reference back() {
        if (p_end->prev == p_end)
            throw std::out_of_range("front(): list is empty");
        return p_end->prev->data;
    }
    const_reference back() const {
        if (p_end->prev == p_end)
            throw std::out_of_range("front(): list is empty");
        return p_end->prev->data;
    }


/* Capacity */
public:
    /**
     * \brief Return the number of elements.
     *
     * \note The std::list does not have `size()`. I implement this
     * method for testing and debugging.
     */
    size_type size() const noexcept { return m_size; }

    /**
     * \brief Checks whether the container is empty
     */
    bool empty() const noexcept { return p_end->next == p_end; }


/* Iterators */
public:
    /**
     * \brief Returns an iterator to the beginning
     */
    iterator        begin()       noexcept { return iterator(p_end->next); }
    const_iterator cbegin() const noexcept { return const_iterator(p_end->next); }

    /**
     * \brief Returns an iterator to the end
     */
    iterator        end()       noexcept { return iterator(p_end); }
    const_iterator cend() const noexcept { return const_iterator(p_end); }

    /**
     * \brief Returns a reverse iterator to the beginning
     */
    reverse_iterator        rbegin()       noexcept { return reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    /**
     * \brief Returns a reverse iterator to the end
     */
    reverse_iterator        rend()       noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }


/* Modifiers */
public:
    /**
     * \brief clears all elements.
     */
    void clear() noexcept {
        node_pointer curr = p_end->next;
        while (curr != p_end) {
            node_pointer next = curr->next;
            delete curr;
            curr = next;
        }
        p_end->prev = p_end;
        p_end->next = p_end;
        m_size = 0;
    }

    /**
     * \brief Inserts `value` before `pos`.
     *
     * \param pos: A const iterator specifying the position before which the new value will be inserted.
     * \param value: The value to be inserted.
     * \return An iterator pointing to the newly inserted element.
     */
    iterator insert(const_iterator pos, const_reference value) {
        // 
        node_pointer curr = pos.get_node();
        node_pointer prev = curr->prev;
        node_pointer newNode = new node(value, prev, curr);

        // 
        prev->next = newNode;
        curr->prev = newNode;
        ++m_size;

        // 
        return iterator(newNode);
    }


    /**
     * \brief Inserts `value` before `pos`.
     *
     * \param pos: A const iterator specifying the position before which the new value will be inserted.
     * \param value: The value to be inserted (by std::move).
     * \return An iterator pointing to the newly inserted element.
     */
    iterator insert(const_iterator pos, value_type&& value) {
        // 
        node_pointer curr = pos.get_node();
        node_pointer prev = curr->prev;
        node_pointer newNode = new node(std::move(value), prev, curr);

        // 
        prev->next = newNode;
        curr->prev = newNode;
        ++m_size;

        // 
        return iterator(newNode);
    }

    /**
     * \brief Inserts `count` copies of `value` before the position specified by `pos`.
     *
     * \param pos: A const iterator specifying the position before which the new values will be inserted.
     * \param count: The number of copies of `value` to insert.
     * \param value: The value to be inserted.
     * \return  An iterator pointing to the first element inserted, or `pos` if `count` is zero.     
     */
    iterator insert(const_iterator pos, size_type count, const_reference value) {
        // 
        if (count == 0)
            return iterator(pos.get_node());   // convert to non-const iterator

        // 
        iterator it, firstIt;
        for (size_type i = 0; i < count; ++i) {
            it = insert(pos, value);
            if (i == 0)
                firstIt = it;
        }

        return firstIt;
    }

    /**
     * \brief Inserts elements from the range [first, last) before the position specified by `pos`.
     *
     * \note The `InputIt` must satisfy at least the requirements of `std::input_iterator`.
     *
     * \param pos: An iterator specifying the position before which the elements will be inserted.     
     * \param first: An iterator to the beginning of the range to be inserted.
     * \param last: An iterator to the end of the range to be inserted (not inclusive).
     * \return An iterator pointing to the first element inserted, or `pos` if the range is empty.
     */
    template <std::input_iterator InputIt>
    iterator insert(const_iterator pos, InputIt first, InputIt last) {
        // 
        iterator first_inserted = insert(pos, *first);
        ++first;

        // 
        for (auto it = first; it != last; ++it) {
            insert(pos, *it);
        }

        return iterator(first_inserted.get_node());
    }

    /**
     * \brief Inserts elements from an initializer list before the specified position `pos`.
     *
     * /param pos: An iterator specifying the position before which the elements will be inserted.
     * /param ilist: An initializer list of elements to be inserted.
     * /return An iterator pointing to the first element inserted.
     */
    iterator insert(const_iterator pos, std::initializer_list<value_type> ilist) {
        return insert(pos, ilist.begin(), ilist.end());
    }


    /**
     * \brief Constructs an element in-place at the specified position in the list.
     *
     * \param pos: The position before which the new element will be inserted.
     * \param args: Variable number of arguments used to construct the new element. 
     *              These arguments are forwarded to the constructor of the element
     *              type, allowing for in-place construction.
     * \return An iterator pointing to the newly inserted element.
     */
    template <typename... Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        // 
        node_pointer curr = pos.get_node();
        node_pointer prev = curr->prev;
        node_pointer newNode = new node(std::forward<Args>(args)...);

        // 
        newNode->prev = prev;
        newNode->next = curr;
        prev->next = newNode;
        curr->prev = newNode;
        ++m_size;

        // 
        return iterator(newNode);
    }


    /**
     * \brief Removes the element at pos.
     *
     * \param pos: An iterator to the element to be erased.
     * \return An iterator pointing to the element after the erased element.
     * \throws std::out_of_range: Thrown if an attempt to erase the end iterator.
     */
    iterator erase(const_iterator pos) {
        // 
        if (pos == cend())
            throw std::out_of_range("erase(): Attempt to erase end iterator");

        // 
        node_pointer node2delete = pos.get_node();
        node_pointer prev = node2delete->prev;

        prev->next = node2delete->next;
        prev->next->prev = prev;
        delete node2delete;
        --m_size;

        // 
        return iterator(prev->next);
    }


    /**
     * \brief Removes the elements in [first, last).
     *
     * \param first An iterator to the first element in the range to be erased.
     * \param last An iterator to the element immediately following the last element to be erased.
     * \return An iterator pointing to the element specified by `last`.
     */
    iterator erase(const_iterator first, const_iterator last) {
        // 
        iterator it;
        while (first != last) {
            it = erase(first);
            first = const_iterator(it.get_node());
        }
        return iterator(last.get_node());
    }


    /**
     * \brief adds an element to the end
     *
     * \param value: The value of the element to be added to the end of the list.
     */
    void push_back(const_reference value) {
        node_pointer newNode = new node(value, p_end->prev, p_end);
        if (p_end->prev == p_end)
            p_end->next = newNode;   // list is empty
        else
            p_end->prev->next = newNode;
        p_end->prev = newNode;
        ++m_size;
    }


    /**
     * \brief adds an element to the end
     *
     * \param value: The value of the element to be added to the end of the list (by std::move()).
     */
    void push_back(value_type&& value) {
        node_pointer newNode = new node(std::move(value), p_end->prev, p_end);
        if (p_end->prev == p_end)
            p_end->next = newNode;   // list is empty
        else
            p_end->prev->next = newNode;
        p_end->prev = newNode;
        ++m_size;
    }


    /**
     * \brief Constructs an element in-place at the end of the list.
     *
     * \param args: Variable number of arguments used to construct the new
     * element. These arguments are forwarded to the constructor of the element
     * type, allowing for in-place construction.
     */
    template <typename... Args>
    void emplace_back(Args&&... args) {
        node_pointer newNode = new node(std::forward<Args>(args)...);
        newNode->prev = p_end->prev;
        newNode->next = p_end;
        if (p_end->prev == p_end)
            p_end->next = newNode;   // list is empty
        else
            p_end->prev->next = newNode;
        p_end->prev = newNode;
        ++m_size;
    }


    /**
     * \brief Removes the last element.
     *
     * \throws std::out_of_range if the list is empty, with the message "pop_back(): List is empty".
     */
    void pop_back() {
        // 
        if (p_end->next == p_end)
            throw std::out_of_range("pop_front(): List is empty");

        // 
        node_pointer node_2_delete = p_end->prev;
        p_end->prev = node_2_delete->prev;
        p_end->prev->next = p_end;

        // 
        --m_size;
    }


    /**
     * \brief inserts an element to the beginning
     *
     * \param value: The value of the element to be added to the beginning of the list.
     */
    void push_front(const_reference value) {
        node_pointer newNode = new node(value, p_end, p_end->next);
        if (m_size == 0)
            p_end->prev = newNode;
        else
            p_end->next->prev = newNode;
        p_end->next = newNode;
        ++m_size;
    }


    /**
     * \brief inserts an element to the beginning
     *
     * \param value: The value of the element to be added to the beginning of the list (by std::move).
     */
    void push_front(value_type&& value) {
        node_pointer newNode = new node(std::move(value), p_end, p_end->next);
        if (m_size == 0)
            p_end->prev = newNode;
        else
            p_end->next->prev = newNode;
        p_end->next = newNode;
        ++m_size;
    }


    /**
     * \brief Constructs an element in-place at the beginning of the list.
     *
     * \param args: Variable number of arguments used to construct the new
     * element. These arguments are forwarded to the constructor of the element
     * type, allowing for in-place construction.
     */
    template <typename... Args>
    void emplace_front(Args&&... args) {
        node_pointer newNode = new node(std::forward<Args>(args)...);
        newNode->prev = p_end;
        newNode->next = p_end->next;
        if (m_size == 0)
            p_end->prev = newNode;
        else
            p_end->next->prev = newNode;
        p_end->next = newNode;
        ++m_size;
    }


    /**
     * \brief Removes the first element
     *
     * \throws std::out_of_range if the list is empty, with the message "pop_back(): List is empty".
     */
    void pop_front() {
        // 
        if (p_end->next == p_end)
            throw std::out_of_range("pop_front(): List is empty");

        // 
        node_pointer node_2_delete = p_end->next;
        p_end->next = p_end->next->next;
        p_end->next->prev = p_end;
        --m_size;

        // 
        delete node_2_delete;
    }


    /**
     * \brief changes the number of elements stored
     *
     * \param count The new size of the list.
     * \param value The value used to initialize new elements when the list size is increased.
     *              Defaults to a default-constructed element of the list's value type.
     */
    void resize(size_type count, const_reference value = value_type()) {
        if (count > m_size) {
            size_type num2add = count - m_size;
            for (size_type i = 0; i < num2add; ++i) {
                push_back(value);
            }
        }
        else if (count < m_size) {
            const_iterator it = cbegin();
            std::advance(it, count);
            erase(it, cend());
        }
    }


    /**
     * \brief swaps the contents
     */
    void swap(list& other) {
        std::swap(m_size, other.m_size);
        std::swap(p_end, other.p_end);
    }


/* Operations */
public:
    /**
     * \brief Merge two sorted list
     *
     * \note No iterators or references become invalidated.
     * \note Undefined behavior if the `this` or `other` is not sorted
     */
    void merge(list& other) {
        // This function does nothing if `other` refers to the same object as `this`
        if (this == &other)
            return;

        // 
        node_pointer curr1 = p_end->next;
        node_pointer curr2 = other.p_end->next;
        node_pointer tail = p_end;

        // 
        while (curr1 != p_end && curr2 != other.p_end) {
            if (curr1->data < curr2->data) {
                tail = curr1;
                curr1 = curr1->next;
            }
            else {
                // 
                node_pointer next = curr2->next;

                // insert
                tail->next = curr2;
                curr2->prev = tail;
                curr2->next = curr1;
                curr1->prev = curr2;

                // traverse
                curr2 = next;
                next->prev = other.p_end;

                // update tail
                tail = tail->next;
            }
        }

        // remaining node
        if (curr2 != other.p_end) {
            // 
            tail->next = curr2;
            curr2->prev = tail;

            // 
            node_pointer otherTail = other.p_end->prev;
            otherTail->next = p_end;
            p_end->prev = otherTail;        
        }

        // 
        m_size += other.m_size;

        // 
        other.m_size = 0;
        other.init_sentinel_node();
    }


    /**
     * \brief Moves elements from another list
     *
     * \param pos: element after which the content will be inserted.
     * \param other: anothe rcontainer to move the content from
     *
     * \note No iterators or references become invalidated.
     */
    void splice(const_iterator pos, list& other) {
        // 
        if (other.m_size == 0) 
            return;

        node_pointer curr = pos.get_node();
        node_pointer next = curr->next;

        // 
        curr->next = other.p_end->next;
        other.p_end->next->prev = curr;

        // 
        next->prev = other.p_end->prev;
        other.p_end->prev->next = next;

        // 
        m_size += other.m_size;

        // 
        other.m_size = 0;
        other.init_sentinel_node();
    }


    /**
     * \brief Removes elements satisfying specific criteria
     */
    void remove(const_reference value) {
        // 
        node_pointer curr = p_end->next;

        // 
        while (curr != p_end) {
            node_pointer next = curr->next;
            if (curr->data == value) {
                curr->prev->next = curr->next;
                curr->next->prev = curr->prev;
                delete curr;
                m_size--;
            }
            curr = next;
        }
    }


    /**
     * \brief Reverses the order of the elements
     */
    void reverse() noexcept {
        // 
        if (m_size < 1)
            return;

        // 
        node_pointer current = p_end->next;

        do {
            std::swap(current->prev, current->next);
            current = current->prev;
        } while (current != p_end);

        // 
        std::swap(p_end->prev, p_end->next);
    }


    /**
     * \brief Removes consecutive duplicate elements
     */
    void unique() {
        // 
        if (m_size < 2) {
            return;
        }

        // 
        node_pointer curr = p_end->next;
        while (curr != p_end && curr->next != p_end) {
            node_pointer next = curr->next;
            if (curr->data == next->data) {
                next->next->prev = curr;
                curr->next = next->next;
                delete next;
                m_size--;
            } else {
                curr = next;
            }
        }
    }


    /**
     * \brief Sorts the elements
     */
    void sort() {
        // 
        if (empty() || p_end->next->next == p_end)
            return;

        // break the connection of the sentinel for using merge sort easily
        node_pointer unsorted_head = p_end->next;
        p_end->prev->next = nullptr;
        p_end->next->prev = nullptr;
        p_end->next       = nullptr;
        p_end->prev       = nullptr;

        // merge sort
        node_pointer sorted_head = merge_sort(unsorted_head);
        node_pointer sorted_tail = sorted_head;
        while (sorted_tail->next != nullptr)
            sorted_tail = sorted_tail->next;

        // reconnect nodes to `p_end`
        p_end->next = sorted_head;
        sorted_head->prev = p_end;
        p_end->prev = sorted_tail;
        sorted_tail->next = p_end;
    }


private:
    /**
     * \brief Initializes the sentinel node used in the list.
     */
    void init_sentinel_node() noexcept {
        p_end->prev = p_end;
        p_end->next = p_end;
    }


    /**
     * \brief Get middle node for merge sort
     */
    node_pointer get_middle_node(node_pointer head) {
        // 
        if (head == nullptr || head->next == nullptr)
            return head;

        // 
        node_pointer slow = head;
        node_pointer fast = head;

        // 
        while (fast->next != nullptr && fast->next->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
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
        if (head1 == nullptr)
            return head2;
        if (head2 == nullptr)
            return head1;

        // 
        node_pointer dummy = new node(0);
        node_pointer tail = dummy;

        // 
        while (head1 != nullptr && head2 != nullptr) {
            if (head1->data < head2->data) {
                tail->next = head1;
                head1->prev = tail;
                head1 = head1->next;
            } else {
                tail->next = head2;
                head2->prev = tail;
                head2 = head2->next;
            }
            tail = tail->next;
        }

        // 
        if (head1 != nullptr) {
            tail->next = head1;
            head1->prev = tail;
        }
        if (head2 != nullptr) {
            tail->next = head2;
            head2->prev = tail;
        }

        // 
        node_pointer sorted_head = dummy->next;
        if (sorted_head)
            sorted_head->prev = nullptr;
        delete dummy;
        return sorted_head;
    }


    /**
     * \brief Merge sort
     */
    node_pointer merge_sort(node_pointer head) {
        // 
        if (head == nullptr || head->next == nullptr)
            return head;

        // 
        node_pointer mid = get_middle_node(head);
        node_pointer mid_next = mid ? mid->next : nullptr;
        mid_next->prev = nullptr;
        mid->next = nullptr;

        // 
        if (mid != nullptr && mid->next != nullptr) {
            mid->next->prev = nullptr;
            mid->next = nullptr;
        }

        // 
        node_pointer left_part = merge_sort(head);
        node_pointer right_part = merge_sort(mid_next);

        // 
        return merge(left_part, right_part);
    }


/**/
private:
    size_type    m_size;
    node_pointer p_end;
};


/**
 * \brief Specializes the std::swap algorithm for mystl::list.
 */
template <typename T>
void swap(list<T>& lhs, list<T>& rhs) {
    lhs.swap(rhs);
}


} // namespace mystl::


#endif // LIST_HPP_
