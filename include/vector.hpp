/**
 * \file vector.hpp
 *
 * \reference:
 * - The Cherno: VECTOR/DYNAMIC ARRAY - Making DATA STRUCTURES in C++
 *          url: https://youtu.be/ryRf4Jh_YC0?si=9gncPajBHf4uIz-T
 * - The Cherno: Writing an ITERATOR in C++
 *          url: https://youtu.be/F9eDv-YIOQ0?si=BHQ1Nyq6Jo0cUd1R
 * - StackExchange: Basic iterator supporting vector implementation
 *             url: https://codereview.stackexchange.com/questions/202157/basic-iterator-supporting-vector-implementation
 */

#pragma once

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstddef>          // size_t
#include <utility>          // move, forward
#include <initializer_list> // initializer_list
#include <stdexcept>        // out_of_range
#include <iterator>         // random_access_iterator_tag, distance
#include <memory>           // allocator


namespace mystl {


/**
 * \class vector
 *
 * A template class of a simplified version of the std::vector,
 * providing dynamic array functionality with the ability to resize.
 */
template <typename _T, typename _Allocator = std::allocator<_T>>
class vector {
private:
    template <typename _Iter_val, typename _Iter_ptr, typename _Iter_ref>
    class vector_iterator_base;

public:
    using value_type             = _T;
    using allocator_type         = _Allocator;
    using size_type              = std::size_t;
    using pointer                = _T*;
    using reference              = _T&;
    using const_pointer          = const _T*;
    using const_reference        = const _T&;
    using difference_type        = std::ptrdiff_t;
    using iterator               = vector_iterator_base<value_type, pointer, reference>;
    using const_iterator         = vector_iterator_base<value_type, const_pointer, const_reference>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    static constexpr size_type DEFAULT_CAPACITY = 10;
    static constexpr size_type REALLOC_RATE     = 2;

private:
    /**
     * \brief Iterator for the mystl::vector class template that supports random access iterator operations.
     *
     * \tparam _Iter_val Type of the value that the iterator points to.
     * \tparam _Iter_ptr Type of the pointer to the value (const or non-const).
     * \tparam _Iter_ref Type of the reference to the value (const or non-const).
     *
     * \ref: StackExchange: Basic iterator supporting vector implementation
     *                 url: https://codereview.stackexchange.com/questions/202157/basic-iterator-supporting-vector-implementation
     */
    template <typename _Iter_val, typename _Iter_ptr, typename _Iter_ref>
    class vector_iterator_base {
    public:
        using value_type        = _Iter_val;
        using pointer           = _Iter_ptr;
        using reference         = _Iter_ref;
        using difference_type   = std::ptrdiff_t;
        using iterator_category = std::random_access_iterator_tag;   // C++ named requirements: LegacyRandomAccessIterator

    public:
        vector_iterator_base() : p_ptr(nullptr) {}
        vector_iterator_base(pointer ptr) : p_ptr(ptr) {}

    public:
        reference operator*()           const { return *p_ptr; }
        pointer   operator->()          const { return p_ptr; }
        reference operator[](int index) const { return *(p_ptr + index); }

        vector_iterator_base& operator+=(difference_type n) { p_ptr += n; return *this; }
        vector_iterator_base& operator-=(difference_type n) { p_ptr -= n; return *this; }

        vector_iterator_base& operator++()    { ++p_ptr; return *this; }
        vector_iterator_base& operator--()    { --p_ptr; return *this; }
        vector_iterator_base  operator++(int) { vector_iterator_base old(*this); ++(*this); return old; }
        vector_iterator_base  operator--(int) { vector_iterator_base old(*this); --(*this); return old; }

        vector_iterator_base operator+(difference_type n) const { vector_iterator_base temp(*this); return temp += n; }
        vector_iterator_base operator-(difference_type n) const { vector_iterator_base temp(*this); return temp -= n; }

        difference_type operator-(const vector_iterator_base& other) const { return p_ptr - other.p_ptr; }

