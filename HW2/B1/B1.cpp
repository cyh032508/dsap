#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cassert>    // For Test
#include <random>     // For Test
#include <functional> // For Test

// 模板參數 T 的合法型態會支援：
// - 值初始化：歸零
// - 複製語意：複製建構與賦值
// - 同型態間的比較運算：==, !=, >, >=, <, <=
// 符合以上要求的可能型態有：int, double, std::string 等等
template<typename T>
class Set {
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;
public:
    Set() = default;
    Set(std::initializer_list<T> l);
    explicit Set(const std::vector<T>& values);
    explicit Set(std::vector<T>&& values);
    void Add(const T&);
    void Add(T&&);
    void Remove(const T&);
    [[nodiscard]] bool Contains(const T&) const;
    [[nodiscard]] size_t Size() const noexcept { return repr_.size(); }

    void Swap(Set<T>& other);
    void Clear();
    [[nodiscard]] bool IsDisjointWith(const Set<T>& other) const;

    [[nodiscard]] Iterator begin() {
        return repr_.begin();
    }
    [[nodiscard]] Iterator end() {
        return repr_.end();
    }
    [[nodiscard]] ConstIterator begin() const {
        return repr_.begin();
    }
    [[nodiscard]] ConstIterator end() const {
        return repr_.end();
    }

private:
    std::vector<T> repr_;
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
Set<T>::Set(std::vector<T>&& values) {
    for (T& v : values) {
        Add(std::move(v));
    }
}

template<typename T>
void Set<T>::Add(const T& val) {
    if (!Contains(val)) {
        repr_.push_back(val);
    }
}

template<typename T>
void Set<T>::Add(T&& val) {
    if (!Contains(val)) {
        repr_.push_back(std::move(val));
    }
}

template<typename T>
void Set<T>::Remove(const T& val) {
    auto p = std::find(std::begin(repr_), std::end(repr_), val);
    if (p != std::end(repr_)) {
        repr_.erase(p);
    }
}

template<typename T>
bool Set<T>::Contains(const T& val) const {
    return std::find(std::begin(repr_), std::end(repr_), val) != std::end(repr_);
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const Set<T>& s) {
    std::vector<T> output(s.begin(), s.end());
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

template<typename T>
[[nodiscard]] Set<T> operator&(const Set<T>& lhs, const Set<T>& rhs);

template<typename T>
[[nodiscard]] Set<T> operator|(const Set<T>& lhs, const Set<T>& rhs);

template<typename T>
[[nodiscard]] Set<T> operator^(const Set<T>& lhs, const Set<T>& rhs);

template<typename T>
[[nodiscard]] Set<T> operator-(const Set<T>& lhs, const Set<T>& rhs);

void Test1(); // Sample1
void Test2(); // Swap, Clear
void Test3(); // operator==, operator!=
void Test4(); // operator<=, operator>=
void Test5(); // operator&, operator|, IsDisjointWith
void Test6(); // operator^, operator-
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
    // 實作 Swap(other) 方法，與集合 other 交換內容
    // 實作 Clear() 方法，清除內容，讓集合為空
    {
        Set<int> a = {3, 9, 5};
        Set<int> b = {3, 9, 1, 7, 5, 3};

        std::cout << "2.1)  " << a << std::endl;
        std::cout << "2.2)  " << b << std::endl;

        a.Swap(b);
        std::cout << "2.3)  " << a << std::endl;
        std::cout << "2.4)  " << b << std::endl;

        a.Clear();
        std::cout << "2.5)  " << a << std::endl;

        // 依序印出：
        // 2.1)  {3, 5, 9}
        // 2.2)  {1, 3, 5, 7, 9}
        // 2.3)  {1, 3, 5, 7, 9}
        // 2.4)  {3, 5, 9}
        // 2.5)  {}
    }

    // 實作 operator== 運算，回傳兩個集合是否相等
    // 實作 operator!= 運算，回傳兩個集合是否不相等
    // - 兩個集合相等只要所有元素值與個數相同就可以，不在乎元素存放的前後順序
    // operator==, operator !=
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 1};

