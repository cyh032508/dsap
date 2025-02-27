#include <initializer_list>
#include <iostream>
#include <cassert>    // For Test
#include <random>     // For Test
#include <functional> // For Test

template<typename T>
struct ListNode {
    explicit ListNode(T val) : val{std::move(val)}, next{nullptr} {}
    T val;
    ListNode<T>* next;
};

template<typename T>
class List {
public:
    class ConstIterator {
        friend class List;
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
        friend class List;
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

    List();
    List(const List<T>&);
    List& operator=(const List<T>&);
    List(std::initializer_list<T>);
    ~List();

    [[nodiscard]] size_t Size() const { return size_; }
    [[nodiscard]] bool Empty() const { return head_ == nullptr; }

    void PushFront(const T&);
    void PopFront();

    void Clear();
    void Swap(List<T>& other);
    void Remove(const T& val);

    void Reverse();

    void Sort();

    void EraseAfter(Iterator pos);

    void InsertAfter(Iterator pos, const T& val);
    void InsertAfter(Iterator pos, const List<T>& other);

    void Erase(size_t pos);
    void Erase(Iterator pos);

    void Insert(size_t pos, const T& val);
    void Insert(size_t pos, const List<T>& other);

    void Insert(Iterator pos, const T& val);
    void Insert(Iterator pos, const List<T>& other);

