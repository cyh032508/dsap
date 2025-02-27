#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cassert>    // For Test
#include <random>     // For Test
#include <functional> // For Test

template<typename T>
struct ListNode {
    explicit ListNode(T val) : val{std::move(val)}, next{nullptr} {}
    T val;
    ListNode* next;
};

// 模板參數 T 的合法型態會支援：
// - 值初始化：歸零
// - 複製語意：複製建構與賦值
// - 同型態間的比較運算：==, !=, >, >=, <, <=
// 符合以上要求的可能型態有：int, double, std::string 等等
template<typename T>
class Set {
public:
    class ConstIterator {
        friend class Set;
    public:
        ConstIterator(ListNode<T>* ptr) : ptr_{ptr} {
        }
        bool operator==(const ConstIterator& rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator!=(const ConstIterator& rhs) const {
            return ptr_ != rhs.ptr_;
        }
        ConstIterator& operator++() {
            ptr_ = ptr_->next;
            return *this;
        }
        ConstIterator operator++(int) {
            ConstIterator prev = *this;
            ptr_ = ptr_->next;
            return prev;
        }
        const T& operator*() const {
            return ptr_->val;
        }
        const T* operator->() const {
            return &ptr_->val;
        }
    private:
        ListNode<T>* ptr_;
    };

    class Iterator {
        friend class Set;
    public:
        Iterator(ListNode<T>* ptr) : ptr_{ptr} {
        }
        bool operator==(const Iterator& rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator!=(const Iterator& rhs) const {
            return ptr_ != rhs.ptr_;
        }
        Iterator& operator++() {
            ptr_ = ptr_->next;
            return *this;
        }
        Iterator operator++(int) {
            auto prev = *this;
            ptr_ = ptr_->next;
            return prev;
        }
        T& operator*() {
            return ptr_->val;
        }
        T* operator->() {
            return &ptr_->val;
        }
    private:
        ListNode<T>* ptr_;
    };

    Set() = default;
    Set(const Set&) = delete;
    Set(std::initializer_list<T> l);
    Set(Set<T>&& other) {
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
    }
    explicit Set(const std::vector<T>& values);
    ~Set();

    Set<T>& operator=(const Set<T>&) = delete;
    Set<T>& operator=(Set<T>&& other) {
        std::swap(size_, other.size_);
        std::swap(head_, other.head_);
        return *this;
    }

    void Add(const T&);
    void Remove(const T&);
    [[nodiscard]] bool Contains(const T&) const;
    [[nodiscard]] size_t Size() const noexcept { return size_; }

    [[nodiscard]] Set<T> operator&(const Set<T>& rhs) const;

    [[nodiscard]] Set<T> operator|(const Set<T>& rhs) const;

    [[nodiscard]] Iterator begin() {
        return head_;
    }
    [[nodiscard]] Iterator end() {
        return nullptr;
    }
    [[nodiscard]] ConstIterator begin() const {
        return head_;
    }
    [[nodiscard]] ConstIterator end() const {
        return nullptr;
    }

private:
    size_t size_{};
    ListNode<T>* head_{};
};

template<typename T>
Set<T>::Set(std::initializer_list<T> l) {
    for (const T& v : l) {
        Add(v);
    }
}

template<typename T>
Set<T>::Set(const std::vector<T>& values) {
    for (const T& v : values) {
        Add(v);
    }
}

template<typename T>
Set<T>::~Set() {
    while (head_ != nullptr) {
        auto toDeleted = head_;
        head_ = head_->next;
        delete toDeleted;
    }
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const Set<T>& s) {
    std::vector<T> output;
    for (const auto& v : s) {
        output.push_back(v);
    }
    std::sort(std::begin(output), std::end(output));
    os << '{';
    if (output.size() > 0) {
        os << output[0];
        for (size_t i = 1; i < output.size(); ++i) {
            os << ", " << output[i];
        }
    }
    os << '}';
    return os;
}

template<typename T>
[[nodiscard]] bool operator==(const Set<T>& lhs, const Set<T>& rhs);

template<typename T>
[[nodiscard]] bool operator!=(const Set<T>& lhs, const Set<T>& rhs);

template<typename T>
[[nodiscard]] bool operator<=(const Set<T>& lhs, const Set<T>& rhs);

template<typename T>
[[nodiscard]] bool operator>=(const Set<T>& lhs, const Set<T>& rhs);

void Test1(); // Sample1
void Test2(); // Add, Remove, Contains
void Test3(); // Add, Remove, operator==, operator!=
void Test4(); // Add, Remove, operator<=, operator>=
void Test5(); // Add, Remove, operator&, operator|
void Test6(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6 };
    f[id-1]();
}

namespace Feis { /* HIDDEN */ }

void Test1() {
    // 實作 Contains(val) 方法
    // 實作 Add(val) 方法
    // 實作 Remove(val) 方法
    {
        Set<int> a = {3, 9, 5};

        std::cout << "01) " << std::boolalpha << a.Contains(3) << std::endl;
        std::cout << "02) " << std::boolalpha << a.Contains(4) << std::endl;

        a.Add(4);
        std::cout << "03) " << a << std::endl;

        a.Add(5);
        std::cout << "04) " << a << std::endl;

        a.Remove(5);
        std::cout << "05) " << a << std::endl;

        a.Remove(6);
        std::cout << "06) " << a << std::endl;

        // 依序印出:
        // 01) true
        // 02) false
        // 03) {3, 4, 5, 9}
        // 04) {3, 4, 5, 9}
        // 05) {3, 4, 9}
        // 06) {3, 4, 9}
    }

    // 實作 operator== 運算，回傳兩個集合是否相等
    // 實作 operator!= 運算，回傳兩個集合是否不相等
    // - 兩個集合相等只要所有元素值與個數相同就可以，不在乎元素存放的前後順序
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 1};