        std::cout << "3.1)  " << std::boolalpha << (a == a) << std::endl;
        std::cout << "3.2)  " << std::boolalpha << (a == b) << std::endl;
        std::cout << "3.3)  " << std::boolalpha << (a == c) << std::endl;
        std::cout << "3.4)  " << std::boolalpha << (a != a) << std::endl;
        std::cout << "3.5)  " << std::boolalpha << (a != b) << std::endl;
        std::cout << "3.6)  " << std::boolalpha << (a != c) << std::endl;

        Set<std::string> d = {"1.5", "3.4"};
        Set<std::string> e = {"1.5", "3.5", "2.5"};
        Set<std::string> f = {"3.5", "1.5"};

        std::cout << "3.7)  " << std::boolalpha << (d == e) << std::endl;
        std::cout << "3.8)  " << std::boolalpha << (d == f) << std::endl;
        std::cout << "3.9)  " << std::boolalpha << (d != e) << std::endl;
        std::cout << "3.10) " << std::boolalpha << (d != f) << std::endl;

        // 依序印出
        // 3.1) true
        // 3.2) false
        // 3.3) true
        // 3.4) false
        // 3.5) true
        // 3.6) false
        // 3.7) false
        // 3.8) false
        // 3.9) true
        // 3.10) true
    }

    // 實作 operator<= 運算，回傳集合 lhs 中所有元素值是不是都出現在集合 rhs 中
    // 實作 operator>= 運算，回傳集合 rhs 中所有元素值是不是都出現在集合 lhs 中
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 2};
        Set<int> g = {3, 1};

        std::cout << "4.1)  " << std::boolalpha << (a <= g) << std::endl;
        std::cout << "4.2)  " << std::boolalpha << (a >= g) << std::endl;
        std::cout << "4.3)  " << std::boolalpha << (a <= b) << std::endl;
        std::cout << "4.4)  " << std::boolalpha << (a >= b) << std::endl;
        std::cout << "4.5)  " << std::boolalpha << (a <= c) << std::endl;
        std::cout << "4.6)  " << std::boolalpha << (a >= c) << std::endl;
        std::cout << "4.7)  " << std::boolalpha << (b <= c) << std::endl;
        std::cout << "4.8)  " << std::boolalpha << (b >= c) << std::endl;

        Set<std::string> d = {"1.5", "3.5"};
        Set<std::string> e = {"1.5", "3.5", "2.4"};
        Set<std::string> f = {"3.5", "2.5"};

        std::cout << "4.9) " << std::boolalpha << (d <= e) << std::endl;
        std::cout << "4.10) " << std::boolalpha << (d >= e) << std::endl;
        std::cout << "4.11) " << std::boolalpha << (d <= f) << std::endl;
        std::cout << "4.12) " << std::boolalpha << (d >= f) << std::endl;
        std::cout << "4.13) " << std::boolalpha << (e <= f) << std::endl;
        std::cout << "4.14) " << std::boolalpha << (e >= f) << std::endl;

        // 依序印出 :
        // 4.1) true
        // 4.2) true
        // 4.3) true
        // 4.4) false
        // 4.5) false
        // 4.6) false
        // 4.7) false
        // 4.8) true
        // 4.9) true
        // 4.10) false
        // 4.11) false
        // 4.12) false
        // 4.13) false
        // 4.14) false
    }

    // 實作 operator& 運算，回傳一個新集合表示交集：包含所有集合 lhs 與 rhs 中都出現的元素值
    // 實作 operator| 運算，回傳一個新集合表示聯集：包含所有集合 lhs 或 rhs 中有出現的元素值
    // 實作 IsDisjointWith 方法，回傳與集合 other 是否交集為空
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 2};
        Set<int> g = {4, 5};

        std::cout << "5.1)  " << (a & a) << std::endl;
        std::cout << "5.2)  " << (a & b) << std::endl;
        std::cout << "5.3)  " << (a & c) << std::endl;
        std::cout << "5.4)  " << (a | a) << std::endl;
        std::cout << "5.5)  " << (a | b) << std::endl;
        std::cout << "5.6)  " << (a | c) << std::endl;
        std::cout << "5.7)  " << std::boolalpha << a.IsDisjointWith(b) << std::endl;
        std::cout << "5.8)  " << std::boolalpha << a.IsDisjointWith(g) << std::endl;

        Set<std::string> d = {"1.5", "3.5"};
        Set<std::string> e = {"1.5", "3.5", "2.4"};
        Set<std::string> f = {"3.5", "2.5"};
        Set<std::string> h = {"4.5", "5.5"};

        std::cout << "5.9)  " << (d & e) << std::endl;
        std::cout << "5.10) " << (d & f) << std::endl;
        std::cout << "5.11) " << (d | e) << std::endl;
        std::cout << "5.12) " << (d | f) << std::endl;
        std::cout << "5.13) " << (d | e | f) << std::endl;
        std::cout << "5.14) " << std::boolalpha << d.IsDisjointWith(e) << std::endl;
        std::cout << "5.15) " << std::boolalpha << d.IsDisjointWith(h) << std::endl;

        // 依序印出 :
        // 5.1) {1, 3}
        // 5.2) {1, 3}
        // 5.3) {3}
        // 5.4) {1, 3}
        // 5.5) {1, 2, 3}
        // 5.6) {1, 2, 3}
        // 5.7) false
        // 5.8) true
        // 5.9) {1.5, 3.5}
        // 5.10) {3.5}
        // 5.11) {1.5, 2.4, 3.5}
        // 5.12) {1.5, 2.5, 3.5}
        // 5.13) {1.5, 2.4, 2.5, 3.5}
        // 5.14) false
        // 5.15) true
    }

    // 實作 operator^ 運算，回傳一個新集合，包含所有只存在 lhr 或 rhs 其中一個集合內的元素值
    // 實作 operator- 運算，回傳一個新集合表示差集，包含所有集合中 lhs 有出現但 rhs 中沒出現的元素值
    {
        Set<int> a = {1, 3};
        Set<int> b = {1, 3, 2};
        Set<int> c = {3, 2};
        Set<int> d;

        std::cout << "6.1)  " << (a ^ a) << std::endl;
        std::cout << "6.2)  " << (a ^ b) << std::endl;
        std::cout << "6.3)  " << (a ^ c) << std::endl;
        std::cout << "6.4)  " << (a - a) << std::endl;
        std::cout << "6.5)  " << (a - b) << std::endl;
        std::cout << "6.6)  " << (a - c) << std::endl;

        // 依序印出 :
        // 6.1) {}
        // 6.2) {2}
        // 6.3) {1, 2}
        // 6.4) {}
        // 6.5) {}
        // 6.6) {1}
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
void Set<T>::Swap(Set<T>& other) {
    repr_.swap(other.repr_);
}

template<typename T>
void Set<T>::Clear() {
    repr_.clear();
}

template<typename T>
bool operator==(const Set<T>& lhs, const Set<T>& rhs) {
    if(lhs.Size() != rhs.Size()) {
        return false;
    }
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
    for(const auto& elem : lhs) {
        if(!rhs.Contains(elem)) {
            return false;
        }
    }
    return true;
}

template<typename T>
bool operator>=(const Set<T>& lhs, const Set<T>& rhs) {
    return (rhs <= lhs);
}

template<typename T>
Set<T> operator&(const Set<T>& lhs, const Set<T>& rhs) {
    Set<T> intersection;
    for(const auto& elem : lhs) {
        if(rhs.Contains(elem)) {
            intersection.Add(elem);
        }
    }
    return intersection;
}

template<typename T>
Set<T> operator|(const Set<T>& lhs, const Set<T>& rhs) {
    Set<T> uni = lhs;
    for(const auto& elem : rhs) {
        if(!lhs.Contains()) {
            uni.Add(elem);
        }
    }
    return uni;
}

template<typename T>
bool Set<T>::IsDisjointWith(const Set<T>& other) const {
    for(const auto& elem : *this) {
        if(other.Contains(elem)) {
            return false;
        }
    }
    return true;
}

template<typename T>
Set<T> operator^(const Set<T>& lhs, const Set<T>& rhs) {
    Set<T> result;
    
    for (const auto& elem : lhs) {
        if (!rhs.Contains(elem)) {
            result.Add(elem);
        }
    }
    
    for (const auto& elem : rhs) {
        if (!lhs.Contains(elem)) {
            result.Add(elem);
        }
    }
    return result;
}

template<typename T>
Set<T> operator-(const Set<T>& lhs, const Set<T>& rhs) {
    Set<T> result;
    
    for (const auto& elem : lhs) {
        if (!rhs.Contains(elem)) {
            result.Add(elem);
        }
    }
    return result;
}
