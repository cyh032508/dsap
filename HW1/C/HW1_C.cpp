#include <iostream>
#include <random>
#include <iomanip>

class USDollar {
private:
    double amount;
public:
    USDollar() :amount(0.00) {};
    USDollar(const char* num);
    USDollar(const std::string& num);
    USDollar(const int num);
    USDollar(float num);
    USDollar(double num);
    USDollar& operator=(const char* num);
    USDollar& operator=(const std::string& num);
    USDollar& operator=(const int num);
    USDollar& operator=(float num);
    USDollar& operator=(double num);
    USDollar operator+(const USDollar& rhs) const;
    USDollar operator-(const USDollar& rhs) const;
    operator double() const;
    bool operator==(const USDollar& rhs) const;
    bool operator!=(const USDollar& rhs) const;
    bool operator>(const USDollar& rhs) const;
    bool operator<(const USDollar& rhs) const;
    bool operator>=(const USDollar& rhs) const;
    bool operator<=(const USDollar& rhs) const;
    operator bool() const;
    friend std::ostream& operator<<(std::ostream& os, const USDollar& d);
};

USDollar::USDollar(const char* num) {
    std::string str = num;
    std::string substr1 = "$";
    std::string substr2 = ".";
    size_t pos1 = str.find(substr1);
    str = str.erase(pos1, 1);
    amount = std::stod(str);
    amount = std::round(amount*100) / 100;
}
USDollar::USDollar(const std::string& num) {
    std::string str = num;
    std::string substr1 = "$";
    std::string substr2 = ".";
    size_t pos1 = str.find(substr1);
    str = str.erase(pos1, 1);
    amount = std::stod(str);
    amount = std::round(amount*100) / 100;
}
USDollar::USDollar(const int num) {
    amount = static_cast<double>(num);
}
USDollar::USDollar(float num) {
    num = std::round(num*100) / 100;
    amount = num;
}
USDollar::USDollar(double num) {
    num = std::round(num*100) / 100;
    amount = num;
}
USDollar& USDollar::operator=(const char* num) {
    
    std::string str = num;
    std::string substr1 = "$";
    std::string substr2 = ".";
    size_t pos1 = str.find(substr1);
    str = str.erase(pos1, 1);
    amount = std::stod(str);
    amount = std::round(amount*100) / 100;
    return *this;
}

USDollar& USDollar::operator=(const std::string& num) {
    std::string str = num;
    std::string substr1 = "$";
    std::string substr2 = ".";
    size_t pos1 = str.find(substr1);
    str = str.erase(pos1, 1);
    amount = std::stod(str);
    amount = std::round(amount*100) / 100;
    return *this;
}

USDollar& USDollar::operator=(const int num) {
    amount = static_cast<double>(num);
    return *this;
}

USDollar& USDollar::operator=(float num) {
    num = std::round(num*100) / 100;
    amount = num;
    return *this;
}
USDollar& USDollar::operator=(double num) {
    num = std::round(num*100) / 100;
    amount = num;
    return *this;
}
USDollar USDollar::operator+(const USDollar& rhs) const {
    double result = this->amount + rhs.amount;
    result = std::round(result*100) / 100;
    return USDollar(result);
}
USDollar USDollar::operator-(const USDollar& rhs) const {
    double result = this->amount - rhs.amount;
    result = std::round(result*100) / 100;
    return USDollar(result);
}
USDollar::operator double() const{
    return amount;
}
bool USDollar::operator==(const USDollar& rhs) const {
    return this->amount == rhs.amount;
}
bool USDollar::operator!=(const USDollar& rhs) const {
    return !(*this == rhs);
}
bool USDollar::operator>(const USDollar& rhs) const {
    return this->amount > rhs.amount;
}
bool USDollar::operator<(const USDollar& rhs) const {
    return this->amount < rhs.amount;
}
bool USDollar::operator>=(const USDollar& rhs) const {
    return !(*this < rhs);
}
bool USDollar::operator<=(const USDollar& rhs) const {
    return !(*this > rhs);
}
USDollar::operator bool() const {
    return amount >= 0;
}

std::ostream& operator<<(std::ostream& os, const USDollar& d) {
    if(d.amount >= 0) {
        os << "$" << std::fixed << std::setprecision(2) << d.amount;
    } else {
        os << "-$" << std::fixed << std::setprecision(2) << -1 * d.amount;
    }
    return os;
}

#include <iostream>
#include <random>

void Test1(); // sample
void Test2(); // constructors
void Test3(); // constructors, +, -
void Test4(); // constructors, casting to double
void Test5(); // constructors, comparison
void Test6(); // constructors, +, -, comparison
void Test7(); // constructors, +, -. comparison, casting to bool

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = {Test1, Test2, Test3, Test4, Test5, Test6, Test7 };
    f[id-1]();
}

