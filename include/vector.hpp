/**
 * \file vector.hpp

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
#include <new>              // ::operator new, ::operator delete
#include <initializer_list> // initializer_list
#include <stdexcept>        // out_of_range
#include <iterator>         // random_access_iterator_tag, distance
#include <type_traits>      // enable_if, is_integral


namespace mystl {


/**
 * \class vector_iterator
 */
template <typename _vector>
class vector_iterator {
public:
    using value_type        = typename _vector::value_type;
    using pointer           = value_type*;
    using reference         = value_type&;
    using const_pointer     = const value_type*;
    using const_reference   = const value_type&;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;   // C++ named requirements: LegacyRandomAccessIterator

public:
    vector_iterator() : p_ptr(nullptr) {}
    vector_iterator(pointer ptr) : p_ptr(ptr) {}

public:
    reference       operator*()                 { return *p_ptr; }
    const_reference operator*()           const { return *p_ptr; }
    pointer         operator->()                { return p_ptr; }
    const_pointer   operator->()          const { return p_ptr; }
    reference       operator[](int index)       { return *(p_ptr + index); }
    const_reference operator[](int index) const { return *(p_ptr + index); }

    vector_iterator& operator+=(int n) { p_ptr += n; return *this; }
    vector_iterator& operator-=(int n) { p_ptr -= n; return *this; }
    vector_iterator& operator++()      { ++p_ptr; return *this; }
    vector_iterator& operator--()      { --p_ptr; return *this; }
    vector_iterator operator++(int) { 
        vector_iterator old(*this);   // remember original value
        ++(*this);                    // post-increment
        return old;                   // return original value
    }
    vector_iterator operator--(int) { 
        vector_iterator old(*this);   // remember original value
        --(*this);                    // post-decrement
        return old;                   // return original value
    }

    vector_iterator operator+(int n) const { vector_iterator temp(*this); return temp += n; }
    vector_iterator operator-(int n) const { vector_iterator temp(*this); return temp -= n; }

    difference_type operator-(const vector_iterator& other) const { return p_ptr - other.p_ptr; }

    bool operator<(const vector_iterator& other)  const { return p_ptr <  other.p_ptr; }
    bool operator<=(const vector_iterator& other) const { return p_ptr <= other.p_ptr; }
    bool operator>(const vector_iterator& other)  const { return p_ptr >  other.p_ptr; }
    bool operator>=(const vector_iterator& other) const { return p_ptr >= other.p_ptr; }
    bool operator==(const vector_iterator& other) const { return p_ptr == other.p_ptr; }
    bool operator!=(const vector_iterator& other) const { return p_ptr != other.p_ptr; }

private:
    pointer p_ptr;
};


/**
 * \class vector
 *
 * A template class of a simplified version of the std::vector,
 * providing dynamic array functionality with the ability to resize.
 */
template <typename _T>
class vector {
public:
    using value_type             = _T;
    using size_type              = std::size_t;
    using pointer                = _T*;
    using reference              = _T&;
    using const_pointer          = const _T*;
    using const_reference        = const _T&;
    using difference_type        = std::ptrdiff_t;
    using iterator               = vector_iterator<vector<value_type>>;
    using const_iterator         = vector_iterator<vector<const value_type>>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    static constexpr size_type DEFAULT_CAPACITY = 10;
    static constexpr size_type REALLOC_RATE     = 2;

public:
    /**
     * \brief Construct new vector with default initial capacity.
     */
    vector() 
        : m_size(0), m_capacity(DEFAULT_CAPACITY), 
          p_elem(static_cast<pointer>(::operator new(DEFAULT_CAPACITY * sizeof(value_type))))
    {
    }


    /**
     * \brief Constructs the container with count copies of elements with value. 
     *
     * If `value` is not specified, the elements will initializ to their default-constructed values.
     *
     * \param count: The number of elements to construct.
     * \param value: The value to initialize the elements, default is a default-constructed of value_type.
     */
    explicit vector(size_type count, const_reference value = value_type()) {
        resize(count, value);
    }


    /**
     * \brief Constructs the container with the contents of the range [first,
     * last).
     *
     * \note Enabled only if InputIt is an iterator. (using SFINAE)
     *
     * \param first, last: iterators defining the range to be copied into the
     * vector, the `first` must be smaller than `last`
     */
    template <typename InputIt, 
              typename std::enable_if<!std::is_integral<InputIt>::value, InputIt>::type* = nullptr>
    vector(InputIt first, InputIt last)
        : m_size(last - first),
          m_capacity(m_size),
          p_elem(static_cast<pointer>(::operator new(m_capacity * sizeof(value_type))))
    {
        size_type i = 0;
        for (InputIt it = first; it != last; ++it, ++i) {
            new(&p_elem[i]) value_type(*it);
        }
    }


    /**
     * \brief Copy constructor
     */
    vector(const vector<value_type>& other)
        : m_size(other.m_size), m_capacity(other.m_capacity), p_elem(nullptr)
    {
        if (other.p_elem != nullptr) {
            p_elem = static_cast<pointer>(::operator new(m_capacity * sizeof(value_type)));
            for (size_type i = 0; i < m_size; ++i)
                new(&p_elem[i]) value_type(other.p_elem[i]);
        }
    }


