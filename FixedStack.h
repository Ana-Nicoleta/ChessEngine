#pragma once
#include <array>
#include <stdexcept>
#include <utility>

template <typename T, size_t MaxSize>
class FixedStack {
private:
    std::array<T, MaxSize> data;
    size_t currentIndex;

public:
    FixedStack() : currentIndex(0) {}
    void push(T item) {
        if (currentIndex >= MaxSize) {
            throw std::overflow_error("Stack Overflow: The history stack is full!");
        }
        data[currentIndex++] = std::move(item);
    }
    T pop() {
        if (currentIndex == 0) {
            throw std::underflow_error("Stack Underflow: No moves left to undo!");
        }
        return std::move(data[--currentIndex]);
    }
    const T& peek() const {
        if (currentIndex == 0) {
            throw std::underflow_error("Stack is empty!");
        }
        return data[currentIndex - 1];
    }
    const T& get(size_t index) const {
        if (index >= currentIndex) {
            throw std::out_of_range("Index out of bounds!");
        }
        return data[index];
    }

    bool isEmpty() const { return currentIndex == 0; }
    size_t size() const { return currentIndex; }
    size_t capacity() const { return MaxSize; }
};
