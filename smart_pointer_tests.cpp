#include "unique_pointer.hpp"
#include "shared_pointer.hpp"
#include <gtest/gtest.h>
#include <type_traits>
#include <utility>

// Счётчик показывает, сколько объектов ещё не удалено.
struct Object {
    static inline int alive = 0;
    int value = 10;
    Object() { ++alive; }
    ~Object() { --alive; }
};

TEST(UniquePointerTest, EmptyPointer) {
    UniquePointer<int> ptr(nullptr);
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.release(), nullptr);
    ptr.reset();
    EXPECT_FALSE(ptr);
}

TEST(UniquePointerTest, AccessToObject) {
    UniquePointer<Object> ptr(new Object);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr->value, 10);
    (*ptr).value = 20;
    EXPECT_EQ(ptr.get()->value, 20);
}

TEST(UniquePointerTest, MoveConstructor) {
    UniquePointer<int> first(new int(42));
    int* address = first.get();
    UniquePointer<int> second(std::move(first));

    EXPECT_EQ(first.get(), nullptr);
    EXPECT_EQ(second.get(), address);
    EXPECT_EQ(*second, 42);
}

TEST(UniquePointerTest, MoveAssignment) {
    UniquePointer<Object> first(new Object);
    UniquePointer<Object> second(new Object);
    first->value = 42;

    second = std::move(first);

    EXPECT_FALSE(first);
    EXPECT_EQ(second->value, 42);
    EXPECT_EQ(Object::alive, 1);  // Старый объект second удалён.
}

TEST(UniquePointerTest, Reset) {
    UniquePointer<Object> ptr(new Object);
    Object* replacement = new Object;
    replacement->value = 30;

    ptr.reset(replacement);
    EXPECT_EQ(ptr.get(), replacement);
    EXPECT_EQ(ptr->value, 30);
    EXPECT_EQ(Object::alive, 1);

    ptr.reset();
    EXPECT_FALSE(ptr);
    EXPECT_EQ(Object::alive, 0);
}

TEST(UniquePointerTest, Release) {
    UniquePointer<Object> ptr(new Object);
    Object* address = ptr.get();
    Object* released = ptr.release();

    EXPECT_EQ(released, address);
    EXPECT_FALSE(ptr);
    EXPECT_EQ(Object::alive, 1);  // release не удаляет объект.
    delete released;
    EXPECT_EQ(Object::alive, 0);
}

TEST(UniquePointerTest, Swap) {
    UniquePointer<int> first(new int(10));
    UniquePointer<int> second(new int(20));
    first.swap(second);
    EXPECT_EQ(*first, 20);
    EXPECT_EQ(*second, 10);
}

TEST(UniquePointerTest, DestructorDeletesObject) {
    EXPECT_EQ(Object::alive, 0);
    {
        UniquePointer<Object> ptr(new Object);
        EXPECT_EQ(Object::alive, 1);
    }
    EXPECT_EQ(Object::alive, 0);
}

TEST(UniquePointerTest, ArrayAccessAndMove) {
    UniquePointer<int[]> first(new int[3]{10, 20, 30});
    first[1] = 99;
    UniquePointer<int[]> second(std::move(first));

    EXPECT_FALSE(first);
    EXPECT_EQ(second[0], 10);
    EXPECT_EQ(second[1], 99);
    EXPECT_EQ(second[2], 30);
}

TEST(UniquePointerTest, ArrayResetAndDestruction) {
    EXPECT_EQ(Object::alive, 0);
    {
        UniquePointer<Object[]> ptr(new Object[3]);
        EXPECT_EQ(Object::alive, 3);
        ptr.reset(new Object[2]);
        EXPECT_EQ(Object::alive, 2);
    }
    EXPECT_EQ(Object::alive, 0);
}

TEST(SharedPointerTest, EmptyPointer) {
    SharedPointer<int> ptr;
    EXPECT_EQ(ptr.get(), nullptr);
    EXPECT_FALSE(ptr);
    EXPECT_EQ(ptr.get_count(), 0);
    EXPECT_FALSE(ptr.if_unique());
    ptr.reset();
    EXPECT_FALSE(ptr);
}

TEST(SharedPointerTest, AccessToObject) {
    SharedPointer<Object> ptr(new Object);
    EXPECT_TRUE(ptr);
    EXPECT_EQ(ptr->value, 10);
    (*ptr).value = 20;
    EXPECT_EQ(ptr.get()->value, 20);
    EXPECT_EQ(ptr.get_count(), 1);
    EXPECT_TRUE(ptr.if_unique());
}

TEST(SharedPointerTest, CopyConstructor) {
    SharedPointer<int> first(new int(42));
    SharedPointer<int> second(first);

    EXPECT_EQ(first.get(), second.get());
    EXPECT_EQ(first.get_count(), 2);
    EXPECT_EQ(second.get_count(), 2);
    EXPECT_FALSE(first.if_unique());
    *second = 99;
    EXPECT_EQ(*first, 99);  // Обе копии указывают на один объект.
}