void Test1()
{
    // USDollar 物件在預設初始化後為『$0.00』
    {
        USDollar g;
        std::cout << g << std::endl; // 此時印出 "$0.00"
    }

    // USDollar 物件可用 C 風格字串 ("") 或 C++字串 (std::string 型態) 表示金額以進行初始化或賦值運算
    // - 金額一般格式為 "$#.##"
    // - 金額可以為負數，負數格式是在 "$" 符號前加上負號"-"。例如 "-$59.50"
    // - 金額一定帶兩位小數，不足兩位小數時後面要補 0。例如：使用 "$59.50" 而不是 "$59.5"
    // - 金額前方不會有 0，例如：使用 "$10.35" 而不是 "$010.35";
    // - 輸入的字串格式不正確時為《未定義行為》(undefined behavior)
    // - 金額 "$0.00" 為正數，而金額 "-$0.00" 不存在且不符合格式。
    // - 金額的範圍要在 [-1000.00, 1000.00] 之間，最多帶兩位小數。超過範圍或不合規則則為《未定義行為》(undefined behavior)
    {
        USDollar g1 = "$37.03";
        std::cout << g1 << std::endl; // 此時印出 "$37.03"

        g1 = "$100.00";
        std::cout << g1 << std::endl; // 此時印出 "$100.00"

        USDollar g2 = std::string("-$83.10");
        std::cout << g2 << std::endl; // 此時印出 "-$83.10"
        //std::cout << a.size() << std::endl; 

        g2 = std::string("$97.07");
        std::cout << g2 << std::endl; // 此時印出 "$97.07"
    }

    // USDollar 物件可用整數 (int) 表示金額後進行初始化或賦值
    // - 金額的範圍要在 [-1000, 1000] 之間。超過範圍或不合規則則為《未定義行為》(undefined behavior)
    {
        USDollar g = 80;
        std::cout << g << std::endl; // 此時印出 "$80.00"

        g = -75;
        std::cout << g << std::endl; // 此時印出 "-$75.00"
    }

    // USDollar 物件可用浮點數 (double 與 float) 表示金額後進行初始化或賦值
    // - 金額請四捨五入到小數點後第二位
    // - 四捨五入後的金額的範圍要在 [-1000.00, 1000.00] 之間。超過範圍或不合規則則為《未定義行為》(undefined behavior)
    {
        USDollar g = 83.1;
        std::cout << g << std::endl; // 此時印出 "$83.10"

        g = -87.995f;
        std::cout << g << std::endl; // 此時印出 "-$88.00"
    }

    // 提供 USDollar 物件間的加法 (+) 運算，回傳相加的結果
    // - 運算元與運算結果的範圍都要在 [-1000.00, 1000.00] 之間，超過範圍則為《未定義行為》(undefined behavior)
    {
        std::cout << (USDollar(0.1) + USDollar(0.2)) << std::endl; // 此時印出 "$0.30"
    }

    // 提供 USDollar 物件間的減法 (-) 運算，回傳相加的結果
    // - 運算元與運算結果的範圍都要在 [-1000.00, 1000.00] 之間，超過範圍則為《未定義行為》(undefined behavior)
    {
        std::cout << (USDollar(0.1) - USDollar(0.2)) << std::endl; // 此時印出 "-$0.10"
    }

    // USDollar 物件可隱性轉型成 double 型態物件
    {
        USDollar g1 = "$90.77";
        double g2 = g1;
        std::cout << g2 << std::endl; // 此時印出 "90.77"
    }

    // 提供 USDollar 物件間的比較運算 (>, <, ==, !=, >=, <=)，回傳比較的結果
    {
        auto g1 = USDollar(0.1);
        auto g2 = USDollar(0.2);

        std::cout << std::boolalpha;
        std::cout << (g1 == g2) << std::endl; // 此時印出 "false"
        std::cout << (g1 != g2) << std::endl; // 此時印出 "true"
        std::cout << (g1 >= g2) << std::endl; // 此時印出 "false"
        std::cout << (g1 <= g2) << std::endl; // 此時印出 "true"
        std::cout << (g1 > g2) << std::endl; // 此時印出 "false"
        std::cout << (g1 < g2) << std::endl; // 此時印出 "true"
    }

    // USDollar 物件可隱性轉型成 bool 型態物件，金額大於等於 "$0.00" 時為真 (true)，反之為非 (false)
    {
        USDollar g1 = "-$10.00";
        if (g1) {
            std::cout << "Rich" << std::endl;
        } else {
            std::cout << "Poor" << std::endl; // 印出 "Poor"
        }

        USDollar g2 = "$10.00";
        if (g2) {
            std::cout << "Rich" << std::endl; // 印出 "Rich"
        } else {
            std::cout << "Poor" << std::endl;
        }

        USDollar g3;
        if (g3) {
            std::cout << "Rich" << std::endl; // 印出 "Rich"
        } else {
            std::cout << "Poor" << std::endl;
        }
    }
   
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { /* HIDDEN */ }
void Test7() { /* HIDDEN */ }