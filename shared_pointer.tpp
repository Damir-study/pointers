#include "shared_pointer.hpp"
#include <iostream>


template <typename T>
SharedPointerBase<T>::~SharedPointerBase() {}

template <typename T>
SharedPointerBase<T>::SharedPointerBase(T *ptr) 
    : ptr(ptr) {
    if (ptr) count_ptr = new std::size_t(1);
    else count_ptr = nullptr;
    }

template <typename T>
T* SharedPointerBase<T>::get() const noexcept{
    return this->ptr;
}

template <typename T>
std::size_t SharedPointerBase<T>::get_count() const noexcept {
    if (count_ptr) return *count_ptr;
    else return 0;
}

template <typename T>
SharedPointerBase<T>::operator bool() const noexcept{
    return (ptr != nullptr);
}

template <typename T>
bool SharedPointerBase<T>::if_unique() const noexcept {
    if (count_ptr) return *count_ptr == 1;
    else return false;
}


template <typename T>
SharedPointer<T>::~SharedPointer() {
    if (this->count_ptr && --(*(this->count_ptr)) == 0) {
        delete this->ptr;
        delete this->count_ptr;
    }



    this->ptr = nullptr;
    this->count_ptr = nullptr;
}

template <typename T>
SharedPointer<T>::SharedPointer(T *ptr) 
    : SharedPointerBase<T>(ptr) {

    }

template <typename T>
SharedPointer<T>::SharedPointer(const SharedPointer<T> &other) {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    if (this->count_ptr) ++(*(this->count_ptr));
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*), T(*)>
SharedPointer<T>::SharedPointer(const SharedPointer<Y> &other) {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    if (this->count_ptr) ++(*(this->count_ptr));
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<T> &other) {
    if (this != &other) {
        if (this->count_ptr && --(*(this->count_ptr)) == 0) {
            delete this->ptr;
            delete this->count_ptr;
        }
        this->ptr = other.ptr;
        this->count_ptr = other.count_ptr;
        if (this->count_ptr) ++(*(this->count_ptr));
    }

    return *this;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*), T(*)>
SharedPointer<T>& SharedPointer<T>::operator=(const SharedPointer<Y> &other) {
    if (this->count_ptr && --(*(this->count_ptr)) == 0) {
        delete this->ptr;
        delete this->count_ptr;
    }
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    if (this->count_ptr) ++(*(this->count_ptr));
    
    return *this;
}

template <typename T>
SharedPointer<T>::SharedPointer(SharedPointer<T>&& other) noexcept {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    other.ptr = nullptr;
    other.count_ptr = nullptr;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*), T(*)>
SharedPointer<T>::SharedPointer(SharedPointer<Y>&& other) noexcept {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    other.ptr = nullptr;
    other.count_ptr = nullptr;
}

template <typename T>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<T>&& other) noexcept {
    if (this != &other) {
        if (this->count_ptr && --(*(this->count_ptr)) == 0) {
            delete this->ptr;
            delete this->count_ptr;
        }
        this->ptr = other.ptr;
        this->count_ptr = other.count_ptr;
        other.ptr = nullptr;
        other.count_ptr = nullptr;
    }
    return *this;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*), T(*)>
SharedPointer<T>& SharedPointer<T>::operator=(SharedPointer<Y>&& other) noexcept {
    if (this->count_ptr && --(*(this->count_ptr)) == 0) {
        delete this->ptr;
        delete this->count_ptr;
    }
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    other.ptr = nullptr;
    other.count_ptr = nullptr;
    
    return *this;
}

template <typename T>
T* SharedPointer<T>::operator->() const noexcept{
    return this->ptr;
}

template <typename T>
T& SharedPointer<T>:: operator*() const noexcept{
    return *(this->ptr);
}

template <typename T>
void SharedPointer<T>::swap(SharedPointer<T>& other) noexcept{
    if (this != &other) {
        T *ptr_temp = this->ptr;
        this->ptr = other.ptr;
        other.ptr = ptr_temp;

        std::size_t *count_temp = this->count_ptr;
        this->count_ptr = other.count_ptr;
        other.count_ptr = count_temp;
    }
}

