// [YOUR CODE WILL BE PLACED HERE] 
#include <iostream>
struct c {
    unsigned int c : 3;
};

class PocketCube {
private:
    c cube[6 * 8];
public:
    PocketCube();
    PocketCube RotateFront();
    PocketCube RotateRight();
    PocketCube RotateDown();
    friend std::ostream& operator<<(std::ostream& os, const PocketCube& c);
};

void swap(c& a, c& b){
    c temp = a;  
    a = b;                 
    b = temp;              
}

PocketCube::PocketCube() {
    cube[0].c = static_cast<unsigned int>(0);
    cube[1].c = static_cast<unsigned int>(0);
    cube[2].c = static_cast<unsigned int>(1); 
    cube[3].c = static_cast<unsigned int>(1);
    cube[4].c = static_cast<unsigned int>(0);
    cube[5].c = static_cast<unsigned int>(0);
    cube[6].c = static_cast<unsigned int>(0);
    cube[7].c = static_cast<unsigned int>(0);

    cube[8].c = static_cast<unsigned int>(0);
    cube[9].c = static_cast<unsigned int>(0);
    cube[10].c = static_cast<unsigned int>(1);
    cube[11].c = static_cast<unsigned int>(1);
    cube[12].c = static_cast<unsigned int>(0);
    cube[13].c = static_cast<unsigned int>(0);
    cube[14].c = static_cast<unsigned int>(0);
    cube[15].c = static_cast<unsigned int>(0);

    cube[16].c = static_cast<unsigned int>(2); 
    cube[17].c = static_cast<unsigned int>(2);
    cube[18].c = static_cast<unsigned int>(3); 
    cube[19].c = static_cast<unsigned int>(3);
    cube[20].c = static_cast<unsigned int>(4); 
    cube[21].c = static_cast<unsigned int>(4);
    cube[22].c = static_cast<unsigned int>(5); 
    cube[23].c = static_cast<unsigned int>(5);

    cube[24].c = static_cast<unsigned int>(2);
    cube[25].c = static_cast<unsigned int>(2);
    cube[26].c = static_cast<unsigned int>(3);
    cube[27].c = static_cast<unsigned int>(3);
    cube[28].c = static_cast<unsigned int>(4);
    cube[29].c = static_cast<unsigned int>(4);
    cube[30].c = static_cast<unsigned int>(5);
    cube[31].c = static_cast<unsigned int>(5);

    cube[32].c = static_cast<unsigned int>(0);
    cube[33].c = static_cast<unsigned int>(0);
    cube[34].c = static_cast<unsigned int>(6); 
    cube[35].c = static_cast<unsigned int>(6);
    cube[36].c = static_cast<unsigned int>(0);
    cube[37].c = static_cast<unsigned int>(0);
    cube[38].c = static_cast<unsigned int>(0);
    cube[39].c = static_cast<unsigned int>(0);

    cube[40].c = static_cast<unsigned int>(0);
    cube[41].c = static_cast<unsigned int>(0);
    cube[42].c = static_cast<unsigned int>(6);
    cube[43].c = static_cast<unsigned int>(6);
    cube[44].c = static_cast<unsigned int>(0);
    cube[45].c = static_cast<unsigned int>(0);
    cube[46].c = static_cast<unsigned int>(0);
    cube[47].c = static_cast<unsigned int>(0);
}

PocketCube PocketCube::RotateFront() {
    swap(cube[11], cube[17]);
    swap(cube[17], cube[34]);
    swap(cube[34], cube[28]);
    swap(cube[10], cube[25]);
    swap(cube[25], cube[35]);
    swap(cube[35], cube[20]);
    swap(cube[18], cube[19]);
    swap(cube[18], cube[26]);
    swap(cube[26], cube[27]);
    return *this;
}

PocketCube PocketCube::RotateRight() {
    swap(cube[3], cube[19]);
    swap(cube[19], cube[35]);
    swap(cube[35], cube[30]);
    swap(cube[11], cube[27]);
    swap(cube[27], cube[43]);
    swap(cube[43], cube[22]);
    swap(cube[21], cube[20]);
    swap(cube[20], cube[28]);
    swap(cube[28], cube[29]);
    return *this;
}

PocketCube PocketCube::RotateDown() {
    swap(cube[27], cube[25]);
    swap(cube[25], cube[31]);
    swap(cube[31], cube[29]);
    swap(cube[26], cube[24]);
    swap(cube[24], cube[30]);
    swap(cube[30], cube[28]);
    swap(cube[35], cube[34]);
    swap(cube[34], cube[42]);
    swap(cube[42], cube[43]);
    return *this;
}

std::ostream& operator<<(std::ostream& os, const PocketCube& c) {
    int index = 0;
    for(int i = 0; i < 6; i++) {
        for(int j = 0; j < 8; j++) {
            if(c.cube[index].c == 0) {
                os << "-";
            } else if(c.cube[index].c == 1) {
                os << "W";
            } else if(c.cube[index].c == 2) {
                os << "O";
            } else if(c.cube[index].c == 3) {
                os << "G";
            } else if(c.cube[index].c == 4) {
                os << "R";
            } else if(c.cube[index].c == 5) {
                os << "B";
            } else if(c.cube[index].c == 6) {
                os << "Y";
            }
            index ++;
        }
        os << "\n";
    }
    return os;
}

#include <iostream>
#include <random>     // For Test
#include <vector>     // For Test

void Test1(); // Sample1
void Test2(); // All 
void Test3(); // RotateRight, RotateDown 
void Test4(); // 重複其中一種旋轉
void Test5(); // 同時有多個
void Test6(); // All
void Test7(); // RotateRight, RotateDown
void Test8(); // 重複其中一種旋轉
void Test9(); // 同時有多個
void Test10(); // 同時有多個

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = {Test1, Test2, Test3, Test4, Test5, Test6, Test7, Test8, Test9, Test10};
    f[id-1]();
}

void Test1() {
    PocketCube a, b, c, d;

    std::cout << a << std::endl;
    std::cout << a.RotateFront() << std::endl;
    std::cout << a << std::endl;

    std::cout << b << std::endl;
    std::cout << b.RotateDown() << std::endl;
    std::cout << b << std::endl;

    std::cout << c << std::endl;
    std::cout << c.RotateRight() << std::endl;
    std::cout << c << std::endl;

    std::cout << d.RotateFront().RotateRight().RotateDown().RotateRight() << std::endl;
    std::cout << PocketCube().RotateFront().RotateFront().RotateFront().RotateFront() << std::endl;
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }
void Test6() { Test2(); }
void Test7() { Test3(); }
void Test8() { Test4(); }
void Test9() { Test5(); }
void Test10() { Test5(); }