        std::cout << "07) " << std::boolalpha << (a == a) << std::endl;
        std::cout << "08) " << std::boolalpha << (a == b) << std::endl;
        std::cout << "09) " << std::boolalpha << (a == c) << std::endl;
        std::cout << "10) " << std::boolalpha << (a != a) << std::endl;
        std::cout << "11) " << std::boolalpha << (a != b) << std::endl;
        std::cout << "12) " << std::boolalpha << (a != c) << std::endl;

        Set<std::string> d = {"1.5", "3.4"};
        Set<std::string> e = {"1.5", "3.5", "2.5"};
        Set<std::string> f = {"3.5", "1.5"};

        std::cout << "13) " << std::boolalpha << (d == e) << std::endl;
        std::cout << "14) " << std::boolalpha << (d == f) << std::endl;
        std::cout << "15) " << std::boolalpha << (d != e) << std::endl;
        std::cout << "16) " << std::boolalpha << (d != f) << std::endl;

        // 依序印出 :
        // 07) true
        // 08) false
        // 09) true
        // 10) false
        // 11) true
        // 12) false
        // 13) false
        // 14) false
        // 15) true
        // 16) true
    }

    // 實作 operator<= 運算，回傳集合 lhs 中所有元素值是不是都出現在集合 rhs 中
    // 實作 operator>= 運算，回傳集合 rhs 中所有元素值是不是都出現在集合 lhs 中
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 2};
        Set<int> g = {3, 1};

        std::cout << "17) " << std::boolalpha << (a <= g) << std::endl;
        std::cout << "18) " << std::boolalpha << (a >= g) << std::endl;
        std::cout << "19) " << std::boolalpha << (a <= b) << std::endl;
        std::cout << "20) " << std::boolalpha << (a >= b) << std::endl;
        std::cout << "21) " << std::boolalpha << (a <= c) << std::endl;
        std::cout << "22) " << std::boolalpha << (a >= c) << std::endl;
        std::cout << "23) " << std::boolalpha << (b <= c) << std::endl;
        std::cout << "24) " << std::boolalpha << (b >= c) << std::endl;

        Set<std::string> d = {"1.5", "3.5"};
        Set<std::string> e = {"1.5", "3.5", "2.4"};
        Set<std::string> f = {"3.5", "2.5"};

        std::cout << "25) " << std::boolalpha << (d <= e) << std::endl;
        std::cout << "26) " << std::boolalpha << (d >= e) << std::endl;
        std::cout << "27) " << std::boolalpha << (d <= f) << std::endl;
        std::cout << "28) " << std::boolalpha << (d >= f) << std::endl;
        std::cout << "29) " << std::boolalpha << (e <= f) << std::endl;
        std::cout << "30) " << std::boolalpha << (e >= f) << std::endl;

        // 依序印出 :
        // 17) true
        // 18) true
        // 19) true
        // 20) false
        // 21) false
        // 22) false
        // 23) false
        // 24) true
        // 25) true
        // 26) false
        // 27) false
        // 28) false
        // 29) false
        // 30) false
    }

    // 實作 operator& 運算，回傳一個新集合表示交集：包含所有集合 lhs 與 rhs 中都出現的元素值
    // 實作 operator| 運算，回傳一個新集合表示聯集：包含所有集合 lhs 或 rhs 中有出現的元素值
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 2};

        std::cout << "31) " << (a & a) << std::endl;
        std::cout << "32) " << (a & b) << std::endl;
        std::cout << "33) " << (a & c) << std::endl;
        std::cout << "34) " << (a | a) << std::endl;
        std::cout << "35) " << (a | b) << std::endl;
        std::cout << "37) " << (a | c) << std::endl;

        Set<std::string> d = {"1.5", "3.5"};
        Set<std::string> e = {"1.5", "3.5", "2.4"};
        Set<std::string> f = {"3.5", "2.5"};

        std::cout << "38) " << (d & e) << std::endl;
        std::cout << "39) " << (d & f) << std::endl;
        std::cout << "40) " << (d | e) << std::endl;
        std::cout << "41) " << (d | f) << std::endl;
        std::cout << "42) " << (d | e | f) << std::endl;

        // 依序印出 :
        // 31) {1, 3}
        // 32) {1, 3}
        // 33) {3}
        // 34) {1, 3}
        // 35) {1, 2, 3}
        // 37) {1, 2, 3}
        // 38) {1.5, 3.5}
        // 39) {3.5}
        // 40) {1.5, 2.4, 3.5}
        // 41) {1.5, 2.5, 3.5}
        // 42) {1.5, 2.4, 2.5, 3.5}
    }
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { /* HIDDEN */ }

