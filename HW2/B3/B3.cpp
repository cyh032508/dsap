#include <utility>
#include <vector>
#include <iostream>
#include <initializer_list>
#include <algorithm>
#include <cassert>    // For Test
#include <random>     // For Test
#include <functional> // For Test

// 模板參數 T 的合理型態會支援：
// - 值初始化：歸零 (T{})
// - 複製語意：複製建構與賦值 (=)
// - 同型態間的比較運算：==, !=, >, >=, <, <=
// 符合以上要求的可能型態有：int, double, std::string, ...
template<typename T>
class ImmutableSet {
public:
    using Iterator = typename std::vector<T>::iterator;
    using ConstIterator = typename std::vector<T>::const_iterator;
    ImmutableSet() = default;
    ImmutableSet(std::initializer_list<T> l);
    ImmutableSet(const std::vector<T>& values);

    [[nodiscard]] size_t Size() const { return repr_.size(); }
    [[nodiscard]] bool Contains(const T& value) const;

    [[nodiscard]] bool IsDisjointWith(const ImmutableSet<T>& other) const;

    [[nodiscard]] ConstIterator begin() const { return repr_.begin(); }
    [[nodiscard]] ConstIterator end() const { return repr_.end(); }

private:
    std::vector<T> repr_;
};

template<typename T>
[[nodiscard]] bool operator==(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs);

template<typename T>
[[nodiscard]] bool operator!=(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs);

template<typename T>
[[nodiscard]] bool operator<=(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs);

template<typename T>
[[nodiscard]] bool operator>=(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs);

template<typename T>
[[nodiscard]] ImmutableSet<T> operator&(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs);

template<typename T>
[[nodiscard]] ImmutableSet<T> operator|(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs);

template<typename T>
std::ostream& operator<<(std::ostream& os, const ImmutableSet<T>& s) {
    std::vector<T> output{s.begin(), s.end()};
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

void Test1(); // Sample1
void Test2(); // constructors and Contains
void Test3(); // constructors, operator==, operator!=
void Test4(); // constructors, operator<=, operator>=
void Test5(); // constructors, operator&, operator|, IsDisjointWith
void Test6(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6};
    f[id-1]();
}

namespace Feis { /* HIDDEN */ }