    [[nodiscard]] Iterator begin();
    [[nodiscard]] ConstIterator begin() const;
    [[nodiscard]] Iterator end();
    [[nodiscard]] ConstIterator end() const;

private:
    size_t size_;
    ListNode<T>* head_;
};

template<typename T>
std::ostream& operator<<(std::ostream&, const List<T>&);

template<typename Iterator>
void Advance(Iterator& iter, size_t n = 1) {
    for (size_t i = 0; i < n; ++i) {
        ++iter;
    }
}

template<typename Iterator>
Iterator Next(Iterator iter, size_t n = 1) {
    for (size_t i = 0; i < n; ++i) {
        ++iter;
    }
    return iter;
}

void Test1(); // Sample1
void Test2(); // Clear, Swap, Remove
void Test3(); // EraseAfter
void Test4(); // InsertAfter(pos, val)
void Test5(); // InsertAfter(pos, other)
void Test6(); // Erase
void Test7(); // Insert
void Test8(); // Reverse
void Test9(); // Sort
void Test10(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = {
            Test1, Test2, Test3, Test4, Test5, Test6,
            Test7, Test8, Test9, Test10 };
    f[id-1]();
}

void Test1() {
    // 實作 Clear() 方法，移除列表內所有元素
    // 實作 Swap(other) 方法，將所有元素與另一個列表 other 交換
    // 實作 Remove(val) 方法，移除列表內所有值為 val 的元素
    {
        List<int> l1 = {0, 1, 2, 3, 4, 5};
        List<int> l2 = {10, 11, 12};

        std::cout << "01) " << l1 << std::endl;
        std::cout << "02) " << l2 << std::endl;

        l1.Swap(l2);
        std::cout << "03) " << l1 << std::endl;
        std::cout << "04) " << l2 << std::endl;

        l1.Clear();
        std::cout << "05) " << l1 << std::endl;

        List<int> l = {3, 9, 1, 7, 5, 3, 9};

        l.Remove(3);
        std::cout << "06) " << l << std::endl;

        l.Remove(4);
        std::cout << "07) " << l << std::endl;

        // 依序印出:
        // 01) [0, 1, 2, 3, 4, 5]
        // 02) [10, 11, 12]
        // 03) [10, 11, 12]
        // 04) [0, 1, 2, 3, 4, 5]
        // 05) []
        // 06) [9, 1, 7, 5, 9]
        // 07) [9, 1, 7, 5, 9]
    }

    // 實作 EraseAfter(pos) 方法，移除 pos 迭代器所在位置的下一個元素
    // - 當下一個元素不存在時則不會有任何效果
    {
        List<int> l = {0, 1, 2, 3, 4};

        l.PopFront();
        std::cout << "08) " << l << std::endl;

        List<int>::Iterator p1 = l.begin();
        Advance(p1, 2);

        l.EraseAfter(p1);
        std::cout << "09) " << l << std::endl;

        List<int>::Iterator p2 = l.begin();

        l.EraseAfter(p2);
        std::cout << "10) " << l << std::endl;

        // 依序印出:
        // 08) [1, 2, 3, 4]
        // 09) [1, 2, 3]
        // 10) [1, 3]
    }

    // 實作 InsertAfter(pos, val) 方法，將新元素 val 插入在指定的 pos 位置後方
    // - 當 pos 位置與 end() 相等時為《未定義行為》
    {
        List<int> l1 = {0, 1, 2, 3, 4};
        List<int> l2 = {10, 11, 12};

        List<int>::Iterator p1 = Next(l1.begin(), 4);

        l1.InsertAfter(p1, 5);
        std::cout << "11) " << l1 << std::endl;

        l1.PushFront(6);
        std::cout << "12) " << l1 << std::endl;

        l1.InsertAfter(l1.begin(), 7);
        std::cout << "13) " << l1 << std::endl;


        // 依序印出:
        // 11) [0, 1, 2, 3, 4, 5]
        // 12) [6, 0, 1, 2, 3, 4, 5]
        // 13) [6, 7, 0, 1, 2, 3, 4, 5]
    }

    // 實作 InsertAfter(pos, other) 方法，將列表 other 插入在指定的 pos 位置後方
    // - 當 pos 位置與 end() 相等時為《未定義行為》
    {
        List<int> l1 = {0, 1, 2, 3, 4};
        List<int> l2 = {10, 11, 12};

        List<int>::Iterator p2 = Next(l1.begin());

        l1.InsertAfter(p2, l2);
        std::cout << "14) " << l1 << std::endl;
        std::cout << "15) " << l2 << std::endl;

        // 依序印出:
        // 14) [0, 1, 10, 11, 12, 2, 3, 4]
        // 15) [10, 11, 12]
    }

    // 實作 Erase(pos) 方法，移除位於 pos 位置的元素
    // - 當 pos 是 size_t 型態時，表示由 0 開始編號的位置
    // - 當 pos 是 Iterator 型態時，表示該迭代器所指向的位置
    {
        List<int> l = {0, 1, 2, 3, 4, 5, 6};

        l.Erase(0);
        std::cout << "16) " << l << std::endl;

        l.Erase(3);
        std::cout << "17) " << l << std::endl;

        l.Erase(1);
        std::cout << "18) " << l << std::endl;

        l.Erase(Next(l.begin(), 2));
        std::cout << "19) " << l << std::endl;

        // 依序印出:
        // 16) [1, 2, 3, 4, 5, 6]
        // 17) [1, 2, 3, 5, 6]
        // 18) [1, 3, 5, 6]
        // 19) [1, 3, 6]
    }

    // 實作 Insert(pos, val) 方法，將新元素 val 插入在指定編號的 pos 位置 (由 0 開始編號)
    // 實作 Insert(pos, other) 方法，將列表 other 插入在指定編號的 pos 位置 (由 0 開始編號)
    {
        List<int> l1 = {0, 1, 2, 3, 4};
        List<int> l2 = {10, 11, 12};

        l1.Insert(5, 5);
        std::cout << "20) " << l1 << std::endl;

        l1.Insert(0, 6);
        std::cout << "21) " << l1 << std::endl;

        l1.Insert(1, 7);
        std::cout << "22) " << l1 << std::endl;

        l1.Insert(2, l2);
        std::cout << "23) " << l1 << std::endl;
        std::cout << "24) " << l2 << std::endl;

        // 依序印出:
        // 20) [0, 1, 2, 3, 4, 5]
        // 21) [6, 0, 1, 2, 3, 4, 5]
        // 22) [6, 7, 0, 1, 2, 3, 4, 5]
        // 23) [6, 7, 10, 11, 12, 0, 1, 2, 3, 4, 5]
        // 24) [10, 11, 12]
    }

    // 實作 Reverse() 方法，反轉列表的元素順序
    {
        List<int> l = {0, 1, 3, 4, 5};
        l.Reverse();
        std::cout << "25) " << l << std::endl;

        l.Reverse();
        std::cout << "26) " << l << std::endl;

        // 依序印出:
        // 25) [5, 4, 3, 1, 0]
        // 26) [0, 1, 3, 4, 5]
    }

    // 實作 Sort() 方法，將列表的元素由小往大排
    {
        List<int> l = {3, 5, 1, 4, 2};

        l.Sort();
        std::cout << "27) " << l << std::endl;

        // 依序印出:
        // 27) [1, 2, 3, 4, 5]
    }
}

namespace Feis { /* HIDDEN */ }

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { /* HIDDEN */ }
void Test7() { /* HIDDEN */ }
void Test8() { /* HIDDEN */ }
void Test9() { /* HIDDEN */ }
void Test10() { /* HIDDEN */ }

template<typename T>
std::ostream& operator<<(std::ostream& os, const List<T>& l) {
    os << '[';
    for (auto p = l.begin(); p != l.end(); ++p) {
        if (p != l.begin()) {
            os << ", ";
        }
        os << *p;
    }
    os << ']';
    return os;
}

template<typename T>
List<T>::List() : size_{0}, head_{nullptr} {
}

template<typename T>
List<T>::List(const List<T>& other) {
    ListNode<T> temp(T{});
    ListNode<T>* insertAt = &temp;
    for (auto p = other.begin(); p != other.end(); ++p) {
        insertAt->next = new ListNode<T>(*p);
        insertAt = insertAt->next;
    }
    head_ = temp.next;
    size_ = other.size_;
}

template<typename T>
List<T>& List<T>::operator=(const List<T>& other) {
    if (this == &other) {
        return *this;
    }
    List<T> temp = other;
    std::swap(head_, temp.head_);
    std::swap(size_, temp.size_);
    return *this;
}

template<typename T>
List<T>::~List() {
    while (!Empty()) {
        PopFront();
    }
}

template<typename T>
List<T>::List(std::initializer_list<T> l) {
    ListNode<T> temp(T{});
    ListNode<T>* prev = &temp;
    for (const auto& v : l) {
        prev->next = new ListNode<T>(v);
        prev = prev->next;
    }
    head_ = temp.next;
    size_ = l.size();
}

template<typename T>
void List<T>::PushFront(const T& val) {
    ListNode<T>* temp = head_;
    head_ = new ListNode(val);
    head_->next = temp;
    ++size_;
}

template<typename T>
void List<T>::PopFront() {
    ListNode<T>* temp = head_->next;
    delete head_;
    head_ = temp;
    --size_;
}

template<typename T>
typename List<T>::ConstIterator List<T>::begin() const {
    return head_;
}

template<typename T>
typename List<T>::Iterator List<T>::begin() {
    return head_;
}

template<typename T>
typename List<T>::ConstIterator List<T>::end() const {
    return nullptr;
}

template<typename T>
typename List<T>::Iterator List<T>::end() {
    return nullptr;
}

// [YOUR CODE WILL BE PLACED HERE]

#include <vector>
#include <algorithm>

template<typename T>
void List<T>::Clear() {
    while(head_ != nullptr) {
        ListNode<T>* temp = head_;
        head_ = head_->next;
        delete temp;
    }
    size_ = 0;
}

template<typename T>
void List<T>::Swap(List<T>& other) {
    ListNode<T>* temp = this->head_;
    this->head_ = other.head_;
    other.head_ = temp;
    size_t temp_size = this->size_;
    this->size_ = other.size_;
    other.size_ = temp_size;
}

template<typename T>
void List<T>::Remove(const T& val) {
    ListNode<T>* current = this->head_;
    ListNode<T>* prev = nullptr;
    while(current != nullptr) {
        if(current->val == val) {
            if(prev == nullptr) {
                head_ = current->next;
            } else {
                prev->next = current->next;
            }
            ListNode<T>* temp = current;
            current = current->next;
            delete temp;
            size_--;
        } else {
            prev = current;
            current = current->next;
        }
    }
}

template<typename T>
void List<T>::EraseAfter(Iterator pos) {
    if(pos.ptr_ == nullptr || pos.ptr_->next == nullptr)
        return;
    ListNode<T>* temp = pos.ptr_->next;
    pos.ptr_->next = temp->next;
    temp->next = nullptr;
    delete(temp);
    size_--;
}

template<typename T>
void List<T>::InsertAfter(Iterator pos, const T& val) {
    if(pos == nullptr) 
        return;
    ListNode<T>* new_node = new ListNode<T>(val);
    ListNode<T>* temp = pos.ptr_->next;
    pos.ptr_->next = new_node;
    new_node->next = temp;
    size_++;  
}

template<typename T>
void List<T>::InsertAfter(Iterator pos, const List<T>& other) {
    if(pos == nullptr) 
        return;
    if(other.Empty())
        return;
    ListNode<T>* current = pos.ptr_->next;
    ListNode<T>* prev = pos.ptr_;
    List<T> temp = other;
    ListNode<T>* temp_current = temp.head_;
    while(temp_current != nullptr) {
        ListNode<T>* new_node = new ListNode<T>(temp_current->val);
        prev->next = new_node;
        new_node->next = current;

        prev = new_node;
        current = new_node->next;
        size_++;
        temp_current = temp_current->next;
    }  
}

template<typename T> 
void List<T>::Erase(size_t pos) {
    if(pos >= size_)
        return;
    
    ListNode<T>* prev = nullptr;
    ListNode<T>* current = head_;

    for(int i = 0; i < pos; i++) {
        prev = current;
        current = current->next;
    }

    if(prev == nullptr) {
        head_ = current->next;
        current->next = nullptr;
    } else {
        prev->next = current->next;
        current->next = nullptr;
    }
    delete current;
    size_--;
}

template<typename T>
void List<T>::Erase(Iterator pos) {
    if(pos == nullptr)
        return;
    ListNode<T>* current = pos.ptr_;
    ListNode<T>* temp = pos.ptr_->next;
    ListNode<T>* prev = nullptr;

    if(pos.ptr_ == head_) {
        head_ = head_->next;
        current->next = nullptr;
        delete(current);
        size_--;
    } else {
        prev = head_;
        while(prev->next != pos.ptr_) {
            prev = prev->next;
        }
        prev->next = temp;
        current->next = nullptr;
        delete current;
        size_--;
    }
}

template<typename T>
void List<T>::Insert(size_t pos, const T& val) {
    if(pos > size_)
        return;
    
    ListNode<T>* prev = nullptr;
    ListNode<T>* current = head_;

    for(int i = 0; i < pos; i++) {
        prev = current;
        current = current->next;
    }
    ListNode<T>* new_node = new ListNode<T>(val);

    
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
void List<T>::Insert(size_t pos, const List<T>& other) {
    if(pos > size_)
        return;
    
    List<T> temp = other;
    ListNode<T>* current = head_;
    ListNode<T>* prev = nullptr;

    for(int i = 0; i < pos; i++) {
        prev = current;
        current = current->next;
    }

    ListNode<T>* temp_current = temp.head_;
    while(temp_current != nullptr) {
        ListNode<T>* new_node = new ListNode<T>(temp_current->val);
        if(prev == nullptr) {
            new_node->next = head_;
            head_ = new_node; 
        } else {
            prev->next = new_node;
            new_node->next = current;
        }
        prev = new_node;
        current = new_node->next;
        size_++;
        temp_current = temp_current->next;
    }
}

template<typename T>
void List<T>::Insert(Iterator pos, const T& val) {
    if(pos == nullptr)
        return;

    ListNode<T>* current = pos.ptr_;
    ListNode<T>* prev = nullptr;

    if(pos.ptr_ == head_) {
        ListNode<T>* new_node = new ListNode<T>(val);
        new_node->next = head_;
        head_ = new_node;
        size_++;
        return;
    }

    prev = head_;
    while(prev != nullptr && prev->next != current) {
        prev = prev->next;
    }

    if(prev == nullptr) {
        return;
    }

    ListNode<T>* new_node = new ListNode<T>(val);
    prev->next = new_node;
    new_node->next = current;
    size_++;
}

template<typename T>
void List<T>::Insert(Iterator pos, const List<T>& other) {
    if(pos == nullptr)
        return;
    
    ListNode<T>* current = pos.ptr_;
    ListNode<T>* prev = nullptr;
    List<T> temp = other;

    ListNode<T>* temp_current = temp.head_;
    while(temp_current != nullptr) {
        ListNode<T>* new_node = new ListNode<T>(temp_current->val);
        if(prev == nullptr) {
            new_node->next = head_;
            head_ = new_node;
        } else {
            prev->next = new_node;
            new_node->next = current;
        }
        prev = new_node;
        current = new_node->next;
        size_++;
        temp_current = temp_current->next;
    }
}

template<typename T>
void List<T>::Reverse() {
    if(head_ == nullptr || head_->next == nullptr) 
        return;
    ListNode<T>* prev = nullptr;
    ListNode<T>* current = head_;
    ListNode<T>* next = nullptr;
    while(current != nullptr) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    head_ = prev;
}

template<typename T>
void List<T>::Sort() {
    if(head_ == nullptr)
        return;
}