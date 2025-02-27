#include <iostream>
#include <initializer_list>
#include <stdexcept>
#include <cassert>
#include <random>
#include <functional>

// 型態 T 會支援預設建構、複製建構、複製賦值與比較運算 (==, !=)
template<typename T>
class List {
public:
    List() = default;
    List(std::initializer_list<T> l);
    explicit List(size_t count);
    List(List<T>&& rhs) noexcept;
    List(const List<T>& rhs);
    List<T>& operator=(const List<T>& rhs);
    List<T>& operator=(List<T>&& rhs) noexcept;
    ~List();

    bool operator==(const List<T>& rhs) const;
    bool operator!=(const List<T>& rhs) const;

    T& operator[](size_t pos) ;
    const T& operator[](size_t pos) const;

    T& Front();
    const T& Front() const;
    T& Back();
    const T& Back() const;

    void Resize(size_t count); 

    void PushBack(const T& val); 
    void PushBack(T&& val);

    void PopBack();

    void PushFront(const T& val); 
    void PushFront(T&& val);

    void PopFront(); 

    void Insert(size_t pos, const T& val); 
    void Insert(size_t pos, T&& val);
    void Erase(size_t pos); 

    void Fill(const T& v); 
    void Assign(size_t count, const T& val); 
    void Clear() noexcept;
    void Remove(const T& val); 
    void Swap(List<T>& other);

    size_t Size() const noexcept { return size_; }

    size_t Capacity() const noexcept { return capacity_; }
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    T* elems_ = nullptr;
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& v) {
    if (v.Size() == 0) return os << "[]";
    os << '[' << v[0];
    for (size_t i = 1; i < v.Size(); i++) {
        os << ", " << v[i];
    }
    return os << ']';
}

void Test1();  // Sample1
void Test2();  // Back, Clear 
void Test3();  // PushFront, PopFront
void Test4();  // Insert, Erase
void Test5();  // operator==, operator!=, Remove, Fill
void Test6();  // Swap, Assign
void Test7();  // All
void Test8();  // PushFront, PopFront
void Test9();  // Insert, Erase
void Test10(); // operator==, operator!=, Remove, Fill
void Test11(); // Swap, Assign
void Test12(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = {
        Test1,  Test2, Test3, Test4, Test5,
        Test6,  Test7, Test8, Test9, Test10,
        Test11, Test12
    };
    f[id-1]();
}

template<typename T>
List<T>::List(std::initializer_list<T> l) :
    size_{l.size()}, capacity_{size_}, elems_{new T[capacity_]} {
    size_t i = 0;
    for (const T& v: l) {
        elems_[i] = v;
        ++i;
    }
}

template<typename T>
List<T>::List(size_t count) :
    size_{count}, capacity_{size_}, elems_{new T[capacity_]{}} {
}

template<typename T>
List<T>::List(const List<T>& rhs) :
    size_{rhs.size_}, capacity_{size_}, elems_{new T[capacity_]}
{
    for (size_t i = 0; i < size_; i++) {
        elems_[i] = rhs.elems_[i];
    }
}

template<typename T>
List<T>::List(List<T>&& rhs) noexcept :
    size_{rhs.size_}, capacity_{rhs.capacity_}, elems_{rhs.elems_}
{
    rhs.size_ = 0;
    rhs.capacity_ = 0;
    rhs.elems_ = nullptr;
}