void Test1() {
    // 實作初始化串列 (std::initializer_list<T>) 建構子，可以使用初始化串列建構集合
    // 實作 Contains(value) 方法，回傳集合內是否有值與 value 相等 (==) 的元素
    {
        ImmutableSet<int> a = {5, 1, 3};
        ImmutableSet<int> b = std::vector({3, 5, 1, 3});

        std::cout << "2.1)  " << a << std::endl;
        std::cout << "2.2)  " << b << std::endl;

        std::cout << "2.3)  " << std::boolalpha << a.Contains(1) << std::endl;
        std::cout << "2.4)  " << std::boolalpha << a.Contains(2) << std::endl;
        std::cout << "2.5)  " << std::boolalpha << a.Contains(3) << std::endl;
        std::cout << "2.6)  " << std::boolalpha << b.Contains(1) << std::endl;
        std::cout << "2.7)  " << std::boolalpha << b.Contains(2) << std::endl;
        std::cout << "2.8)  " << std::boolalpha << b.Contains(3) << std::endl;

        // 依序印出 :
        // 2.1)  {1, 3, 5}
        // 2.2)  {1, 3, 5}
        // 2.3)  true
        // 2.4)  false
        // 2.5)  true
        // 2.6)  true
        // 2.7)  false
        // 2.8)  true
    }

    // 實作 operator== 運算，回傳兩個集合是否相等
    // 實作 operator!= 運算，回傳兩個集合是否不相等
    // - 兩個集合相等只要所有元素值與個數相同就可以，不在乎元素存放的前後順序
    {
        ImmutableSet<int> a = {1, 3};
        ImmutableSet<int> b = {1, 3, 2};
        ImmutableSet<int> c = {3, 1};

        std::cout << "3.1)  " << std::boolalpha << (a == a) << std::endl;
        std::cout << "3.2)  " << std::boolalpha << (a == b) << std::endl;
        std::cout << "3.3)  " << std::boolalpha << (a == c) << std::endl;
        std::cout << "3.4)  " << std::boolalpha << (a != a) << std::endl;
        std::cout << "3.5)  " << std::boolalpha << (a != b) << std::endl;
        std::cout << "3.6)  " << std::boolalpha << (a != c) << std::endl;

        ImmutableSet<std::string> d = {"1.5", "3.4"};
        ImmutableSet<std::string> e = {"1.5", "3.5", "2.5"};
        ImmutableSet<std::string> f = {"3.5", "1.5"};

        std::cout << "3.7)  " << std::boolalpha << (d == e) << std::endl;
        std::cout << "3.8)  " << std::boolalpha << (d == f) << std::endl;
        std::cout << "3.9)  " << std::boolalpha << (d != e) << std::endl;
        std::cout << "3.10) " << std::boolalpha << (d != f) << std::endl;

        // 依序印出:
        // 3.1)  true
        // 3.2)  false
        // 3.3)  true
        // 3.4)  false
        // 3.5)  true
        // 3.6)  false
        // 3.7)  false
        // 3.8)  false
        // 3.9)  true
        // 3.10) true
    }

    // 實作 operator<= 運算，回傳集合 lhs 中所有元素值是不是都出現在集合 rhs 中
    // 實作 operator>= 運算，回傳集合 rhs 中所有元素值是不是都出現在集合 lhs 中
    {
        ImmutableSet<int> a = {1, 3};
        ImmutableSet<int> b = {1, 3, 2};
        ImmutableSet<int> c = {3, 2};

        std::cout << "4.1)  " << std::boolalpha << (a <= b) << std::endl;
        std::cout << "4.2)  " << std::boolalpha << (a >= b) << std::endl;
        std::cout << "4.3)  " << std::boolalpha << (a <= c) << std::endl;
        std::cout << "4.4)  " << std::boolalpha << (a >= c) << std::endl;
        std::cout << "4.5)  " << std::boolalpha << (b <= c) << std::endl;
        std::cout << "4.6)  " << std::boolalpha << (b >= c) << std::endl;

        ImmutableSet<std::string> d = {"1.5", "3.5"};
        ImmutableSet<std::string> e = {"1.5", "3.5", "2.4"};
        ImmutableSet<std::string> f = {"3.5", "2.5"};

        std::cout << "4.7)  " << std::boolalpha << (d <= e) << std::endl;
        std::cout << "4.8)  " << std::boolalpha << (d >= e) << std::endl;
        std::cout << "4.9)  " << std::boolalpha << (d <= f) << std::endl;
        std::cout << "4.10) " << std::boolalpha << (d >= f) << std::endl;
        std::cout << "4.11) " << std::boolalpha << (e <= f) << std::endl;
        std::cout << "4.12) " << std::boolalpha << (e >= f) << std::endl;

        // 依序印出:
        // 4.1)  true
        // 4.2)  false
        // 4.3)  false
        // 4.4)  false
        // 4.5)  false
        // 4.6)  true
        // 4.7)  true
        // 4.8)  false
        // 4.9)  false
        // 4.10) false
        // 4.11) false
        // 4.12) false
    }

    // 實作 operator& 運算，回傳一個新集合表示交集：包含所有集合 lhs 與 rhs 中都出現的元素值
    // 實作 operator| 運算，回傳一個新集合表示聯集：包含所有集合 lhs 或 rhs 中有出現的元素值
    // 實作 IsDisjointWith 方法，回傳與集合 other 是否交集為空
    {
        ImmutableSet<int> a = {1, 3};
        ImmutableSet<int> b = {1, 3, 2};
        ImmutableSet<int> c = {3, 2};
        ImmutableSet<int> g = {4, 5};

        std::cout << "5.1)  " << (a & a) << std::endl;
        std::cout << "5.2)  " << (a & b) << std::endl;
        std::cout << "5.3)  " << (a & c) << std::endl;
        std::cout << "5.4)  " << (a | a) << std::endl;
        std::cout << "5.5)  " << (a | b) << std::endl;
        std::cout << "5.6)  " << (a | c) << std::endl;
        std::cout << "5.7)  " << std::boolalpha << a.IsDisjointWith(b) << std::endl;
        std::cout << "5.8)  " << std::boolalpha << a.IsDisjointWith(g) << std::endl;

        ImmutableSet<std::string> d = {"1.5", "3.5"};
        ImmutableSet<std::string> e = {"1.5", "3.5", "2.4"};
        ImmutableSet<std::string> f = {"3.5", "2.5"};
        ImmutableSet<std::string> h = {"4.5", "5.5"};

        std::cout << "5.9)  " << (d & e) << std::endl;
        std::cout << "5.10) " << (d & f) << std::endl;
        std::cout << "5.11) " << (d | e) << std::endl;
        std::cout << "5.12) " << (d | f) << std::endl;
        std::cout << "5.13) " << (d | e | f) << std::endl;
        std::cout << "5.14) " << std::boolalpha << d.IsDisjointWith(e) << std::endl;
        std::cout << "5.15) " << std::boolalpha << d.IsDisjointWith(h) << std::endl;

        // 依序印出:
        // 5.1)  {1, 3}
        // 5.2)  {1, 3}
        // 5.3)  {3}
        // 5.4)  {1, 3}
        // 5.5)  {1, 2, 3}
        // 5.6)  {1, 2, 3}
        // 5.7)  false
        // 5.8)  true
        // 5.9)  {1.5, 3.5}
        // 5.10) {3.5}
        // 5.11) {1.5, 2.4, 3.5}
        // 5.12) {1.5, 2.5, 3.5}
        // 5.13) {1.5, 2.4, 2.5, 3.5}
        // 5.14) false
        // 5.15) true
    }
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { /* HIDDEN */ }

// [YOUR CODE WILL BE PLACED HERE]
template<typename T>
ImmutableSet<T>::ImmutableSet(std::initializer_list<T> l) : repr_(l) {
    std::sort(repr_.begin(), repr_.end());
    auto last = std::unique(repr_.begin(), repr_.end());
    repr_.erase(last, repr_.end());
}

template<typename T>
ImmutableSet<T>::ImmutableSet(const std::vector<T>& values) {
    repr_ = values;
    std::sort(repr_.begin(), repr_.end());
    auto last = std::unique(repr_.begin(), repr_.end());
    repr_.erase(last, repr_.end());
}

template<typename T>
bool ImmutableSet<T>::Contains(const T& val) const {
    return std::binary_search(repr_.begin(), repr_.end(), val);
}

template<typename T>
bool operator==(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs) {
    return lhs.Size() == rhs.Size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T>
bool operator!=(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs) {
    return !(lhs == rhs);
}

template<typename T>
bool operator<=(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs) {
    return std::includes(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
}

template<typename T>
bool operator>=(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs) {
    return(rhs <= lhs);
}

template<typename T>
ImmutableSet<T> operator&(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs) {
    std::vector<T> intersection;
    std::set_intersection(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::back_inserter(intersection));
    return ImmutableSet<T>{std::move(intersection)};
}

template<typename T>
ImmutableSet<T> operator|(const ImmutableSet<T>& lhs, const ImmutableSet<T>& rhs) {
    std::vector<T> union_set(lhs.Size() + rhs.Size());
    auto it = std::set_union(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), union_set.begin());
    union_set.resize(it - union_set.begin());
    return ImmutableSet<T>{std::move(union_set)};
}

template<typename T>
bool ImmutableSet<T>::IsDisjointWith(const ImmutableSet<T>& other) const {
    for(const auto& elem : *this) {
        if(other.Contains(elem))
            return false;
    }
    return true;
}