#include <iostream>
#include <stack>
#include <vector>
#include <random>
#include <memory>
#include <iomanip>
#include <cassert>
#include <random>

template<typename T>
class IIterator {
public:
    virtual bool HasNext() const = 0;
    virtual void MoveNext() = 0;
    virtual const T& Current() const = 0;
    virtual ~IIterator() {}
};

enum class FibonacciCallStatus {
    kInitialized,
    kEntering,
    kLeftReturned,
    kRightReturned,
    kFinished,
};

struct FibonacciCallState {
    unsigned int n;
    FibonacciCallStatus status;
    unsigned int return_value;
};

class FibonacciCallStackIterator : public IIterator<std::stack<FibonacciCallState>> {
public:
    FibonacciCallStackIterator(std::stack<FibonacciCallState> stack) : current_{std::move(stack)} {}
    
    FibonacciCallStackIterator(const FibonacciCallStackIterator&) = default;
    FibonacciCallStackIterator(FibonacciCallStackIterator&&) = default;
    FibonacciCallStackIterator& operator=(const FibonacciCallStackIterator&) = default;
    FibonacciCallStackIterator& operator=(FibonacciCallStackIterator&&) = default;
    
    bool HasNext() const override;
    
    void MoveNext() override;

    const std::stack<FibonacciCallState>& Current() const override { return current_; }
private:
    std::stack<FibonacciCallState> current_;
};

class FibonacciCall {
public:
    FibonacciCall(unsigned int n) : n_{n} {}
    FibonacciCallStackIterator Iterator() const { 
        std::stack<FibonacciCallState> s;
        s.push({n_, FibonacciCallStatus::kInitialized});
        return FibonacciCallStackIterator(std::move(s));
    }
    unsigned int operator()() {
        auto iter = Iterator();
        while (iter.HasNext()) {
            iter.MoveNext();
        }
        return iter.Current().top().return_value;
    }
private:
    unsigned int n_;
};

std::ostream& operator<<(std::ostream& os, std::stack<FibonacciCallState> s) {
    if (s.empty()) {
        return os << "Empty :       []";
    }
    FibonacciCallState state = s.top();
    switch (state.status) {
        case FibonacciCallStatus::kInitialized:
            os << "Initialized    : ";
            break;
        case FibonacciCallStatus::kEntering:
            os << "Entering       : ";
            break;
        case FibonacciCallStatus::kLeftReturned:
            os << "Left Returned  : ";
            break;
        case FibonacciCallStatus::kRightReturned:
            os << "Right Returned : ";
            break;
        case FibonacciCallStatus::kFinished:
            os << "Finished       : " << state.return_value;
            return os;
    }
    std::stack<FibonacciCallState> reversed;
    while (!s.empty()) {
        reversed.push(s.top());
        s.pop();
    }
    os << '[' << reversed.top().n;
    reversed.pop();
    while (!reversed.empty()) {
        os << ", " << reversed.top().n;
        reversed.pop();
    }
    os << ']';
    return os;
}

void Skip(
    FibonacciCallStackIterator& iter,
    std::size_t skip) 
{
    std::size_t skipped = 0;
    while (iter.HasNext() && skipped < skip) {
        iter.MoveNext();
        ++skipped;
    }
}

void Test1(); // Sample1
void Test2(); // operator()
void Test3(); // Skip(), Iterator()
void Test4(); // Skip(), Iterator()

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4};
    f[id-1]();
}

void Test1() {
    {
        std::cout << "01) " << FibonacciCall(1)() << std::endl;
        std::cout << "02) " << FibonacciCall(2)() << std::endl;
        std::cout << "03) " << FibonacciCall(3)() << std::endl;
        std::cout << "04) " << FibonacciCall(4)() << std::endl;
        std::cout << "05) " << FibonacciCall(5)() << std::endl;
        std::cout << "06) " << FibonacciCall(6)() << std::endl;
        std::cout << "07) " << FibonacciCall(32)() << std::endl;
    }

    {
        FibonacciCallStackIterator iter = FibonacciCall(4).Iterator();
        std::cout << "10) " << iter.Current() << std::endl;
        std::size_t idx = 10;
        while (iter.HasNext()) {
            iter.MoveNext();
            std::cout << std::to_string(++idx) << ") " << iter.Current() << std::endl;
        }
    }

    {
        FibonacciCallStackIterator iter = FibonacciCall(4).Iterator();
        std::cout << "30) " << iter.Current() << std::endl;
        for (std::size_t idx = 31; idx <= 43; ++idx)
        {
            Skip(iter, 1);
            std::cout << std::to_string(idx) << ") " << iter.Current() << std::endl;
        }
    }

    {
        FibonacciCallStackIterator iter = FibonacciCall(8).Iterator();
        Skip(iter, 3);
        std::cout << "50) " << iter.Current() << std::endl;
        Skip(iter, 6);
        std::cout << "51) " << iter.Current() << std::endl;
        Skip(iter, 3);
        std::cout << "52) " << iter.Current() << std::endl;
        Skip(iter, 2);
        std::cout << "53) " << iter.Current() << std::endl;
    }
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }

// [YOUR CODE WILL BE PLACED HERE] 
void FibonacciCallStackIterator::MoveNext() {

}

bool FibonacciCallStackIterator::HasNext() const {

}