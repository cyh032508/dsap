#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <random>

struct IOrderedNumberInputStream {
    virtual unsigned long long Get() = 0;
    virtual bool IsEndOfStream() const = 0;
    virtual ~IOrderedNumberInputStream() {}
};

class OrderedRandomNumberSequence : public IOrderedNumberInputStream {
public:
    OrderedRandomNumberSequence(size_t s, unsigned seed) : size_(s), curr_index_(0), random_generator_(seed), last_value_(0) {}
    OrderedRandomNumberSequence(const OrderedRandomNumberSequence&) = delete;
    OrderedRandomNumberSequence& operator=(const OrderedRandomNumberSequence&) = delete;
    unsigned long long Get() {
        ++curr_index_;
        last_value_ = last_value_ + random_generator_() % 100;
        return last_value_;
    }
    bool IsEndOfStream() const {
        return curr_index_ >= size_;
    }
private:
    size_t size_;
    size_t curr_index_;
    std::mt19937 random_generator_;
    unsigned long long last_value_;
};

// [YOUR CODE WILL BE PLACED HERE] 
#include <queue>

class CompositeOrderedNumberInputStream : public IOrderedNumberInputStream {
public:
    CompositeOrderedNumberInputStream(const std::vector<IOrderedNumberInputStream*>& inputs) : inputs_(inputs) {
        for (auto input : inputs_) {
            if (!input->IsEndOfStream()) {
                // 將每個輸入串流的第一個數字放入優先佇列中
                pq.push({input->Get(), input});
            }
        }
    }

    unsigned long long Get() override {
        // 取出優先佇列中最小的數字
        auto [value, stream] = pq.top();
        pq.pop();

        // 如果輸入串流還有更多的數字，則將下一個數字放入優先佇列中
        if (!stream->IsEndOfStream()) {
            pq.push({stream->Get(), stream});
        }

        return value;
    }

    bool IsEndOfStream() const override {
        // 優先佇列為空時表示所有輸入串流都已經取完了數字
        return pq.empty();
    }

private:
    // 優先佇列中的元素包含數字和對應的輸入串流指標
    struct Element {
        unsigned long long value;
        IOrderedNumberInputStream* stream;
        bool operator<(const Element& other) const {
            // 以數字的大小來比較
            return value > other.value; // 注意這裡是 ">"，因為我們希望最小的值在最前面
        }
    };

    // 優先佇列，根據 Element 中的 value 來自動排序
    std::priority_queue<Element> pq;

    // 輸入的串流源
    std::vector<IOrderedNumberInputStream*> inputs_;
};




//////////////////////

std::ostream& operator<<(std::ostream&, IOrderedNumberInputStream&);

void Test1(); // Sample1
void Test2(); // All
void Test3(); // All
void Test4(); // All
void Test5(); // All
void Test6(); // All
void Test7(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6, Test7};
    f[id-1]();
}

std::ostream& operator<<(std::ostream& os, IOrderedNumberInputStream& input) {
    bool isFirst = true;
    os << '[';
    while (!input.IsEndOfStream()) {
        if (!isFirst) {
            os << ", ";
        } else {
            isFirst = false;
        }
        os << input.Get();
    }
    return os << ']';
}

namespace Feis {}

void Test1() {
    // OrderedRandomNumberSequence
    {
        OrderedRandomNumberSequence a1(10, 10), b1(10, 20);
        std::cout << "01) " << a1 << std::endl;
        std::cout << "02) " << b1 << std::endl;
    }

    // CompositeOrderedNumberInputStream (1)
    {
        OrderedRandomNumberSequence a1(10, 10), b1(10, 20);
        CompositeOrderedNumberInputStream o1({&a1, &b1});
        std::cout << "03) " << o1 << std::endl;
    }

    // CompositeOrderedNumberInputStream (2)
    {
        OrderedRandomNumberSequence a1(10, 10), b1(10, 20), a2(20, 30), b2(0, 40), c2(10, 50), d1(5, 60);

        CompositeOrderedNumberInputStream o1({&a1, &b1});
        CompositeOrderedNumberInputStream o2({&a2, &b2, &c2});
        CompositeOrderedNumberInputStream o3({&d1});

        CompositeOrderedNumberInputStream o4({&o1, &o2, &o3});

        std::cout << "04) " << o4 << std::endl;
    }
}

void Test2() {}
void Test3() {}
void Test4() {}
void Test5() {}
void Test6() {}
void Test7() {}