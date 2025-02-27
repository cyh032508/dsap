#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <functional>
#include <cassert>

// 模板參數 TKey 的合法型態會支援：
// - 值初始化：歸零
// - 複製語意：複製建構與賦值
// - 同型態間的比較運算：==, !=, >, >=, <, <=
// 符合以上要求的可能型態有：int, double, std::string 等等
//
// 模板參數 TValue 的合法型態會支援：
// - 值初始化：歸零
// - 複製語意：複製建構與賦值
// 符合以上要求的可能型態有：int, double, std::string 等等
template<typename TKey, typename TValue>
class Dictionary {
public:
    struct KeyValuePair {
        TKey key;
        TValue value;
    };

    friend std::ostream& operator<<(std::ostream& os, const KeyValuePair& elem) {
        return os << elem.key << ": " << elem.value;
    }

    using ElemType = KeyValuePair;
    using Iterator = typename std::vector<ElemType>::iterator;
    using ConstIteartor = typename std::vector<ElemType>::const_iterator;

    Dictionary() = default;
    Dictionary(std::initializer_list<ElemType> l) : repr_{l} {}

    [[nodiscard]] TValue& operator[](const TKey& key);

    [[nodiscard]] size_t Size() const noexcept { return repr_.size(); }

    void RemoveKey(const TKey& key);
    void RemoveValue(const TValue& value);

    [[nodiscard]] bool Contains(const TKey& key) const;
    void Clear();
    void Swap(Dictionary<TKey, TValue>& other);

    [[nodiscard]] std::vector<TKey> Keys() const;

    [[nodiscard]] Iterator begin() { return repr_.begin(); }
    [[nodiscard]] ConstIteartor begin() const { return repr_.begin(); }
    [[nodiscard]] Iterator end() { return repr_.end(); }
    [[nodiscard]] ConstIteartor end() const { return repr_.end(); }

private:
    std::vector<ElemType> repr_;
};

template<typename TKey, typename TValue>
[[nodiscard]] Dictionary<TKey, TValue> operator|(const Dictionary<TKey, TValue>&, const Dictionary<TKey, TValue>&);

template<typename TKey, typename TValue>
TValue& Dictionary<TKey, TValue>::operator[](const TKey& key) {
    auto pos = 
            std::find_if(std::begin(repr_), std::end(repr_), [&key](const auto& p) { return p.key == key; });

    if (pos == std::end(repr_)) {
        repr_.push_back({key, TValue{}});
        return repr_.back().value;
    }

    return pos->value;
}

template<typename TKey, typename TValue>
std::ostream& operator<<(std::ostream& os, const Dictionary<TKey, TValue>& dict) {
    os << '{';
    if (dict.Size() > 0) {
        std::vector<typename Dictionary<TKey, TValue>::ElemType> kvps(dict.begin(), dict.end());
        std::sort(std::begin(kvps), std::end(kvps), [](const auto& a, const auto& b) { return a.key < b.key;} );
        os << kvps[0];
        for (size_t i = 1; i < kvps.size(); ++i) {
            os << ", " << kvps[i];
        }
    }
    os << '}';
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << '[';
    if (v.size() > 0) {
        os << v[0];
        for (size_t i = 1; i < v.size(); ++i) {
            os << ", " << v[i];
        }
    }
    os << ']';
    return os;
}

template<typename T>
std::vector<T> Sorted(std::vector<T>&& values) {
    std::sort(values.begin(), values.end());
    return std::move(values);
}

void Test1(); // Sample1
void Test2(); // Contains, Swap, Clear
void Test3(); // RemoveKey, Keys
void Test4(); // RemoveValue, operator|
void Test5(); // All

int main() {
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5 };
    f[id-1]();
}

namespace Feis { /* HIDDEN */ }

