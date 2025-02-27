#include <initializer_list>
#include <iostream>
#include <cassert>    // For Test
#include <random>     // For Test
#include <functional> // For Test

template<typename T>
struct ListNode {
    explicit ListNode(const T& val) : val{val}, next{nullptr} {}
    explicit ListNode(T&& val) : val{std::move(val)}, next{nullptr} {}
    T val;
    ListNode<T>* next;
};

template<typename T>
struct IList {
    virtual T& operator[](size_t pos) = 0;
    virtual const T& operator[](size_t pos) const = 0;   
    virtual void Insert(size_t pos, const T& val) = 0;
    virtual void Insert(size_t pos, T&& val) = 0;
    virtual void Erase(size_t pos) = 0;
    virtual size_t Size() const = 0;
    virtual bool Empty() const = 0;
    virtual void Remove(const T& val) = 0;
    virtual void Clear() = 0;
    virtual ~IList() {}
};

template<typename T>
class SinglyLinkedList : public IList<T> {
public:
    SinglyLinkedList();
    SinglyLinkedList(const SinglyLinkedList<T>&);
    SinglyLinkedList(SinglyLinkedList<T>&&);
    SinglyLinkedList& operator=(const SinglyLinkedList<T>&);
    SinglyLinkedList& operator=(SinglyLinkedList<T>&&);
    SinglyLinkedList(std::initializer_list<T>);
    ~SinglyLinkedList();

    T& operator[](size_t pos) override;
    const T& operator[](size_t pos) const override;
    void Insert(size_t pos, const T& val)  override;
    void Insert(size_t pos, T&& val) override;
    void Erase(size_t pos) override;
    size_t Size() const override { return size_; }
    bool Empty() const override { return size_ == 0; }
    void Remove(const T& val) override;
    void Clear() override;

private:
    size_t size_;
    ListNode<T>* head_;
};

template<typename T>
std::ostream& operator<<(std::ostream&, const IList<T>&);

void Test1(); // Sample1
void Test2(); // Insert, Erase
void Test3(); // Insert, Erase, Clear, Remove
void Test4(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = {
            Test1, Test2, Test3, Test4};
    f[id-1]();
}

void Test1() {
    
    // 實作 Insert(pos, val) 方法，將新元素 val 插入在指定編號的 pos 位置
    // - pos 表示由 0 開始編號的位置
    {
        SinglyLinkedList<int> l1 = {0, 1, 2, 3, 4};

        l1.Insert(5, 5);
        std::cout << "10) " << l1 << std::endl;

        l1.Insert(0, 6);
        std::cout << "11) " << l1 << std::endl;

        l1.Insert(1, 7);
        std::cout << "12) " << l1 << std::endl;
        

        SinglyLinkedList<std::string> l2;
        
        l2.Insert(0, "Hello");
        l2.Insert(1, "world");
        std::cout << "13) " << l2 << std::endl;

        // 依序印出:
        // 10) [0, 1, 2, 3, 4, 5]
        // 11) [6, 0, 1, 2, 3, 4, 5]
        // 12) [6, 7, 0, 1, 2, 3, 4, 5]
        // 13) [Hello, world]
    }
    
    
    // 實作 Erase(pos) 方法，移除位於 pos 位置的元素
    // - pos 表示由 0 開始編號的位置
    {
        SinglyLinkedList<int> l = {0, 1, 2, 3, 4, 5, 6};

        l.Erase(0);
        std::cout << "20) " << l << std::endl;

        l.Erase(3);
        std::cout << "21) " << l << std::endl;

        l.Erase(1);
        std::cout << "22) " << l << std::endl;

        // 依序印出:
        // 20) [1, 2, 3, 4, 5, 6]
        // 21) [1, 2, 3, 5, 6]
        // 22) [1, 3, 5, 6]
    }

    // 實作 Clear() 方法，移除列表內所有元素
    // 實作 Remove(val) 方法，移除列表內所有值為 val 的元素
    {
        SinglyLinkedList<int> l1 = {0, 1, 2, 3, 4, 5};

        std::cout << "30) " << l1 << std::endl;

        l1.Clear();
        std::cout << "31) " << l1 << std::endl;

        SinglyLinkedList<int> l2 = {3, 9, 1, 7, 5, 3, 9};

        l2.Remove(3);
        std::cout << "32) " << l2 << std::endl;

        l2.Remove(4);
        std::cout << "33) " << l2 << std::endl;

        // 依序印出:
        // 30) [0, 1, 2, 3, 4, 5]
        // 31) []
        // 32) [9, 1, 7, 5, 9]
        // 33) [9, 1, 7, 5, 9]
    }
}

namespace Feis { /* HIDDEN */ }
 
void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }

template<typename T>
std::ostream& operator<<(std::ostream& os, const IList<T>& l) {
    os << '[';
    for (std::size_t i = 0; i < l.Size(); ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << l[i];
    }
    os << ']';
    return os;
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList() : size_{0}, head_{nullptr} {
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(const SinglyLinkedList<T>& other) {
    ListNode<T> temp(T{});
    ListNode<T>* insertAt = &temp;
    for (auto p = other.head_; p != nullptr; p = p->next) {
        insertAt->next = new ListNode<T>(p->val);
        insertAt = insertAt->next;
    }
    head_ = temp.next;
    size_ = other.size_;
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(SinglyLinkedList<T>&& other) {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
}

template<typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(const SinglyLinkedList<T>& other) {
    if (this == &other) {
        return *this;
    }
    SinglyLinkedList<T> temp = other;
    std::swap(head_, temp.head_);
    std::swap(size_, temp.size_);
    return *this;
}

template<typename T>
SinglyLinkedList<T>& SinglyLinkedList<T>::operator=(SinglyLinkedList<T>&& other) {
    std::swap(head_, other.head_);
    std::swap(size_, other.size_);
    return *this;
}

template<typename T>
SinglyLinkedList<T>::~SinglyLinkedList() {
    while (!Empty()) {
        Erase(0);
    }
}

template<typename T>
SinglyLinkedList<T>::SinglyLinkedList(std::initializer_list<T> l) {
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
T& SinglyLinkedList<T>::operator[](std::size_t pos) {
    ListNode<T>* temp = head_;
    for (std::size_t i = 0; i < pos; ++i) {
        temp = temp->next;
    }
    return temp->val;
}

template<typename T>
const T& SinglyLinkedList<T>::operator[](std::size_t pos) const {
    return const_cast<SinglyLinkedList<T>*>(this)->operator[](pos);
}


// [YOUR CODE WILL BE PLACED HERE]
template<typename T>
void SinglyLinkedList<T>::Insert(size_t pos, const T& val) {
    if(pos > size_)
        return;
    ListNode<T>* current = head_;
    ListNode<T>* prev = nullptr;
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
void SinglyLinkedList<T>::Insert(size_t pos, T&& val) {
    if(pos > size_)
        return;
    ListNode<T>* current = head_;
    ListNode<T>* prev = nullptr;
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
void SinglyLinkedList<T>::Erase(size_t pos) {
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
void SinglyLinkedList<T>::Clear() {
    while(head_ != nullptr) {
        ListNode<T>* temp = head_;
        head_ = head_->next;
        delete temp;
    }
    size_ = 0;
}

template<typename T>
void SinglyLinkedList<T>::Remove(const T& val) {
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