TEST(SharedPointerTest, CopyAssignment) {
    SharedPointer<Object> first(new Object);
    SharedPointer<Object> second(new Object);
    second = first;

    EXPECT_EQ(first.get(), second.get());
    EXPECT_EQ(first.get_count(), 2);
    EXPECT_EQ(second.get_count(), 2);
    EXPECT_EQ(Object::alive, 1);  // Старый объект second удалён.
}

TEST(SharedPointerTest, CopyEmptyPointer) {
    SharedPointer<int> empty;
    SharedPointer<int> copy(empty);
    SharedPointer<int> other(new int(10));
    other = empty;

    EXPECT_FALSE(copy);
    EXPECT_FALSE(other);
    EXPECT_EQ(empty.get_count(), 0);
    EXPECT_EQ(copy.get_count(), 0);
    EXPECT_EQ(other.get_count(), 0);
}

TEST(SharedPointerTest, MoveConstructor) {
    SharedPointer<int> first(new int(42));
    int* address = first.get();
    SharedPointer<int> second(std::move(first));

    EXPECT_FALSE(first);
    EXPECT_EQ(first.get_count(), 0);
    EXPECT_EQ(second.get(), address);
    EXPECT_EQ(*second, 42);
    EXPECT_EQ(second.get_count(), 1);
}

TEST(SharedPointerTest, MoveAssignment) {
    SharedPointer<Object> first(new Object);
    SharedPointer<Object> second(new Object);
    first->value = 42;
    second = std::move(first);

    EXPECT_FALSE(first);
    EXPECT_EQ(first.get_count(), 0);
    EXPECT_EQ(second->value, 42);
    EXPECT_EQ(second.get_count(), 1);
    EXPECT_EQ(Object::alive, 1);
}

TEST(SharedPointerTest, Reset) {
    SharedPointer<Object> first(new Object);
    SharedPointer<Object> second(first);
    first.reset(new Object);

    EXPECT_NE(first.get(), second.get());
    EXPECT_EQ(first.get_count(), 1);
    EXPECT_EQ(second.get_count(), 1);
    EXPECT_EQ(Object::alive, 2);  // Старый объект ещё нужен second.

    second.reset();
    EXPECT_FALSE(second);
    EXPECT_EQ(second.get_count(), 0);
    EXPECT_EQ(Object::alive, 1);
    first.reset();
    EXPECT_EQ(Object::alive, 0);
}

TEST(SharedPointerTest, Swap) {
    SharedPointer<int> first(new int(10));
    SharedPointer<int> copy(first);
    SharedPointer<int> second(new int(20));
    first.swap(second);

    EXPECT_EQ(*first, 20);
    EXPECT_EQ(*second, 10);
    EXPECT_EQ(first.get_count(), 1);
    EXPECT_EQ(second.get_count(), 2);
    EXPECT_EQ(second.get(), copy.get());
}

TEST(SharedPointerTest, LastOwnerDeletesObject) {
    EXPECT_EQ(Object::alive, 0);
    {
        SharedPointer<Object> first(new Object);
        {
            SharedPointer<Object> second(first);
            EXPECT_EQ(first.get_count(), 2);
        }
        EXPECT_EQ(Object::alive, 1);
        EXPECT_EQ(first.get_count(), 1);
        EXPECT_TRUE(first.if_unique());
    }
    EXPECT_EQ(Object::alive, 0);
}

TEST(SharedPointerTest, ArrayAccessAndCopy) {
    SharedPointer<int[]> first(new int[3]{10, 20, 30});
    SharedPointer<int[]> second(first);
    second[1] = 99;

    EXPECT_EQ(first[0], 10);
    EXPECT_EQ(first[1], 99);
    EXPECT_EQ(first[2], 30);
    EXPECT_EQ(first.get_count(), 2);
    first.reset();
    EXPECT_EQ(second[1], 99);
    EXPECT_TRUE(second.if_unique());
}

TEST(SharedPointerTest, ArrayDestruction) {
    EXPECT_EQ(Object::alive, 0);
    {
        SharedPointer<Object[]> first(new Object[3]);
        SharedPointer<Object[]> second(first);
        first.reset();
        EXPECT_EQ(Object::alive, 3);
    }
    EXPECT_EQ(Object::alive, 0);
}

// Базовая проверка подтипизации.
struct Base {
    virtual ~Base() = default;
    int value = 42;
};
struct Derived : Base {};

TEST(UniquePointerTest, MoveDerivedToBase) {
    UniquePointer<Derived> derived(new Derived);
    UniquePointer<Base> base(std::move(derived));
    EXPECT_FALSE(derived);
    EXPECT_EQ(base->value, 42);
}

TEST(SharedPointerTest, CopyDerivedToBase) {
    SharedPointer<Derived> derived(new Derived);
    SharedPointer<Base> base(derived);
    EXPECT_EQ(base.get(), static_cast<Base*>(derived.get()));
    EXPECT_EQ(base->value, 42);
    EXPECT_EQ(derived.get_count(), 2u);
    EXPECT_EQ(base.get_count(), 2u);
}
