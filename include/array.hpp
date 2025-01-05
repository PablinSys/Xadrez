// array.hpp
#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <cstddef>

template<typename T, std::size_t N>
class Array {
public:
    Array();
    ~Array();
    void add(T& value);
    void remove(int index);
    template<typename U>
    bool contains(U& value, bool (*predicate)(U&, T&));
    int getSize();
    T& operator[](int index);

private:
    void resize();
    T* array;
    std::size_t size;
    int len = 0;
};

template<typename T, std::size_t N>
Array<T, N>::Array() {
    size = N;
    array = new T[N];
}

template<typename T, std::size_t N>
void Array<T, N>::resize() {
    size *= 2;
    T* temp = new T[size];
    for (int i = 0; i < len; i++)
        temp[i] = array[i];
    delete[] array;
    array = temp;
}

template<typename T, std::size_t N>
void Array<T, N>::add(T& value) {
    if (len == size)
        resize();
    array[len] = value;
    len++;
}

template<typename T, std::size_t N>
void Array<T, N>::remove(int index) {
    if (index < len && index >= 0 && len > 0) {
        for (int i = index; i < len - 1; i++)
            array[i] = array[i + 1];
        len--;
    }
}

template<typename T, std::size_t N>
int Array<T, N>::getSize() {
    return len;
}

template<typename T, std::size_t N>
T& Array<T, N>::operator[](int index) {
    return array[index];
}

template<typename T, std::size_t N>
Array<T, N>::~Array() {
    delete[] array;
}

#endif // ARRAY_HPP