// [YOUR CODE WILL BE PLACED HERE]
template<typename T>
bool Set<T>::Contains(const T& val) const {
    ListNode<T>* current = head_;

    while(current != nullptr) {
        if(current->val == val) {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
void Set<T>::Add(const T& val) {
    if(Contains(val))
        return;
    ListNode<T>* new_node = new ListNode<T>(val);
    ListNode<T>* current = head_;
    ListNode<T>* prev = nullptr;
    while(current != nullptr && current->val < val) {
        prev = current;
        current = current->next;
    }

    if(prev == nullptr) {
        new_node->next = head_;
        head_ = new_node;
    } else {
        prev->next = new_node;
        new_node->next = current;
    }
    size_++;
}

template<typename T>
void Set<T>::Remove(const T& val) {
    if(!Contains(val))
        return;
    ListNode<T>* current = head_;
    ListNode<T>* prev = nullptr;

    while(current != nullptr && current->val != val) {
        prev = current;
        current = current->next;
    }
    if(prev == nullptr) {
        head_ = current->next;
        current->next = nullptr;
        delete current;
    } else {
        prev->next = current->next;
        current->next = nullptr;
        delete current;
    }
    size_--;
}

template<typename T>
bool operator==(const Set<T>& lhs, const Set<T>& rhs) {
    if(lhs.Size() != rhs.Size())
        return false;
    for(const auto& elem : lhs) {
        if(!rhs.Contains(elem))
            return false;
    }
    return true; 
}

template<typename T>
bool operator!=(const Set<T>& lhs, const Set<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator<=(const Set<T>& lhs, const Set<T>& rhs) {
    if(lhs.Size() > rhs.Size())
        return false;
    for(const auto& elem : lhs) {
        if(!rhs.Contains(elem))
            return false;
    }
    return true;
}

template<typename T>
bool operator>=(const Set<T>& lhs, const Set<T>& rhs) {
    return (rhs <= lhs);
}

template<typename T>
Set<T> Set<T>::operator&(const Set<T>& rhs) const {
    Set<T> intersection;
    for(const auto& elem : *this) {
        if(rhs.Contains(elem))
            intersection.Add(elem);
    }
    return intersection;
}

template<typename T>
Set<T> Set<T>::operator|(const Set<T>& rhs) const {
    Set<T> uni;
    for(const auto& elem : *this) {
        uni.Add(elem);
    }
    for(const auto& elem : rhs) {
        if(!uni.Contains(elem))
            uni.Add(elem);
    }
    return uni;
}