        bool operator<(const vector_iterator_base& other)  const { return p_ptr <  other.p_ptr; }
        bool operator<=(const vector_iterator_base& other) const { return p_ptr <= other.p_ptr; }
        bool operator>(const vector_iterator_base& other)  const { return p_ptr >  other.p_ptr; }
        bool operator>=(const vector_iterator_base& other) const { return p_ptr >= other.p_ptr; }
        bool operator==(const vector_iterator_base& other) const { return p_ptr == other.p_ptr; }
        bool operator!=(const vector_iterator_base& other) const { return p_ptr != other.p_ptr; }

        pointer base() const { return p_ptr; }

    private:
        pointer p_ptr;
    };

    friend iterator operator+(difference_type n, const iterator& it) { return it + n; }
    friend iterator operator-(difference_type n, const iterator& it) { return it + n; }


public:
    /**
     * \brief Construct new vector with default initial capacity.
     *
     * \param alloc: allocator to use for all memory allocations of this container.
     */
    vector(const allocator_type& alloc = allocator_type()) 
        : m_alloc(alloc), m_size(0), m_capacity(0), 
          p_elem(nullptr)
    {
        realloc(DEFAULT_CAPACITY);
    }


    /**
     * \brief Constructs the container with count copies of elements with value. 
     *
     * \param count: The number of elements to construct.
     * \param value: The value to initialize the elements.
     * \param alloc: allocator to use for all memory allocations of this container.
     */
    vector(size_type count, const_reference value, const allocator_type& alloc = allocator_type()) 
        : m_alloc(alloc), m_size(0), m_capacity(0), p_elem(nullptr)
    {
        resize(count, value);
    }


    /**
     * \brief Constructs the container with count copies of elements with default-constructed value. 
     *
     * \param count: The number of elements to construct.
     * \param alloc: allocator to use for all memory allocations of this container.
     */
    explicit vector(size_type count, const allocator_type& alloc = allocator_type()) 
        : m_alloc(alloc), m_size(0), m_capacity(0), p_elem(nullptr)
    {
        resize(count, value_type());
    }


    /**
     * \brief Constructs the container with the contents of the range [first,
     * last).
     *
     * \note The `InputIt` must satisfy at least the requirements of `std::input_iterator`.
     *
     * \param first, last: iterators defining the range to be copied into the
     * vector, the `first` must be smaller than `last`.
     * \param alloc: allocator to use for all memory allocations of this container.
     */
    template <std::input_iterator InputIt>
    vector(InputIt first, InputIt last, const allocator_type& alloc = allocator_type())
        : m_alloc(alloc), 
          m_size(std::distance(first, last)),
          m_capacity(m_size),
          p_elem(std::allocator_traits<allocator_type>::allocate(m_alloc, m_capacity))
    {
        // 
        size_type i = 0;
        for (InputIt it = first; it != last; ++it, ++i) {
            std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + i, *it);
        }
    }


    /**
     * \brief Copy constructor
     */
    vector(const vector<value_type>& other)
        : m_alloc(other.m_alloc), m_size(other.m_size), m_capacity(other.m_capacity), p_elem(nullptr)
    {
        if (other.p_elem != nullptr) {
            p_elem = std::allocator_traits<allocator_type>::allocate(m_alloc, m_capacity);
            for (size_type i = 0; i < m_size; ++i)
                std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + i, other.p_elem[i]);
        }
    }


    /**
     * \brief Move constructor
     */
    vector(vector<value_type>&& other) noexcept
        : m_alloc(other.m_alloc), m_size(other.m_size), m_capacity(other.m_capacity), p_elem(other.p_elem)
    {
        other.m_size = 0;
        other.m_capacity = 0;
        other.p_elem = nullptr;
    }


    /**
     * \brief Construct by initializer.
     *
     * \param initList: initializer list to initialize the elements of the container with.
     * \param alloc: allocator to use for all memory allocations of this container.
     */
    vector(std::initializer_list<value_type> initList, const allocator_type& alloc = allocator_type())
        : m_alloc(alloc), m_size(initList.size()), m_capacity(m_size),
          p_elem(std::allocator_traits<allocator_type>::allocate(m_alloc, m_capacity))
    {
        size_type i = 0;
        for (auto& elem : initList) {
            std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + i, elem);
            ++i;
        }
    }


    /**
     * \brief Destructor for the vector.
     * 
     * Destroys all elements in the vector and deallocates the memory block
     * allocated for the vector's storage.
     */
    ~vector() {
        clear();
        destroy_vector();
    }

