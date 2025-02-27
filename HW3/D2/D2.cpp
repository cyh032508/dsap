#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cassert>     // For Test
#include <random>     // For Test
#include <functional> // For Test

template<typename T>
struct IList {
    virtual T& operator[](size_t pos) = 0;
    virtual const T& operator[](size_t pos) const = 0;   
    virtual void Insert(size_t pos, const T& val) = 0;
    virtual void Insert(size_t pos, T&& val) = 0;
    virtual void Erase(size_t pos) = 0;
    virtual size_t Size() const = 0;
    virtual bool Empty() const = 0;
    virtual void Remove(const T&) = 0;
    virtual ~IList() {}
};

template<typename T>
class Vector: public IList<T> {
public:
    Vector() = default;
    Vector(std::initializer_list<T> l); 
    explicit Vector(size_t count); 
    Vector(Vector<T>&& rhs); 
    Vector(const Vector<T>& rhs); 
    Vector<T>& operator=(const Vector<T>& rhs);
    Vector<T>& operator=(Vector<T>&& rhs);
    ~Vector(); 

    T& operator[](size_t pos) override;
    const T& operator[](size_t pos) const override;   

    void Resize(size_t count); 

    void Insert(size_t pos, const T& val) override; 
    void Insert(size_t pos, T&& val) override; 
    void Erase(size_t pos) override; 
    void Remove(const T&) override;

    size_t Size() const override { return size_; }  
    bool Empty() const override { return size_ == 0; }

    size_t Capacity() const { return capacity_; }
    
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    T* elems_ = nullptr;
};

template<typename T>
Vector<T>::Vector(std::initializer_list<T> l) {
    size_ = l.size();
    capacity_ = size_;
    elems_ = new T[capacity_];

    int i = 0;
    for (const T& v: l) {
        elems_[i] = v;
        ++i;
    }
}

template<typename T>
Vector<T>::Vector(size_t count) :
    size_{count}, capacity_{size_}, elems_{new T[capacity_]{}} {
}

template<typename T>
Vector<T>::Vector(const Vector<T>& rhs) :
    size_{rhs.size_}, capacity_{size_}, elems_{new T[capacity_]}
{
    for (size_t i = 0; i < size_; i++) {
        elems_[i] = rhs.elems_[i];
    }
}

template<typename T>
Vector<T>::Vector(Vector<T>&& rhs) :
    size_{rhs.size_}, capacity_{rhs.capacity_}, elems_{rhs.elems_}
{
    rhs.size_ = 0;
    rhs.capacity_ = 0;
    rhs.elems_ = nullptr;
}

template<typename T>
Vector<T>::~Vector() {
    delete[] elems_;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& rhs) {
    if (this == &rhs) {
        return *this;
    }
    Resize(rhs.size_);
    for (size_t i = 0; i < size_; i++) {
        elems_[i] = rhs.elems_[i];
    }
    return *this;
}    

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& rhs) {
    std::swap(size_, rhs.size_);
    std::swap(capacity_, rhs.capacity_);
    std::swap(elems_, rhs.elems_);
    return *this;
}

template<typename T>
void Vector<T>::Resize(size_t count) {
    if (count <= capacity_) {
        for (size_t i = size_; i < count; i++) {
            elems_[i] = T{};
        }
        size_ = count;
        return;
    }

    capacity_ = std::max(count, 2 * (capacity_ + 1));

    T* newElems = new T[capacity_]{};
    for (size_t i = 0; i < size_; i++) {
        newElems[i] = std::move(elems_[i]);
    }
    delete[] elems_;
    elems_ = newElems;
    size_ = count;        
}  

template<typename T>
T& Vector<T>::operator[](size_t pos) {
    return elems_[pos];
}      

template<typename T>
const T& Vector<T>::operator[](size_t pos) const {
    return elems_[pos];
}  

template<typename T>
void Vector<T>::Insert(size_t pos, const T& val) {
    Resize(size_+1);
    for (size_t i = size_ - 1; i > pos; i--) {
        elems_[i] = elems_[i-1];
    }
    elems_[pos] = val;
}

template<typename T>
void Vector<T>::Insert(size_t pos, T&& val) {
    Resize(size_+1);
    for (size_t i = size_ - 1; i > pos; i--) {
        elems_[i] = std::move(elems_[i-1]);
    }
    elems_[pos] = std::move(val);
}

template<typename T>
void Vector<T>::Erase(size_t pos) {
    for (size_t i = pos; i+1 < size_; i++) {
        elems_[i] = std::move(elems_[i+1]);
    }
    Resize(size_-1);
}