template<typename T>
List<T>::~List() {
    delete[] elems_;
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& rhs) {
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
List<T>& List<T>::operator=(List<T>&& rhs) noexcept {
    size_ = rhs.size_;
    capacity_ = rhs.capacity_;
    std::swap(elems_, rhs.elems_);
    return *this;
}

template<typename T>
void List<T>::Resize(size_t count) {
    if (count <= capacity_) {
        for (size_t i = size_; i < count; i++) {
            elems_[i] = T{};
        }
        size_ = count;
        return;
    }

    size_t newCapacity = std::max(count, 2 * (capacity_ + 1));

    T* newElems = new T[newCapacity]{};
    for (size_t i = 0; i < size_; i++) {
        newElems[i] = elems_[i];
    }
    delete[] elems_;

    size_ = count;
    capacity_ = newCapacity;
    elems_ = newElems;
}

template<typename T>
void List<T>::PushBack(const T& val) {
    Resize(Size()+1);
    operator[](Size()-1) = val;
}

template<typename T>
void List<T>::PushBack(T&& val) {
    Resize(Size()+1);
    operator[](Size()-1) = std::move(val);
}

template<typename T>
void List<T>::PopBack() {
    if (size_ == 0) {
        throw std::out_of_range("PopBack(): List is empty.");
    }
    Resize(Size()-1);
}

template<typename T>
T& List<T>::operator[](size_t pos) {
    if (pos >= size_) {
        throw std::out_of_range{ "operator[](pos): " + std::to_string(pos) + " >= " + std::to_string(size_)};
    }
    return elems_[pos];
}      

template<typename T>
const T& List<T>::operator[](size_t pos) const {
    return const_cast<List*>(this)->operator[](pos);
}

template<typename T>
T& List<T>::Front() {
    if (size_ == 0) {
        throw std::out_of_range("Front(): List is empty.");
    }
    return elems_[0];
}

template<typename T>
const T& List<T>::Front() const {
    return const_cast<List*>(this)->Front();
}

void Test1()  {
    // 實作 Back() 方法回傳最後一個元素 (element)
    // - 當陣列為空時，丟出訊息為 "Back(): List is empty." 的 std::out_of_range 例外。
    // - 當有例外丟出時，物件維持呼叫 Back() 前的狀態。
    {
        List<int> a = {3, 9, 1, 7, 5};
        const List<int>& b = a;

        std::cout << a.Front() << std::endl; // 印出 3
        std::cout << a.Back() << std::endl;  // 印出 5
        std::cout << b.Front() << std::endl; // 印出 3
        std::cout << b.Back() << std::endl;  // 印出 5

        List<int> c;

        try {
            std::cout << c.Front() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl; // 印出 "Front(): List is empty."
        }
        std::cout << c << std::endl; // 印出 []

        try {
            std::cout << c.Back() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl; // 印出 "Back(): List is empty."
        }
        std::cout << c << std::endl; // 印出 []
    }

    // 實作 PushFront(val) 方法在陣列最前面插入一個值為 val 的新元素 (element)
    // 實作 PopFront() 方法移除陣列最前面的元素 (element)
    // - 當陣列為空時，丟出訊息為 "PopFront(): List is empty." 的 std::out_of_range 例外。
    // - 當有例外丟出時，物件維持呼叫 PopFront() 前的狀態。
    {
        List<int> a;
        std::cout << a << std::endl; // 印出 []

        for (size_t i = 1; i <= 3; i++) {
            a.PushFront(i);
            std::cout << a << std::endl;  // 依序印出 [1] -> [2, 1] -> [3, 2, 1]
        }

        for (size_t i = 1; i <= 3; i++) {
            a.PopBack();
            std::cout << a << std::endl; // 依序印出 [3, 2] -> [3] -> []
        }

        for (size_t i = 1; i <= 3; i++) {
            a.PushFront(std::move(i));
            std::cout << a << std::endl; // 依序印出 [1] -> [2, 1] -> [3, 2, 1]
        }

        for (size_t i = 1; i <= 3; i++) {
            a.PopFront();
            std::cout << a << std::endl; // 依序印出 [2, 1] -> [1] -> []
        }
    }

    // 實作 operator==(other) 回傳與 other 陣列的長度跟內容值是否都相等 (==)
    // 實作 operator!=(other) 回傳與 other 陣列的長度或內容值是否有任何一個不相等 (!=)
    // 實作 Remove(val) 方法，移除陣列中所有值等於 (==) val 的元素
    // 實作 Fill(val) 方法將陣列中所有元素都賦值為 val
    // 實作 Clear() 方法以移除陣列中所有元素 (element)，讓陣列為空
    {
        List<int> a{3, 9, 1, 7, 5};
        List<int> b{3, 9, 1};
        std::cout << std::boolalpha << (a == b) << std::endl; // 印出 false
        std::cout << std::boolalpha << (a != b) << std::endl; // 印出 true
        a.Resize(3);
        std::cout << std::boolalpha << (a == b) << std::endl; // 印出 true
        std::cout << std::boolalpha << (a != b) << std::endl; // 印出 false

        List<int> d = {3, 9, 1, 2, 9, 1, 7};
        d.Remove(9);
        std::cout << d << std::endl; // 印出 [3, 1, 2, 1, 7]
        d.Fill(9);
        std::cout << d << std::endl; // 印出 [9, 9, 9, 9, 9]
        d.Clear();
        std::cout << d << std::endl; // 印出 []

        List<List<int>> c{{3, 9}, {0, 8}};
        c.Remove({3, 9});
        std::cout << c << std::endl; // 印出 [[0, 8]]
    }

    // 實作 Insert(pos, val) 方法插入值為 val 的新元素在第 pos 號的位置 (位置從 0 開始編號)
    // - 當插入位置 (pos) 大於陣列長度時，丟出訊息為 "Insert(pos, val): 位置編號 > 陣列長度" 的 std::out_of_range 例外。
    // - 當有例外丟出時，物件維持呼叫 Insert(pos, val) 前的狀態。
    // 實作 Erase(pos) 方法移除在第 pos 號位置的元素 (位置從 0 開始編號)
    // - 當移除位置 (pos) 大於或等於陣列長度時，丟出訊息為 "Erase(pos): 位置編號 >= 陣列長度" 的 std::out_of_range 例外
    // - 當有例外丟出時，物件維持呼叫 Erase(pos) 前的狀態。
    {
        List<int> values{0, 3, 1, 4, 1, 4, 2, 0, 5, 3};

        List<int> d;

        for (size_t i = 0; i < 10; i++) {
            try {
                int k = values[i];
                d.Insert(k, k);
            } catch (const std::out_of_range &e) {
                std::cout << e.what() << std::endl;
            }
            std::cout << d << std::endl;
        }
        // 依序印出 [0]
        //         Insert(pos, val): 3 > 1
        //         [0]
        //         [0, 1]
        //         Insert(pos, val): 4 > 2
        //         [0, 1]
        //         [0, 1, 1]
        //         Insert(pos, val): 4 > 3
        //         [0, 1, 1]
        //         [0, 1, 2, 1]
        //         [0, 0, 1, 2, 1]
        //         [0, 0, 1, 2, 1, 5]
        //         [0, 0, 1, 3, 2, 1, 5]

        for (size_t i = 0; i < 10; i++) {
            try {
                d.Erase(values[i]);
            } catch (const std::out_of_range &e) {
                std::cout << e.what() << std::endl;
            }
            std::cout << d << std::endl;
        }
        // 依序印出 [0, 1, 3, 2, 1, 5]
        //        [0, 1, 3, 1, 5]
        //        [0, 3, 1, 5]
        //        Erase(pos, val): 4 >= 4
        //        [0, 3, 1, 5]
        //        [0, 1, 5]
        //        Erase(pos, val): 4 >= 3
        //        [0, 1, 5]
        //        [0, 1]
        //        [1]
        //        Erase(pos, val): 5 >= 1
        //        [1]
        //        Erase(pos, val): 3 >= 1
        //        [1]

        for (size_t i = 0; i < 10; i++) {
            try {
                int k = values[i];
                d.Insert(k, values[i]);
            } catch (const std::out_of_range &e) {
                std::cout << e.what() << std::endl;
            }
            std::cout << d << std::endl;
        }
        // 依序印出 [0, 1]
        //        Insert(pos, val): 3 > 2
        //        [0, 1]
        //        [0, 1, 1]
        //        Insert(pos, val): 4 > 3
        //        [0, 1, 1]
        //        [0, 1, 1, 1]
        //        [0, 1, 1, 1, 4]
        //        [0, 1, 2, 1, 1, 4]
        //        [0, 0, 1, 2, 1, 1, 4]
        //        [0, 0, 1, 2, 1, 5, 1, 4]
        //        [0, 0, 1, 3, 2, 1, 5, 1, 4]
    }

    // 實作 Swap(other) 方法，與其他陣列 (other) 交換
    {
        List<int> a = {3, 9, 1, 2};
        List<int> b = {7, 6, 5};

        std::cout << a << std::endl; // 印出 [3, 9, 1, 2]
        std::cout << b << std::endl; // 印出 [7, 6, 5]
        a.Swap(b);
        std::cout << a << std::endl; // 印出 [7, 6, 5]
        std::cout << b << std::endl; // 印出 [3, 9, 1, 2]
    }

    // 實作 Assign(count, val)，將陣列內容改為由指定個數 (count) 的指定值 (val) 構成
    {
        List<int> a = {3, 9, 1, 2};
        a.Assign(7, 1);
        std::cout << a << std::endl; // 印出 [1, 1, 1, 1, 1, 1, 1]
    }
}

namespace Feis { /* HIDDEN */ }

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { /* HIDDEN */ }
void Test7() { /* HIDDEN */ }
void Test8() { Test3(); }
void Test9() { Test4(); }
void Test10() { Test5(); }
void Test11() { Test6(); }
void Test12() { Test7(); }

// [YOUR CODE WILL BE PLACED HERE]
template<typename T>
T& List<T>::Back() {
    if(size_ == 0) {
        throw std::out_of_range("Back(): List is empty.");
    }
    return elems_[size_ - 1];
}

template<typename T>
const T& List<T>::Back() const {
    if(size_ == 0) {
        throw std::out_of_range("Back(): List is empty.");
    }
    return elems_[size_ - 1];
}
template<typename T>
void List<T>::PushFront(const T& val) {
    Resize(Size() + 1);
    for(int i = size_ - 1; i >= 1; i--) {
        elems_[i] = std::move(elems_[i - 1]);
    }
    elems_[0] = val;
} 

template<typename T>
void List<T>::PushFront(T&& val) {
    Resize(Size() + 1);
    for(int i = size_ - 1; i >= 1; i--) {
        elems_[i] = std::move(elems_[i - 1]);
    }
    elems_[0] = std::move(val);
}

template<typename T>
void List<T>::PopFront() {
    if(size_ == 0)
        throw std::out_of_range("PopFront(): List is empty.");
    for(int i = 0; i < size_ - 1; i++) {
        elems_[i] = std::move(elems_[i + 1]);
    }
    Resize(size_ - 1);
}

template<typename T>
bool List<T>::operator==(const List<T>& rhs) const {
    if(this->size_ != rhs.size_)
        return false;
    for(int i = 0; i < size_; i++) {
        if(elems_[i] != rhs.elems_[i])
            return false;
    }
    return true;
}

template<typename T>
bool List<T>::operator!=(const List<T>& rhs) const {
    return !(*this == rhs);
}

template<typename T>
void List<T>::Remove(const T& val) {
    if(size_ == 0)
        return;
    size_t j = 0; 
    for(size_t i = 0; i < size_; i++) {
        if(elems_[i] != val) {
            elems_[j] =elems_[i];
            j++;
        }
    }
    Resize(j);
}


template<typename T>
void List<T>::Fill(const T& v) {
    if(size_ == 0)
        return;
    for(int i = 0; i < size_; i++) {
        elems_[i] = v;
    }
}

template<typename T>
void List<T>::Clear() noexcept {
    Resize(0);
}

template<typename T>
void List<T>::Insert(size_t pos, const T& val) {
    if(pos > size_)
        throw std::out_of_range("Insert(pos, val): " + std::to_string(pos) + " > " + std::to_string(size_));
    Resize(size_ + 1);
    for(int i = size_ - 1; i > pos; i--) {
        elems_[i] = std::move(elems_[i - 1]);
    }
    elems_[pos] = val;
}

template<typename T>
void List<T>::Insert(size_t pos, T&& val) {
    if(pos > size_)
        throw std::out_of_range("Insert(pos, val): " + std::to_string(pos) + " > " + std::to_string(size_));
    Resize(size_ + 1);
    for(int i = size_ - 1; i > pos; i--) {
        elems_[i] = std::move(elems_[i - 1]);
    }
    elems_[pos] = std::move(val);
}

template<typename T>
void List<T>::Erase(size_t pos) {
    if(pos >= size_)
        throw std::out_of_range("Erase(pos): " + std::to_string(pos) + " >= " + std::to_string(size_));
    for(int i = pos; i < size_ - 1; i++) {
        elems_[i] = std::move(elems_[i + 1]);
    }
    Resize(size_ - 1);
}

template<typename T>
void List<T>::Swap(List<T>& other) {
    T* temp = this->elems_;
    this->elems_ = other.elems_;
    other.elems_ = temp;

    size_t temp_size = this->size_;
    this->size_ = other.size_;
    other.size_ = temp_size;

    size_t temp_cap = this->capacity_;
    this->capacity_ = other.capacity_;
    other.capacity_ = temp_cap;
}

template<typename T>
void List<T>::Assign(size_t count, const T& val) {
    Resize(count);
    Fill(val);
}