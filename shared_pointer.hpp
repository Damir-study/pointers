#ifndef SHARED_POINTER_HPP
#define SHARED_POINTER_HPP

#include <cstddef>
#include <concepts>

template <typename T>
class SharedPointerBase {
public:
    virtual ~SharedPointerBase() = 0;
    SharedPointerBase(T *ptr = nullptr);

    T* get() const noexcept;
    std::size_t get_count() const noexcept;
    operator bool() const noexcept;
    bool if_unique() const noexcept;

protected:
    T *ptr;
    std::size_t *count_ptr;

    template <typename Y>
    friend class SharedPointer;
};


template <typename T>
class SharedPointer: public SharedPointerBase<T> {
public:
    ~SharedPointer() override;
    SharedPointer(T *ptr = nullptr);

    SharedPointer(const SharedPointer&);
    template <typename Y>
    requires std::convertible_to<Y*, T*>
    SharedPointer(const SharedPointer<Y>&);

    SharedPointer& operator=(const SharedPointer&);    
    template <typename Y>
    requires std::convertible_to<Y*, T*>
    SharedPointer& operator=(const SharedPointer<Y>&);

    SharedPointer(SharedPointer&&) noexcept;
    template <typename Y>
    requires std::convertible_to<Y*, T*>
    SharedPointer(SharedPointer<Y>&&) noexcept;

    SharedPointer& operator=(SharedPointer&&) noexcept;
    template <typename Y>
    requires std::convertible_to<Y*, T*>
    SharedPointer& operator=(SharedPointer<Y>&&) noexcept;

    T* operator->() const noexcept;
    T& operator*() const noexcept;
    
    void swap(SharedPointer&) noexcept;
    void reset(T* new_ptr = nullptr);
};


template <typename T>
class SharedPointer<T[]> : public SharedPointerBase<T> {
public:
    ~SharedPointer() override;
    SharedPointer(T *ptr = nullptr);

    SharedPointer(const SharedPointer&);
    template <typename Y>
    requires std::convertible_to<Y(*)[], T(*)[]>
    SharedPointer(const SharedPointer<Y[]>&);

    SharedPointer& operator=(const SharedPointer&);
    template <typename Y>
    requires std::convertible_to<Y(*)[], T(*)[]>
    SharedPointer& operator=(const SharedPointer<Y[]>&);

    SharedPointer(SharedPointer&&) noexcept;
    template <typename Y>
    requires std::convertible_to<Y(*)[], T(*)[]>
    SharedPointer(SharedPointer<Y[]>&&) noexcept;

    SharedPointer& operator=(SharedPointer&&) noexcept;    
    template <typename Y>
    requires std::convertible_to<Y(*)[], T(*)[]>
    SharedPointer& operator=(SharedPointer<Y[]>&&) noexcept; 

    T& operator[](std::size_t index) const noexcept;
    void swap(SharedPointer&) noexcept;
    void reset(T* new_ptr = nullptr);
};


#include "shared_pointer.tpp"

#endif // SHARED_POINTER_HPP