/* Operators */
public:
    /**
     * \brief Access specified element
     */
    reference operator[](size_type index) { return p_elem[index]; }
    const_reference operator[](size_type index) const { return p_elem[index]; }

    /**
     * \brief Copy assign operator
     */
    vector& operator=(const vector& other) {
        if (this != &other) {
            // 
            m_alloc = other.m_alloc;

            // 
            if (m_capacity < other.m_capacity) {
                // old capaacity is not enough
                clear();
                destroy_vector();
                m_capacity = other.m_capacity;
                p_elem = std::allocator_traits<allocator_type>::allocate(m_alloc, m_capacity);
            }
            else {
                // reuse old memory space, clear without destroy
                clear();
            }

            // 
            m_size = other.m_size;
            for (size_type i = 0; i < other.m_size; ++i) 
                std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + i, other.p_elem[i]);
        }
        return *this;
    }

    /**
     * \brief Move assign operator
     */
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
            // 
            m_alloc = other.m_alloc;

            // clear and delete current elements
            clear();
            destroy_vector();

            // transfer ownership
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            p_elem = other.p_elem;

            // Reset the source object
            other.m_size = 0;
            other.m_capacity = 0;
            other.p_elem = nullptr;
        }
        return *this;
    }


/* Element Access */
public:
    /**
     * \brief Access specified element with bounds checking
     *
     * Returns a reference to the element at specified location pos, with 
     * bounds checking. If pos is not within the range of the container, an 
     * exception of type std::out_of_range is thrown.
     *
     * \ref cppreference.com
     */
    reference at(size_type pos) {
        if (pos >= m_size)
            throw std::out_of_range("vector::at");
        return p_elem[pos];
    }

    const_reference at(size_type pos) const {
        if (pos >= m_size)
            throw std::out_of_range("vector::at");
        return p_elem[pos];
    }

    /**
     * \brief Access the first element
     *
     * \throws std::out_of_range if the container is empty.
     */
    reference front() { 
        if (empty())
            throw std::out_of_range("front(): list is empty");
        return p_elem[0]; 
    }

    /**
     * \brief Access the first element
     *
     * \throws std::out_of_range if the container is empty.
     */
    const_reference front() const { 
        if (empty())
            throw std::out_of_range("front(): list is empty");
        return p_elem[0]; 
    }

    /**
     * \brief Access the last elemet
     *
     * \throws std::out_of_range if the container is empty.
     */
    reference back() { 
        if (empty())
            throw std::out_of_range("back(): list is empty");
        return p_elem[m_size-1];
    }

    /**
     * \brief Access the last elemet
     *
     * \throws std::out_of_range if the container is empty.
     */
    const_reference back() const { 
        if (empty())
            throw std::out_of_range("back(): list is empty");
        return p_elem[m_size-1];
    }

    /**
     * \brief Direct access to the underlying contiguous storage
     */
    pointer data() noexcept { return p_elem; }
    const_pointer data() const noexcept { return p_elem; }


/* Iterators */
public:
    /**
     */
    iterator                 begin()       noexcept { return iterator(p_elem); }
    const_iterator          cbegin() const noexcept { return const_iterator(p_elem); }
    reverse_iterator        rbegin()       noexcept { return reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    /**
     */
    iterator                 end()       noexcept { return iterator(p_elem + m_size); }
    const_iterator          cend() const noexcept { return const_iterator(p_elem + m_size); }
    reverse_iterator        rend()       noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }


