/**
 * \file vector.hpp

 * \reference:
 * - The Cherno: VECTOR/DYNAMIC ARRAY - Making DATA STRUCTURES in C++
 *          url: https://youtu.be/ryRf4Jh_YC0?si=9gncPajBHf4uIz-T
 */

#pragma once

#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstddef>   // size_t
#include <utility>   // move, forward
#include <new>       // ::operator new, ::operator delete


namespace mystl {


/**
 * \class Vector
 *
 * A template class of a simplified version of the std::vector,
 * providing dynamic array functionality with the ability to resize.
 */
template <typename T>
class Vector {
private:
    using ValueType      = T;
    using Pointer        = T*;
    using Reference      = T&;
    using ConstPointer   = const T*;
    using ConstReference = const T&;

private:
    static constexpr size_t DEFAULT_CAPACITY = 10;
    static constexpr size_t REALLOC_RATE     = 2;

public:
    /**
     * \brief Construct new vector with default initial capacity.
     */
    Vector() 
        : m_size(0), m_capacity(DEFAULT_CAPACITY), 
          p_data((Pointer)::operator new(DEFAULT_CAPACITY * sizeof(ValueType)))
    {
    }


    /**
     * \brief Destructor for the Vector.
     * 
     * Destroys all elements in the vector and deallocates the memory block
     * allocated for the vector's storage.
     */
    ~Vector() {
        clear();
        destroyVector();
    }


public:
    /**
     * \brief Return size.
     */
    std::size_t size() const { return m_size; }

    /**
     * \brief Return size of allocated storage capacity.
     */
    std::size_t capacity() const { return m_capacity; }

    /**
     * \brief Test whether vector is empty.
     */
    bool empty() const { return m_size == 0; }


public:
    /**
     */
    Reference operator[](std::size_t index) {
        // TODO: Add boundary check in debug mode
        return p_data[index];
    }

    /**
     */
    ConstReference operator[](std::size_t index) const {
        // TODO: Add boundary check in debug mode
        return p_data[index];
    }

    /**
     */
    Vector& operator=(ConstReference other) {
        if (this != other) {
        }
        return *this;
    }


    /**
     */
    Vector& operator=(ValueType&& other) {
        if (this != other) {
        }
        return *this;
    }


public:
    /**
     * \brief Insert at the end of the vector.
     */
    void pushBack(ConstReference value) {
        if (m_size >= m_capacity) {
            realloc(REALLOC_RATE * m_capacity);
        }
        p_data[m_size++] = value;
    }


    /**
     * \brief Insert at the end of the vector.
     */
    void pushBack(ValueType&& value) {
        if (m_size >= m_capacity) {
            realloc(REALLOC_RATE * m_capacity);
        }
        p_data[m_size++] = std::move(value);   // cast value to rval reference
    }


    /**
     * \brief Construct and insert at the end of the vector.
     */
    template <typename... Args>
    Reference emplaceBack(Args&&... args) {
        if (m_size >= m_capacity) {
            realloc(REALLOC_RATE * m_capacity);
        }
        // p_data[m_size] = ValueType(std::forward<Args>(args)...);
        new(&p_data[m_size]) ValueType(std::forward<Args>(args)...);   // in-place construction
        return p_data[m_size++];
    }


    /**
     * \brief Delete the last element.
     */
    void popBack() {
        if (m_size > 0) {
            p_data[--m_size].~ValueType();
        }
    }


    /**
     * \brief Clear all contents in vector.
     *
     * Invokes the destructor for each element in the vector, effectively
     * destroying all contained objects and resetting the vector's size to 0.
     */
    void clear() {
        for (std::size_t i = 0; i < m_size; ++i) {
            p_data[i].~ValueType();
        }
        m_size = 0;
    }


private:
    /**
     * \brief Reallocates the storage of the vector
     *
     * \param newCapacity
     */
    void realloc(size_t newCapacity) {
        // allocate a new block of memory
        Pointer newBlock = (Pointer)::operator new(newCapacity * sizeof(ValueType));

        // copy/move old elements into new block
        if (newCapacity < m_size) {
            m_size = newCapacity;
        }
        for (size_t i = 0; i < m_size; ++i) {
            new(&newBlock[i]) ValueType(std::move(p_data[i]));
        }

        // 
        clear();
        destroyVector();
        p_data = newBlock;
        m_capacity = newCapacity;
    }


    /**
     * \brief Deallocate all memory allocated in vector.
     *
     * Deallocate the memory block allocated for the vector's storage without
     * calling the contained elements' destructors.
     */
    void destroyVector() {
        // delete[] p_data;
        ::operator delete(p_data);
    }

private:
    size_t  m_size     = 0;
    size_t  m_capacity = 0;
    Pointer p_data     = nullptr;
};


}


#endif // !VECTOR_HPP_