    /**
     * \brief Move constructor
     */
    vector(vector<value_type>&& other) noexcept
        : m_size(other.m_size), m_capacity(other.m_capacity), p_elem(other.p_elem)
    {
        other.m_size = 0;
        other.m_capacity = 0;
        other.p_elem = nullptr;
    }


    /**
     * \brief Construct by initializer.
     */
    vector(std::initializer_list<value_type> initList) 
        : m_size(initList.size()), m_capacity(m_size),
          p_elem(static_cast<pointer>(::operator new(m_size * sizeof(value_type))))
    {
        size_type i = 0;
        for (auto& elem : initList) {
            new(&p_elem[i++]) value_type(elem);   // in-place construct
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
     */
    reference operator[](size_type index) { return p_elem[index]; }
    const_reference operator[](size_type index) const { return p_elem[index]; }

    /**
     * \brief Copy assign operator
     */
    vector& operator=(const vector& other) {
        if (this != &other) {
            // 
            if (m_capacity < other.m_capacity) {
                // old capaacity is not enough
                clear();
                destroy_vector();
                m_capacity = other.m_capacity;
                p_elem = static_cast<value_type*>(::operator new(m_capacity * sizeof(value_type)));
            }
            else {
                // reuse old memory space, clear without destroy
                clear();
            }

            // 
            m_size = other.m_size;
            for (size_type i = 0; i < other.m_size; ++i) 
                new(&p_elem[i]) value_type(other.p_elem[i]);
        }
        return *this;
    }

    /**
     * \brief Move assign operator
     */
    vector& operator=(vector&& other) noexcept {
        if (this != &other) {
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
     * \brief Direct access to the underlying contiguous storage
     */
    pointer data() noexcept { return p_elem; }
    const_pointer data() const noexcept { return p_elem; }


/* Iterators */
public:
    /**
     */
    iterator begin() { return iterator(p_elem); }
    const_iterator cbegin() const noexcept { return const_iterator(p_elem); }
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }

    /**
     */
    iterator end() { return iterator(p_elem + m_size); }
    const_iterator cend() const noexcept { return const_iterator(p_elem + m_size); }
    reverse_iterator rend() { return reverse_iterator(begin()); }
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
     * Invokes the destructor for each element in the vector, effectively
     * destroying all contained objects and resetting the vector's size to 0.
     */
    void clear() {
        for (size_type i = 0; i < m_size; ++i) {
            p_elem[i].~value_type();
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
        (it)->~value_type();
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
            (itLeft)->~value_type();
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
        // calculate the `tarIndex` first because the iterator `pos` will be invalidate after using `realloc`
        size_type tarIndex = pos - cbegin();

        // check capacity
        if (m_size >= m_capacity)
            realloc(REALLOC_RATE * m_capacity);

        // shift elements after pos
        for (size_type i = m_size; i > tarIndex; --i) {
            new(&p_elem[i]) value_type(std::move(p_elem[i-1]));
            p_elem[i-1].~value_type();
        }

        // in-place construct
        new(&p_elem[tarIndex]) value_type(std::forward<Args>(args)...);
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
        if (m_size >= m_capacity) {
            realloc(REALLOC_RATE * m_capacity);
        }
        p_elem[m_size++] = value;
    }


    /**
     * \brief Insert at the end of the vector.
     */
    void push_back(value_type&& value) {
        if (m_size >= m_capacity) {
            realloc(REALLOC_RATE * m_capacity);
        }
        p_elem[m_size++] = std::move(value);   // cast value to rval reference
    }


    /**
     * \brief Construct and insert at the end of the vector.
     */
    template <typename... Args>
    reference emplace_back(Args&&... args) {
        if (m_size >= m_capacity) {
            realloc(REALLOC_RATE * m_capacity);
        }
        // p_elem[m_size] = value_type(std::forward<Args>(args)...);
        new(&p_elem[m_size]) value_type(std::forward<Args>(args)...);   // in-place construction
        return p_elem[m_size++];
    }


    /**
     * \brief Delete the last element.
     */
    void pop_back() {
        if (m_size > 0) {
            p_elem[--m_size].~value_type();
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
    void resize(size_type count, const_reference value) {
        // 
        if (m_size > count) {
            erase(cbegin() + count, cend());
        }
        else {
            if (count > m_capacity) {
                reserve(count);
            }
            for (size_type i = m_size; i < count; ++i) {
                new(&p_elem[i]) value_type(value);
            }
        }

        // 
        m_size = count;
    }

    void resize(size_type count) {
        resize(count, value_type());
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
        pointer newBlock = static_cast<pointer>(::operator new(newCapacity * sizeof(value_type)));

        // copy/move old elements into new block
        size_type elemNum = newCapacity > m_size ? m_size : newCapacity;
        for (size_type i = 0; i < elemNum; ++i) {
            new(&newBlock[i]) value_type(std::move(p_elem[i]));
            p_elem[i].~value_type();
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
        ::operator delete[](p_elem);
        p_elem = nullptr;
        m_capacity = 0;
    }


private:
    size_type m_size     = 0;
    size_type m_capacity = 0;
    pointer   p_elem     = nullptr;
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