/* Capacity */
public:
    /**
     * \brief Return size.
     */
    size_type size() const { return m_size; }

    /**
     * \brief Return size of allocated storage capacity.
     */
    size_type capacity() const { return m_capacity; }

    /**
     * \brief Test whether vector is empty.
     */
    bool empty() const { return m_size == 0; }

    /**
     * \brief Reserves storage
     */
    void reserve(size_type newCapacity) {
        if (m_capacity < newCapacity)
            realloc(newCapacity);
    }

    /**
     * \brief Reduces memory usage by freeing unused memory
     */
    void shrink_to_fit() {
        if (m_capacity > m_size) {
            realloc(m_size);
        }
    }


/* Modifiers */
public:
    /**
     * \brief Clear all contents in vector.
     *
     * Use allocator::destroy to destruct all elements in the vector, and set 
     * the vector's size to 0.
     */
    void clear() {
        for (size_type i = 0; i < m_size; ++i) {
            std::allocator_traits<allocator_type>::destroy(m_alloc, p_elem + i);
        }
        m_size = 0;
    }


    /**
     * \brief Erases elements
     *
     * \param pos: iterator to the element to remove (use const_iterator since c++11)
     *
     * \return The element after the deleted element.
     */
    iterator erase(const_iterator pos) {
        // 
        if (pos < cbegin() || pos > cbegin())
            throw std::out_of_range("vector::erase() - Iterator out of range");

        // Convert const_iterator to iterator
        difference_type dist = std::distance(cbegin(), pos);
        iterator nonConstPos = begin() + dist;

        // Move elements
        iterator it;
        for (it = nonConstPos; (it + 1) != end(); ++it) {
            *it = std::move(*(it + 1));
        }

        // Destroy element
        std::allocator_traits<allocator_type>::destroy(m_alloc, it.base());
        m_size--;

        // 
        return it;
    }


    /**
     * \brief Erases elements
     *
     * \param first, last: range of elements to remove (use const_iterator since c++11)
     *
     * \return The element after the deleted element.
     */
    iterator erase(const_iterator first, const_iterator last) {
        // 
        if (first < cbegin() || last > cend())
            throw std::out_of_range("vector::erase() - Iterator out of range");

        // Convert const_iterator to iterator
        difference_type dist2first = std::distance(cbegin(), first);
        difference_type dist2last = std::distance(cbegin(), last);
        iterator nonConstFirst = begin() + dist2first;
        iterator nonConstLast = begin() + dist2last;

        // 
        if (first == last)
            return nonConstLast;

        // Move elements
        iterator itLeft = nonConstFirst, itRight = nonConstLast;
        while (itRight != end()) {
            *itLeft = std::move(*itRight);
            itLeft++;
            itRight++;
        }

        // destroy elements
        iterator newEnd = end() - (dist2last - dist2first);
        while (itLeft != newEnd) {
            std::allocator_traits<allocator_type>::destroy(m_alloc, itLeft.base());
            itLeft++;
        }

        // 
        m_size -= (dist2last - dist2first);

        return nonConstFirst;
    }


    /**
     * \brief Construct a new element in-place before the pos
     *
     * \param pos: the new element will be constructed before pos
     * \param args: arguments to forward to the constructor of the element
     *
     * \return iterator pointing to the emplaced element.
     */
    template <typename ...Args>
    iterator emplace(const_iterator pos, Args&&... args) {
        // compute the `tarIndex` first because the iterator `pos` will be invalidate after using `realloc`
        size_type tarIndex = pos - cbegin();

        // check capacity
        if (m_size >= m_capacity)
            realloc(m_capacity == 0 ? 1 : REALLOC_RATE * m_capacity);

        // shift elements after pos
        for (size_type i = m_size; i > tarIndex; --i) {
            std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + i, std::move(p_elem[i-1]));
            std::allocator_traits<allocator_type>::destroy(m_alloc, p_elem + i - 1);
        }

        // construct
        std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + tarIndex, std::forward<Args>(args)...);
        ++m_size;

        // 
        return iterator(begin() + tarIndex);
    }


    /**
     * \brief inserts elements
     *
     * \param pos: the new element will be inserted before pos
     * \param value: element value to insert
     */
    iterator insert(const_iterator pos, const_reference value) {
        return emplace(pos, value);
    }

    iterator insert(const_iterator pos, value_type&& value) {
        return emplace(pos, std::move(value));
    }


    /**
     * \brief Insert at the end of the vector.
     */
    void push_back(const_reference value) {
        if (m_size >= m_capacity)
            realloc(m_capacity == 0 ? 1 : REALLOC_RATE * m_capacity);
        std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + m_size, value);
        m_size++;
    }


    /**
     * \brief Insert at the end of the vector.
     */
    void push_back(value_type&& value) {
        if (m_size >= m_capacity)
            realloc(m_capacity == 0 ? 1 : REALLOC_RATE * m_capacity);
        std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + m_size, std::move(value));
        m_size++;
    }


    /**
     * \brief Construct and insert at the end of the vector.
     */
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (m_size >= m_capacity)
            realloc(m_capacity == 0 ? 1 : REALLOC_RATE * m_capacity);
        std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + m_size, std::forward<Args>(args)...);
        return p_elem[m_size++];
    }


    /**
     * \brief Delete the last element.
     */
    void pop_back() {
        if (m_size > 0) {
            std::allocator_traits<allocator_type>::destroy(m_alloc, p_elem + m_size);
            --m_size;
        }
        else {
            throw std::length_error("vector::pop_back(): the vector is empty");
        }
    }


    /**
     * \brief Changes the number of elements stored
     *
     * \param count: new size of the container
     * \param value: the value to initialize the new elements with
     */
    void resize(size_type count, const_reference value = value_type()) {
        // 
        if (m_size > count) {
            erase(cbegin() + count, cend());
        }
        else {
            if (count > m_capacity) {
                reserve(count);
            }
            for (size_type i = m_size; i < count; ++i) {
                std::allocator_traits<allocator_type>::construct(m_alloc, p_elem + i, value);
            }
        }

        // 
        m_size = count;
    }


    /**
     * \brief swaps the contents
     *
     * \param other: other vector
     */
    void swap(vector& other) noexcept {
        std::swap(this->m_size, other.m_size);
        std::swap(this->m_capacity, other.m_capacity);
        std::swap(this->p_elem, other.p_elem);
    }