template <typename T>
void SharedPointer<T>::reset(T* new_ptr) {
    if (new_ptr == this->get()) {
        return;
    }

    if (this->count_ptr && --(*this->count_ptr) == 0) {
        delete this->ptr;
        delete this->count_ptr;
    }

    this->ptr = new_ptr;

    if (new_ptr) {
        this->count_ptr = new std::size_t(1);
    } else {
        this->count_ptr = nullptr;
    }
}
 


template <typename T>
SharedPointer<T[]>::~SharedPointer() {
    if (this->count_ptr && --(*(this->count_ptr)) == 0) {
        delete[] this->ptr;
        delete this->count_ptr;
    }
    this->ptr = nullptr;
    this->count_ptr = nullptr;
}

template <typename T>
SharedPointer<T[]>::SharedPointer(T *ptr) 
    : SharedPointerBase<T>(ptr) {}

template <typename T>
SharedPointer<T[]>::SharedPointer(const SharedPointer<T[]> &other) {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    if (this->count_ptr) ++(*(this->count_ptr));
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*)[], T(*)[]>
SharedPointer<T[]>::SharedPointer(const SharedPointer<Y[]> &other) {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    if (this->count_ptr) ++(*(this->count_ptr));
}

template <typename T>
SharedPointer<T[]>& SharedPointer<T[]>::operator=(const SharedPointer<T[]> &other) {
    if (this != &other) {
        if (this->count_ptr && --(*(this->count_ptr)) == 0) {
            delete[] this->ptr;
            delete this->count_ptr;
        }
        this->ptr = other.ptr;
        this->count_ptr = other.count_ptr;
        if (this->count_ptr) ++(*(this->count_ptr));
    }
    return *this;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*)[], T(*)[]>
SharedPointer<T[]>& SharedPointer<T[]>::operator=(const SharedPointer<Y[]> &other) {
    if (this->count_ptr && --(*(this->count_ptr)) == 0) {
        delete[] this->ptr;
        delete this->count_ptr;
    }
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    if (this->count_ptr) ++(*(this->count_ptr));
    
    return *this;
}

template <typename T>
SharedPointer<T[]>::SharedPointer(SharedPointer<T[]>&& other) noexcept {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    other.ptr = nullptr;
    other.count_ptr = nullptr;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*)[], T(*)[]>
SharedPointer<T[]>::SharedPointer(SharedPointer<Y[]>&& other) noexcept {
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    other.ptr = nullptr;
    other.count_ptr = nullptr;
}

template <typename T>
SharedPointer<T[]>& SharedPointer<T[]>::operator=(SharedPointer<T[]>&& other) noexcept {
    if (this != &other) {
        if (this->count_ptr && --(*(this->count_ptr)) == 0) {
            delete[] this->ptr;
            delete this->count_ptr;
        }
        this->ptr = other.ptr;
        this->count_ptr = other.count_ptr;
        other.ptr = nullptr;
        other.count_ptr = nullptr;
    }
    return *this;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*)[], T(*)[]>
SharedPointer<T[]>& SharedPointer<T[]>::operator=(SharedPointer<Y[]>&& other) noexcept {
    if (this->count_ptr && --(*(this->count_ptr)) == 0) {
        delete[] this->ptr;
        delete this->count_ptr;
    }
    this->ptr = other.ptr;
    this->count_ptr = other.count_ptr;
    other.ptr = nullptr;
    other.count_ptr = nullptr;
    
    return *this;
}

template <typename T>
T& SharedPointer<T[]>::operator[](std::size_t index) const noexcept{
    return *(this->ptr+index);
}

template <typename T>
void SharedPointer<T[]>::swap(SharedPointer<T[]>& other) noexcept{
    if (this != &other) {
        T *ptr_temp = this->ptr;
        this->ptr = other.ptr;
        other.ptr = ptr_temp;

        std::size_t *count_temp = this->count_ptr;
        this->count_ptr = other.count_ptr;
        other.count_ptr = count_temp;
    }
}

template <typename T>
void SharedPointer<T[]>::reset(T* new_ptr) {
    if (new_ptr == this->get()) {
        return;
    }

    if (this->count_ptr && --(*this->count_ptr) == 0) {
        delete[] this->ptr;
        delete this->count_ptr;
    }

    this->ptr = new_ptr;

    if (new_ptr) {
        this->count_ptr = new std::size_t(1);
    } else {
        this->count_ptr = nullptr;
    }
}