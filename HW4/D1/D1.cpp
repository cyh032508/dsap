#include <string>
#include <vector>
#include <iostream>
#include <algorithm> // For Test
#include <random>    // For Test
#include <set>       // Changed to set
#include <unordered_map>


struct IStringDatabase {
    virtual void Add(const std::string&) = 0;
    virtual bool Remove(const std::string&) = 0;
    virtual std::vector<std::string> StartsWith(const std::string& prefix) = 0;
    virtual ~IStringDatabase() {}
};

#include <set> 
class StringDatabase : public IStringDatabase {
private:
    std::set<std::string> words;

public:
    void Add(const std::string& str) override {
        words.insert(str);
    }

    bool Remove(const std::string& str) override {
        auto it = words.find(str);
        if (it != words.end()) {
            words.erase(it);
            return true;
        }
        return false;
    }

    std::vector<std::string> StartsWith(const std::string& prefix) override {
        std::vector<std::string> result;
        auto lower = words.lower_bound(prefix);
        auto upper = words.upper_bound(prefix + char(127)); 

        for (auto it = lower; it != upper; ++it) {
            result.push_back(*it);
        }
        return result;
    }
};


// [YOUR CODE WILL BE PLACED HERE] 

void Test1(); // Sample1
void Test2(); // Add, StartsWith
void Test3(); // Add, StartsWith
void Test4(); // Add, StartsWith
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

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << '[';
    if (v.size() > 0) {
        os << v[0];
        for (size_t i = 1; i < v.size(); ++i) {
            os << ", " << v[i];
        }
    }
    return os << ']';
}

namespace Feis {}

void Test1() {
    IStringDatabase&& db = StringDatabase();
    db.Add("Helloworld");
    db.Add("HelloWorld");
    db.Add("Hello");
    db.Add("Helloworld");
    db.Add("Hello");
    db.Add("Hi");

    std::cout << "01) " << db.StartsWith("Hello") << std::endl;
    std::cout << "02) " << db.StartsWith("hello") << std::endl;
    std::cout << "03) " << db.StartsWith("H") << std::endl;

    db.Remove("Helloworld");
    db.Remove("Hi");
    std::cout << "04) " << db.StartsWith("H") << std::endl;
}

void Test2() {}
void Test3() {}
void Test4() {}
void Test5() {}
void Test6() {}
void Test7() {}


/*
01) [Hello, HelloWorld, Helloworld]
02) []
03) [Hello, HelloWorld, Helloworld, Hi]
04) [Hello, HelloWorld]
*/