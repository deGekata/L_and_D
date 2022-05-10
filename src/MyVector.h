#ifndef MY_VECTOR
#define MY_VECTOR

#include <inttypes.h>
#include <stdlib.h>
#include <string>
#include <exception>


template<typename T>
class MyVector {
public:
    MyVector() {
        cap = alloc;
        vector = (T*)malloc(sizeof(T) * alloc);
        elem_num = 0;
    };

    void push_back(const T &data);
    void pop_back();

    [[nodiscard]] bool empty() const;
    [[nodiscard]] size_t size() const;
    [[nodiscard]] size_t capacity() const;
    T &operator[](size_t pos);

    ~MyVector();
private:
    T *vector = nullptr;
    size_t cap;
    size_t elem_num;
    const int alloc = 20;
};

template<typename T>
MyVector<T>::~MyVector() {
    free(vector);
}

template<typename T>
void MyVector<T>::push_back(const T &data) {
    if (elem_num < cap) {
        *(vector + elem_num) = data;
        elem_num++;
    } else {
        vector = (T*)realloc(vector, sizeof(T) * cap * 2);
        cap *= 2;

        if (vector) {
            *(vector + elem_num) = data;
            elem_num++;
        }
    }
}

template<typename T>
void MyVector<T>::pop_back() {
    if (empty())
        return;
    elem_num--;
}

template<typename T>
T &MyVector<T>::operator[](size_t pos) {
    if (pos >= 0 &&  pos <= elem_num)
        return *(this->arr + pos);
    throw  std::__throw_runtime_error("Out of bounds element access");
}

template<typename T>
size_t MyVector<T>::capacity() const {
    return cap;
}

template<typename T>
bool MyVector<T>::empty() const {
    return elem_num == 0;
}

template<typename T>
size_t MyVector<T>::size() const {
    return elem_num;
}

struct MyClass {
    int num;
    double num2;
};

#endif