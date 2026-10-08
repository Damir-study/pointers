#include "unique_pointer.hpp"

template <typename T>
UniquePointerBase<T>::~UniquePointerBase() {}

template <typename T>
UniquePointerBase<T>::UniquePointerBase(T *ptr) 
    : ptr(ptr) {}

template <typename T>
T* UniquePointerBase<T>::get() const noexcept{
    return this->ptr;
}

template <typename T>
T* UniquePointerBase<T>::release() noexcept{
    T *temp = ptr;
    ptr = nullptr;
    return temp; 
}

template <typename T>
UniquePointerBase<T>::operator bool() const noexcept{
    return (ptr != nullptr);
}



template <typename T>
UniquePointer<T>::~UniquePointer() {
    if (this->ptr) {
        delete this->ptr;
    }
}

template <typename T>
UniquePointer<T>::UniquePointer(T *ptr) 
    : UniquePointerBase<T>(ptr) {}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*), T(*)>
UniquePointer<T>& UniquePointer<T>::operator=(UniquePointer<Y>&& other) noexcept {
    T* new_ptr = other.release();

    delete this->ptr;
    this->ptr = new_ptr;

    return *this;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*), T(*)>
UniquePointer<T>::UniquePointer(UniquePointer<Y>&& other) noexcept 
    : UniquePointerBase<T>(other.release()) {}

template <typename T>
T* UniquePointer<T>::operator->() const noexcept{
    return this->ptr;
}

template <typename T>
T& UniquePointer<T>:: operator*() const noexcept{
    return *(this->ptr);
}

template <typename T>
void UniquePointer<T>::swap(UniquePointer<T>& other) noexcept{
    if (this != &other) {
        T *temp = this->ptr;
        this->ptr = other.ptr;
        other.ptr = temp;
    }
}

template <typename T>
void UniquePointer<T>::reset(T* new_ptr) noexcept{
    if (this->ptr && new_ptr != this->get()) delete this->ptr;
    this->ptr = new_ptr;
}



template <typename T>
UniquePointer<T[]>::~UniquePointer() {
    delete[] this->ptr;
}

template <typename T>
UniquePointer<T[]>::UniquePointer(T *ptr) 
    : UniquePointerBase<T>(ptr) {}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*)[], T(*)[]>
UniquePointer<T[]>& UniquePointer<T[]>::operator=(UniquePointer<Y[]>&& other) noexcept {
    T* new_ptr = other.release();

    delete[] this->ptr;
    this->ptr = new_ptr;

    return *this;
}

template <typename T>
template <typename Y>
requires std::convertible_to<Y(*)[], T(*)[]>
UniquePointer<T[]>::UniquePointer(UniquePointer<Y[]>&& other) noexcept 
    : UniquePointerBase<T>(other.release()) {}

template <typename T>
T& UniquePointer<T[]>::operator[](std::size_t index) const noexcept{
    return *(this->ptr+index);
}

template <typename T>
void UniquePointer<T[]>::swap(UniquePointer<T[]>& other) noexcept{
    if (this != &other) {
        T *temp = this->ptr;
        this->ptr = other.ptr;
        other.ptr = temp;
    }
}

template <typename T>
void UniquePointer<T[]>::reset(T* new_ptr) noexcept{
    if (this->ptr && new_ptr != this->get()) delete[] this->ptr;
    this->ptr = new_ptr;
}