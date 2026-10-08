#include "unique_pointer.hpp"
#include "shared_pointer.hpp"

#include <chrono>
#include <iostream>
#include <memory>
#include <type_traits>
#include <vector>

template <typename Pointer>
double measure(int count) {
    auto start = std::chrono::steady_clock::now();
    unsigned long long sum = 0;

    {
        std::vector<Pointer> pointers;
        pointers.reserve(count);

        for (int i = 0; i < count; ++i)
            pointers.emplace_back(new int(i));

        for (const auto& ptr : pointers)
            sum += *ptr;

        // Обычные указатели нужно удалять вручную.
        if constexpr (std::is_pointer_v<Pointer>) {
            for (Pointer ptr : pointers)
                delete ptr;
        }
    } // Умные указатели удаляют объекты здесь.

    auto end = std::chrono::steady_clock::now();
    return std::chrono::duration<double, std::micro>(end - start).count();
}

void compare(int count) {
    const char* names[] = {
        "raw", "my_unique", "std_unique", "my_shared", "std_shared"
    };
    double times[] = {
        measure<int*>(count),
        measure<UniquePointer<int>>(count),
        measure<std::unique_ptr<int>>(count),
        measure<SharedPointer<int>>(count),
        measure<std::shared_ptr<int>>(count)
    };

    std::cout << "\nObjects: " << count << "\nPointer\t\tTime (microseconds)\n";
    for (int i = 0; i < 5; ++i)
        std::cout << names[i] << "\t" << times[i] << '\n';
}




int main() {
    for (int count : {10, 1000, 1'000'000})
        compare(count);

    return 0;
}