template<typename T>
void Vector<T>::Remove(const T& val) {
    size_t j = 0;
    for (size_t i = 0; i < size_; i++) {
        if (elems_[i] != val) {
            elems_[j] = std::move(elems_[i]);
            j++;
        }
    }
    if (j != size_) {
        Resize(j);
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const IList<T>& v) {
    if (v.Size() == 0) return os << "[]";
    os << '[' << v[0];
    for (size_t i = 1; i < v.Size(); i++) {
        os << ", " << v[i];
    }
    return os << ']';
}

// 模板參數 T 的合理型態會支援：
// - 預設建構：歸零
// - 複製語意：複製建構與賦值
// - 同型態間的算術運算：+, -, * 與 / 運算
// - 同型態間的比較運算：==, !=, >, >=, <, <=
// 符合以上要求的可能型態有：int, double, ...
template<typename T>
class NumericArray : public Vector<T> {
public:
    NumericArray() = default;
    NumericArray(std::initializer_list<T> l) : Vector<T>{l}{}
    NumericArray(size_t count, const T& val) : Vector<T>(count, val) {}
    
    // TODO
    T Max() const;
    
    // TODO
    T Min() const;
    
    // TODO
    T Median() const;
    
    // TODO
    T Average() const;
    
    // TODO
    T Sum() const;
    
    // TODO
    NumericArray<T>& Sort();

    // TODO
    void TrimZeroes();
};

// TODO
template<typename T>
NumericArray<T> operator*(const NumericArray<T>& lhs, const T& rhs);

// TODO
template<typename T>
NumericArray<T> operator*(const T& lhs, const NumericArray<T>& rhs);

// TODO
template<typename T>
NumericArray<T> operator/(const NumericArray<T>& lhs, const T& rhs);

void Test1(); // Sample1
void Test2(); // operator*, operator/
void Test3(); // Max, Min
void Test4(); // Median, Sort
void Test5(); // Sum, Average
void Test6(); // TrimZeroes
void Test7(); // All 

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6, Test7 };
    f[id-1]();
}

namespace Feis { /* HIDDEN */ }

