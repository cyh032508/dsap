#include <string>

class ITypewriter {
    public:
        virtual ~ITypewriter() {}

        virtual void Put(char c) = 0;
        virtual void Put(const char* s) = 0;
        virtual void Put(std::string s) = 0;

        virtual std::string ToString() const = 0;
        virtual size_t Size() const = 0;

        virtual void Backspace() = 0;
        virtual void Delete() = 0;

        virtual void ToggleMode() = 0;

        virtual void Left() = 0;
        virtual void Right() = 0;

        virtual void Home() = 0;
        virtual void End() = 0;

        virtual void ClearAll() = 0;
        virtual void ClearToEndOfLine() = 0;

};

// [YOUR CODE WILL BE PLACED HERE]
#include <cstring>
class Typewriter : public ITypewriter{
public:
        Typewriter() : string_("_"), cursor(0), mode(0) {};
        void Put(char c);
        void Put(const char* s);
        void Put(std::string s);

        std::string ToString() const;
        size_t Size() const;

        void Backspace();
        void Delete();

        void ToggleMode();

        void Left();
        void Right();

        void Home();
        void End();

        void ClearAll();
        void ClearToEndOfLine();
private:
    std::string string_;
    size_t cursor;
    int mode; //insert:0, replace:1
};

void Typewriter::Put(char c) {
    if(mode == 0) {
        string_.insert(cursor, 1, c);
        cursor++;
    } else {
        if(cursor == Size()) {
            string_.insert(cursor, 1, c);
            cursor++;
        } else {
            string_[cursor + 1] = c;
            std::swap(string_[cursor + 1], string_[cursor]);
            cursor++;
        }
    }
}

void Typewriter::Put(const char* s) {
    if(mode == 0) {
        string_.insert(cursor, s);
        cursor += std::strlen(s);
    } else {
        if(cursor == Size()) {
            string_.insert(cursor, s);
            cursor += std::strlen(s);
        } else {
            string_.erase(cursor + 1, std::strlen(s));
            string_.insert(cursor, s);
            cursor += std::strlen(s);
        }
    }
}

void Typewriter::Put(std::string s) {
    if (mode == 0) {
        string_.insert(cursor, s);
        cursor += s.length();
    } else {
        if (cursor == string_.length() - 1) { 
            string_.insert(cursor, s);
            cursor += s.length();
        } else {
            string_.erase(cursor + 1, s.length());
            string_.insert(cursor, s);
            cursor += s.length();
        }
    }
}


std::string Typewriter::ToString() const {
    return string_;
}
size_t Typewriter::Size() const {
    return string_.size() - 1;
}

void Typewriter::Backspace() {
    if(cursor == 0) 
        return;
    string_.erase(cursor - 1, 1);
    cursor--;
}
void Typewriter::Delete() {
    if(cursor < string_.size() - 1) {
        string_.erase(cursor + 1, 1);
    }
}

void Typewriter::ToggleMode() {
    if(mode == 0) {
        mode = 1;
        string_[cursor] = '|';
    } else if(mode == 1) {
        mode = 0;
        string_[cursor] = '_';
    }
}

void Typewriter::Left() {
    if(cursor == 0)
        return;
    std::swap(string_[cursor], string_[cursor - 1]);
    cursor--;
}

void Typewriter::Right() {
    if(cursor == string_.size() - 1)
        return;
    std::swap(string_[cursor], string_[cursor + 1]);
    cursor++;
}

void Typewriter::Home() {
    if(cursor == 0)
        return;
    string_.erase(cursor, 1);
    cursor = 0;
    if(mode == 0) {
        string_.insert(cursor, "_");
    } else {
        string_.insert(cursor, "|");
    }
    
}

void Typewriter::End() {
    if(cursor == string_.size() - 1)
        return;
    string_.erase(cursor, 1);
    cursor = string_.size(); 
    if(mode == 0) {
        string_.insert(cursor, "_");
    } else {
        string_.insert(cursor, "|");
    }
}


void Typewriter::ClearAll() {
    string_.clear();
    if(mode == 0) {
        string_ = "_";
    } else {
        string_ = "|";
    }
    cursor = 0;
}

void Typewriter::ClearToEndOfLine() {
    string_.erase(cursor + 1);
}

#include <algorithm>
#include <cassert>    // For Test
#include <random>     // For Test
#include <functional> // For Test
#include <iostream>
#include <vector>

void Test1(); // Sample1
void Test2(); // ToString, Put
void Test3(); // ToString, Put, Size
void Test4(); // ToString, Put, Size, Backspace 
void Test5(); // ToString, Put, Size, Left, Right
void Test6(); // ToString, Put, Size, Left, Right, Home, End
void Test7(); // ToString, Put, Size, Left, Right, Home, Backspace, Delete
void Test8(); // ToString, Put, Size, Left, Right, Home, ClearAll, ClearToEnfOfLine
void Test9(); // ToString, Put, Size, Left, Right, Home, ToggleMode
void Test10(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6, Test7, Test8, Test9, Test10};
    f[id-1]();
}

namespace Feis {}

void Test1() {
    Typewriter t;
    ITypewriter& tw = t;

    std::cout << "01) " << tw.ToString() << std::endl;

    tw.Put('H');
    std::cout << "02) " << tw.ToString() << std::endl;

    std::cout << "03) " << tw.Size() << std::endl;

    tw.Put("ello");
    std::cout << "04) " << tw.ToString() << std::endl;

    std::cout << "05) " << tw.Size() << std::endl;

    tw.Put(std::string(" world!"));
    std::cout << "06) " << tw.ToString() << std::endl;

    std::cout << "07) " << tw.Size() << std::endl;

    tw.Backspace();
    std::cout << "08) " << tw.ToString() << std::endl;

    tw.Left();
    std::cout << "09) " << tw.ToString() << std::endl;

    for (size_t i = 1; i <= 9; ++i) {
        tw.Left();
    }
    std::cout << "10) " << tw.ToString() << std::endl;

    tw.ToggleMode();
    std::cout << "11) " << tw.ToString() << std::endl;

    tw.Put('a');
    std::cout << "12) " << tw.ToString() << std::endl;

    tw.Put("ppy");
    std::cout << "13) " << tw.ToString() << std::endl;

    tw.Put(std::string("!!!"));
    std::cout << "14) " << tw.ToString() << std::endl;

    tw.ClearToEndOfLine();
    std::cout << "15) " << tw.ToString() << std::endl;

    tw.Home();
    std::cout << "16) " << tw.ToString() << std::endl;

    tw.Left();
    std::cout << "17) " << tw.ToString() << std::endl;

    tw.Delete();
    std::cout << "18) " << tw.ToString() << std::endl;

    tw.ToggleMode();
    std::cout << "19) " << tw.ToString() << std::endl;

    tw.Put("Be ");
    std::cout << "20) " << tw.ToString() << std::endl;

    tw.Right();
    std::cout << "21) " << tw.ToString() << std::endl;

    tw.Delete();
    std::cout << "22) " << tw.ToString() << std::endl;

    tw.ClearAll();
    std::cout << "23) " << tw.ToString() << std::endl;

    tw.Put("Be happy");
    std::cout << "24) " << tw.ToString() << std::endl;

    tw.Home();
    std::cout << "25) " << tw.ToString() << std::endl;

    tw.End();
    std::cout << "26) " << tw.ToString() << std::endl;
}

void Test2() {}
void Test3() {}
void Test4() {}
void Test5() {}
void Test6() {}
void Test7() {}
void Test8() {}
void Test9() {}
void Test10() {}