void Test1() {
    // 實作 Contains(key) 方法，回傳字典內是否有鍵值相等 (==) 於 key 的元素
    // 實作 Swap(other) 方法，將字典內容與字典 other 內容交換
    // 實作 Clear() 方法，將字典內容清空，大小變為 0
    {
        Dictionary<std::string, int> a = {{"Mary", 100}, {"John", 50}};
        std::cout << std::boolalpha << a.Contains("Mary") << std::endl;
        std::cout << std::boolalpha << a.Contains("Joe")  << std::endl;

        Dictionary<std::string, int> c = {{"Mary", 100}, {"John", 50}};
        Dictionary<std::string, int> d = {{"Joe", 60}};

        c.Swap(d);
        std::cout << c << std::endl;
        std::cout << d << std::endl;

        c.Clear();
        std::cout << c << std::endl;

        // 依序印出:
        // true
        // false
        // {Joe: 60}
        // {John: 50, Mary: 100}
        // {}
    }

    // 實作 Keys() 方法，回傳一個新陣列 (std::vector<T>)，包含所有鍵值
    // - 沒有限定回傳陣列中鍵值應擺放的先後順序，只要每個鍵值都剛好出現一次即可
    // 實作 RemoveKey(key) 方法，移除鍵值相等 (==) 於 key 的元素
    // - 如果沒有鍵值相等的元素，則不會發生任何作用
    {
        Dictionary<std::string, int> a = {{"Mary", 100}, {"John", 50}};

        std::cout << Sorted(a.Keys()) << std::endl;
        a.RemoveKey("Mary");

        std::cout << a << std::endl;
        a.RemoveKey("Joe");

        std::cout << Sorted(a.Keys()) << std::endl;
        a.RemoveKey("John");

        std::cout << a << std::endl;

        // 依序印出:
        // [John, Mary]
        // {John: 50}
        // [John]
        // {}
    }

    // 實作 RemoveValue(value) 方法，移除所有內容值相等於 value 的元素
    // - 如果沒有內容值相等的元素，則不會發生任何作用
    // 實作 operator|(lhs, rhs) 運算，產生一個新字典，包含所有出現在 lhs 與 rhs 的字典內容。
    // - 如果有一個鍵值在 lhs 與 rhs 都出現的話，則使用 rhs 內所對應到的內容值
    {
        const Dictionary<std::string, int> a = {{"Mary", 100}, {"John", 50}, {"Amy", 50}};
        const Dictionary<std::string, int> b = {{"Joe", 60}, {"Amy", 65}};

        Dictionary<std::string, int> c = a | b;
        Dictionary<std::string, int> d = b | a;

        std::cout << c  << std::endl;
        std::cout << d << std::endl;

        d.RemoveValue(50);
        std::cout << d << std::endl;

        d.RemoveValue(40);
        std::cout << d << std::endl;

        // 依序印出:
        // {Amy: 65, Joe: 60, John: 50, Mary: 100}
        // {Amy: 50, Joe: 60, John: 50, Mary: 100}
        // {Joe: 60, Mary: 100}
        // {Joe: 60, Mary: 100}
    }
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }

// [YOUR CODE WILL BE PLACED HERE]

template<typename TKey, typename TValue>
bool Dictionary<TKey, TValue>::Contains(const TKey& key) const {
    for(const auto& elem : repr_) {
        if(elem.key == key) {
            return true;
        }
    }
    return false;
}

template<typename TKey, typename TValue>
void Dictionary<TKey, TValue>::Swap(Dictionary<TKey, TValue>& other) {
    repr_.swap(other.repr_);
}

template<typename TKey, typename TValue>
void Dictionary<TKey, TValue>::Clear() {
     repr_.clear();
}

template<typename TKey, typename TValue>
std::vector<TKey> Dictionary<TKey, TValue>::Keys() const {
    std::vector<std::string> key;
    for(const auto& elem : repr_) {
        key.push_back(elem.key);
    }
    return key;
}

template<typename TKey, typename TValue>
void Dictionary<TKey, TValue>::RemoveKey(const TKey& key) {
    auto it = std::remove_if(repr_.begin(), repr_.end(), [&](const ElemType& elem) { return elem.key == key; });
    repr_.erase(it, repr_.end());
}

template<typename TKey, typename TValue>
void Dictionary<TKey, TValue>::RemoveValue(const TValue& value) {
    repr_.erase(std::remove_if(repr_.begin(), repr_.end(), [&](const ElemType& elem) { return elem.value == value; }), repr_.end());
}

template<typename TKey, typename TValue>
[[nodiscard]] Dictionary<TKey, TValue> operator|(const Dictionary<TKey, TValue>& lhs, const Dictionary<TKey, TValue>& rhs)
{
  Dictionary<TKey, TValue> l = lhs;
  Dictionary<TKey, TValue> r = rhs;
  vector<TKey> keys = rhs.Keys();
  for(auto key : keys)
    l[key] = r[key];
  return l;
}