void Test1() {
    // Part 1. operator*, operator/
    {
        const NumericArray<int> a{0, 7, 3, 2, 9, 5, -3, 0};

        std::cout << "1.1) " << a << std::endl;
        std::cout << "1.2) " << (3 * a) << std::endl;
        std::cout << "1.3) " << (a * 4) << std::endl;

        const NumericArray<double> b{0, 7, 3, 2, 9, 5, -3, 0};
        std::cout << "1.4) " << b << std::endl;
        std::cout << "1.5) " << (0.5 * b) << std::endl;
        std::cout << "1.6) " << (b * 3.5) << std::endl;

        std::cout << "1.7) " << (a / 2) << std::endl;
        std::cout << "1.8) " << (b / 2.0) << std::endl;
    }

    // Part 2. Max, Min
    {
        const NumericArray<int> a{0, 7, 3, 2, 9, 5, -3, 0};
        std::cout << "2.1) " << a.Max() << std::endl;
        std::cout << "2.2) " << a.Min() << std::endl;
    }

    // Part 3: Median, Sort
    {
        NumericArray<int> a{0, 7, 3, 2, 9, 5, -3, 0};
        std::cout << "3.1) " << a.Median() << std::endl;
        std::cout << "3.2) " << a << std::endl;
        std::cout << "3.3) " << a.Sort() << std::endl;

        NumericArray<double> b{1, 0, 7, 3, 2, 9, 5, -3, 0};
        std::cout << "3.4) " << b.Median() << std::endl;
        std::cout << "3.5) " << b << std::endl;
        std::cout << "3.6) " << b.Sort() << std::endl;
    }

    // Part 4: Sum, Average
    {
        const NumericArray<int> a{0, 7, 3, 2, 9, 5, -3, 0};
        std::cout << "4.1) " << a.Sum() << std::endl;
        std::cout << "4.2) " << a << std::endl;
        std::cout << "4.3) " << a.Average() << std::endl;

        const NumericArray<double> b{0, 7, 3, 2, 9, 5, -3, 0};
        std::cout << "4.4) " << b.Sum() << std::endl;
        std::cout << "4.5) " << b << std::endl;
        std::cout << "4.6) " << b.Average() << std::endl;
    }

    // Part 5: TrimZeroes
    {
        NumericArray<int> a{0, 7, 3, 2, 9, 5, -3, 0};
        a.TrimZeroes();
        std::cout << "5.1) " << a << std::endl;

        NumericArray<double> b{1, 0, 7, 3, 2, 9, 5, -3, 0};
        b.TrimZeroes();
        std::cout << "5.2) " << b << std::endl;
    }
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { /* HIDDEN */ }
void Test7() { /* HIDDEN */ }

// [YOUR CODE WILL BE PLACED HERE]
template<typename T>
T NumericArray<T>::Max() const {
    T max = (*this)[0];
    for(size_t i = 0; i < this->Size(); i++) {
        if((*this)[i] > max)
            max = (*this)[i];
    }
    return max;
}
    
template<typename T>
T NumericArray<T>::Min() const {
    T min = (*this)[0];
    for(size_t i = 0; i < this->Size(); i++) {
        if((*this)[i] < min)
            min = (*this)[i];
    }
    return min;
}
    
template<typename T>
T NumericArray<T>::Median() const {
    NumericArray<T> sortedArray = *this; 
    sortedArray.Sort();
    T median = sortedArray[sortedArray.Size() - 1];
    
    size_t size = sortedArray.Size();
    if(size % 2 == 0) {
        size_t i = 1;
        for(size_t j = sortedArray.Size() - 1; j >= 0; j--) {
            if(i == size/2)
                break;
            if(sortedArray[j] < median) {
                median = sortedArray[j];
                i++;
            }
        }
    } else {
        size_t i = 1;
        for(size_t j = sortedArray.Size() - 1; j >= 0; j--) {
            if(i == size/2 + 1)
                break;
            if(sortedArray[j] < median) {
                median = sortedArray[j];
                i++;
            }
        }
    }
    return median;
}
    
template<typename T>
T NumericArray<T>::Average() const {
    T size = this->Size();
    return this->Sum() / size;
}
    
template<typename T>
T NumericArray<T>::Sum() const {
    T sum = 0;
    for(size_t i = 0; i < this->Size(); i++) {
        sum = sum + (*this)[i];
    }
    return sum;
}
    
template<typename T>
NumericArray<T>& NumericArray<T>::Sort() {
    if(this->Empty())
        return *this;
    for (size_t i = 0; i < this->Size() - 1; ++i) {
        size_t minIndex = i;
        for (size_t j = i + 1; j < this->Size(); ++j) {
            if ((*this)[j] < (*this)[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            T temp = (*this)[i];
            (*this)[i] = (*this)[minIndex];
            (*this)[minIndex] = temp;
        }
    }

    return *this; 
}

template<typename T>
void NumericArray<T>::TrimZeroes() {
    if(this->Empty())
        return;
    size_t front = 0, end = 0;
    for(int i = 0; i < this->Size(); i++) {
        if((*this)[i] == 0)
            front++;
        else
            break;
    }
    for(int i = this->Size(); i > 0; i--) {
        if((*this)[i - 1] == 0)
            end++;
        else
            break;
    }
    for(int i = 0; i < this->Size() - (front + end); i++) {
        (*this)[i] = (*this)[front + i];
    }
    (*this).Resize(this->Size() - (front + end));
    
}


template<typename T>
NumericArray<T> operator*(const NumericArray<T>& lhs, const T& rhs) {
    if(lhs.Empty())
        return NumericArray<T>();
    NumericArray<T> result;
    for(size_t i = 0; i < lhs.Size(); i++) {
        result.Insert(i, lhs[i] * rhs);
    }
    return result;
}

template<typename T>
NumericArray<T> operator*(const T& lhs, const NumericArray<T>& rhs) {
    if(rhs.Empty())
        return NumericArray<T>();
    NumericArray<T> result;
    for(size_t i = 0; i < rhs.Size(); i++) {
        result.Insert(i, lhs * rhs[i]);
    }
    return result;
}

template<typename T>
NumericArray<T> operator/(const NumericArray<T>& lhs, const T& rhs) {
    if(lhs.Empty())
        return NumericArray<T>();
    NumericArray<T> result;
    for(size_t i = 0; i < lhs.Size(); i++) {
        result.Insert(i, lhs[i] / rhs);
    }
    return result;
}
/*
1.1) [0, 7, 3, 2, 9, 5, -3, 0]
1.2) [0, 21, 9, 6, 27, 15, -9, 0]
1.3) [0, 28, 12, 8, 36, 20, -12, 0]
1.4) [0, 7, 3, 2, 9, 5, -3, 0]
1.5) [0, 3.5, 1.5, 1, 4.5, 2.5, -1.5, 0]
1.6) [0, 24.5, 10.5, 7, 31.5, 17.5, -10.5, 0]
1.7) [0, 3, 1, 1, 4, 2, -1, 0]
1.8) [0, 3.5, 1.5, 1, 4.5, 2.5, -1.5, 0]
2.1) 9
2.2) -3
3.1) 3
3.2) [0, 7, 3, 2, 9, 5, -3, 0]
3.3) [-3, 0, 0, 2, 3, 5, 7, 9]
3.4) 2
3.5) [1, 0, 7, 3, 2, 9, 5, -3, 0]
3.6) [-3, 0, 0, 1, 2, 3, 5, 7, 9]
4.1) 23
4.2) [0, 7, 3, 2, 9, 5, -3, 0]
4.3) 2
4.4) 23
4.5) [0, 7, 3, 2, 9, 5, -3, 0]
4.6) 2.875
5.1) [7, 3, 2, 9, 5, -3]
5.2) [1, 0, 7, 3, 2, 9, 5, -3]
*/