private:
    /**
     * \brief Reallocates the storage of the vector
     *
     * \param newCapacity
     */
    void realloc(size_type newCapacity) {
        // allocate a new block of memory
        pointer newBlock = std::allocator_traits<allocator_type>::allocate(m_alloc, newCapacity);

        // copy/move old elements into new block
        size_type elemNum = newCapacity > m_size ? m_size : newCapacity;
        for (size_type i = 0; i < elemNum; ++i) {
            std::allocator_traits<allocator_type>::construct(m_alloc, newBlock + i, std::move(p_elem[i]));
            std::allocator_traits<allocator_type>::destroy(m_alloc, p_elem + i);
        }

        // 
        destroy_vector();
        p_elem = newBlock;
        m_size = elemNum;
        m_capacity = newCapacity;
    }


    /**
     * \brief Deallocate all memory allocated in vector.
     *
     * Deallocate the memory block allocated for the vector's storage without
     * calling the contained elements' destructors.
     */
    void destroy_vector() {
        clear();
        std::allocator_traits<allocator_type>::deallocate(m_alloc, p_elem, m_capacity);
        p_elem = nullptr;
        m_capacity = 0;
    }


private:
    allocator_type m_alloc;
    size_type      m_size;
    size_type      m_capacity;
    pointer        p_elem;
};


/**
 * \brief Specializes the std::swap algorithm for std::vector.
 */
template <typename T>
void swap(vector<T>& lhs, vector<T>& rhs) noexcept {
    lhs.swap(rhs);
}


}


#endif // !VECTOR_HPP_
