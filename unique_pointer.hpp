#ifndef UNIQUE_POINTER_HPP
#define UNIQUE_POINTER_HPP

#include <cstddef>
#include <concepts>

template <typename T>
class UniquePointerBase {
public:
    virtual ~UniquePointerBase() = 0;
    UniquePointerBase(T *ptr = nullptr);

    UniquePointerBase(const UniquePointerBase&) = delete;
    UniquePointerBase& operator=(const UniquePointerBase&) = delete;

    T* get() const noexcept;
    T* release() noexcept;
    operator bool() const noexcept;

protected:
    T *ptr;
};



template <typename T>
class UniquePointer: public UniquePointerBase<T> {
public:
    ~UniquePointer() override;
    UniquePointer(T *ptr);

    template <typename Y>
    requires std::convertible_to<Y*, T*>
    UniquePointer& operator=(UniquePointer<Y>&&) noexcept;
    template <typename Y>
    requires std::convertible_to<Y*, T*>
    UniquePointer(UniquePointer<Y>&&) noexcept;

    T* operator->() const noexcept;
    T& operator*() const noexcept;
    
    void swap(UniquePointer&) noexcept;
    void reset(T* new_ptr = nullptr) noexcept;
};



template <typename T>
class UniquePointer<T[]> : public UniquePointerBase<T> {
public:
    ~UniquePointer() override;
    UniquePointer(T *ptr);

    template <typename Y>
    requires std::convertible_to<Y(*)[], T(*)[]>
    UniquePointer& operator=(UniquePointer<Y[]>&&) noexcept;
    template <typename Y>
    requires std::convertible_to<Y(*)[], T(*)[]>
    UniquePointer(UniquePointer<Y[]>&&) noexcept;  

    T& operator[](std::size_t index) const noexcept;
    void swap(UniquePointer&) noexcept;
    void reset(T* new_ptr = nullptr) noexcept;
};

#include "unique_pointer.tpp"

#endif // UNIQUE_POINTER_HPP