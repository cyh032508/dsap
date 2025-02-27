#include <cmath>
#include <string>
#include <iostream>
#include <map>

class Grade {
public:
    Grade() : rank("X"), score(-1) {};
    Grade(const char* str);
    Grade(const std::string str);
    Grade(const int num);
    Grade& operator= (const char* str);
    Grade& operator= (const std::string& str);
    Grade& operator= (const int num);
    std::string GetRank() const;
    int GetScores() const;
    bool operator>(const Grade& g) const;
    bool operator<(const Grade& g) const;
    bool operator>=(const Grade& g) const;
    bool operator<=(const Grade& g) const;
    bool operator==(const Grade& g) const;
    bool operator!=(const Grade& g) const;
    
    void Reset();
    operator bool() const;

    friend std::ostream& operator<<(std::ostream& os, const Grade& g);
    friend Grade Average(const Grade& g1, const Grade& g2);

private:
    std::string rank;
    int score;
};
Grade::Grade(const char* str) : rank(str) {
    if (rank == "A+") {
        score = 95;
    } else if (rank == "A") {
        score = 87;
    } else if (rank == "A-") {
        score = 82;
    } else if (rank == "B+") {
        score = 78;
    } else if (rank == "B") {
        score = 75;
    } else if (rank == "B-") {
        score = 70;
    } else if (rank == "C+") {
        score = 68;
    } else if (rank == "C") {
        score = 65;
    } else if (rank == "C-") {
        score = 60;
    } else if (rank == "F") {
        score = 50;
    } else {
        
        rank = "X";
        score = -1;
    }
}

Grade::Grade(const std::string str): rank(str){
    if (rank == "A+") {
        score = 95;
    } else if (rank == "A") {
        score = 87;
    } else if (rank == "A-") {
        score = 82;
    } else if (rank == "B+") {
        score = 78;
    } else if (rank == "B") {
        score = 75;
    } else if (rank == "B-") {
        score = 70;
    } else if (rank == "C+") {
        score = 68;
    } else if (rank == "C") {
        score = 65;
    } else if (rank == "C-") {
        score = 60;
    } else if (rank == "F") {
        score = 50;
    } else {
        
        rank = "X";
        score = -1;
    }
}
Grade::Grade(const int num) : score(num) {
    if (num >= 90 && num <= 100) {
        rank = "A+";
    } else if (num >= 85) {
        rank = "A";
    } else if (num >= 80) {
        rank = "A-";
    } else if (num >= 77) {
        rank = "B+";
    } else if (num >= 73) {
        rank = "B";
    } else if (num >= 70) {
        rank = "B-";
    } else if (num >= 67) {
        rank = "C+";
    } else if (num >= 63) {
        rank = "C";
    } else if (num >= 60) {
        rank = "C-";
    } else if (num < 60 && num >= 0) {
        rank = "F";
    } else if (num < 0 || num > 100) {
        rank = "X";
    }
}
std::ostream& operator<<(std::ostream& os, const Grade& g) {
    os << g.GetRank();
    return os;
}

Grade& Grade::operator= (const char* str) {
    rank = str;
    if (rank == "A+") {
        score = 95;
    } else if (rank == "A") {
        score = 87;
    } else if (rank == "A-") {
        score = 82;
    } else if (rank == "B+") {
        score = 78;
    } else if (rank == "B") {
        score = 75;
    } else if (rank == "B-") {
        score = 70;
    } else if (rank == "C+") {
        score = 68;
    } else if (rank == "C") {
        score = 65;
    } else if (rank == "C-") {
        score = 60;
    } else if (rank == "F") {
        score = 50;
    } else if (rank == "X"){
        score = -1;
    }
    return *this;
}

Grade& Grade::operator= (const std::string& str) {
    rank = str;
    if (rank == "A+") {
        score = 95;
    } else if (rank == "A") {
        score = 87;
    } else if (rank == "A-") {
        score = 82;
    } else if (rank == "B+") {
        score = 78;
    } else if (rank == "B") {
        score = 75;
    } else if (rank == "B-") {
        score = 70;
    } else if (rank == "C+") {
        score = 68;
    } else if (rank == "C") {
        score = 65;
    } else if (rank == "C-") {
        score = 60;
    } else if (rank == "F") {
        score = 50;
    } else if (rank == "X"){
        score = -1;
    }
    return *this;
}

Grade& Grade::operator= (const int num) {
    score = num;
    if (num >= 90 && num <= 100) {
        rank = "A+";
    } else if (num >= 85) {
        rank = "A";
    } else if (num >= 80) {
        rank = "A-";
    } else if (num >= 77) {
        rank = "B+";
    } else if (num >= 73) {
        rank = "B";
    } else if (num >= 70) {
        rank = "B-";
    } else if (num >= 67) {
        rank = "C+";
    } else if (num >= 63) {
        rank = "C";
    } else if (num >= 60) {
        rank = "C-";
    } else if (num < 60 && num >= 0) {
        rank = "F";
    } else {
        rank = "X";
    }
    
    return *this;
}

std::string Grade::GetRank() const{
    return rank;
}

int Grade::GetScores() const{
    if(rank == "X"){
        return 0;
    }else{
        return score;
    }
}
bool Grade::operator<(const Grade& g) const {
    if(this -> rank == g.rank) {
        return false;
    } else {
        if(this -> score < g.score) {
            return true;
        } else {
            return false;
        }
    }
}

bool Grade::operator>(const Grade& g) const {
    if(this -> rank == g.rank) {
        return false;
    } else {
        if(this -> score > g.score) {
            return true;
        } else {
            return false;
        }
    }
}

bool Grade::operator<=(const Grade& g) const {
    return !(*this > g);
}

bool Grade::operator>=(const Grade& g) const {
    return !(*this < g);
}

bool Grade::operator==(const Grade& g) const {
    if(this -> rank == g.rank){
        return true;
    } else if((this->rank == "X" || this->rank == "F") && (g.rank == "X" || g.rank == "F")){
        return true;
    } else {
        return false;
    }
}

bool Grade::operator!=(const Grade& g) const {
    return !(*this == g);
}



void Grade::Reset() {
    score = -1;
    rank = "X";
}

Grade::operator bool() const {
    return score >= 0;
}

Grade Average(const Grade& g1, const Grade& g2) {
    if(g1.rank == "X" || g2.rank == "X") {
        return Grade();
    } else {
        int totalScore = g1.score + g2.score;
        int average = (totalScore + 1) / 2; 
        return Grade(average);
    }
}

#include <cmath>
#include <string>
#include <iostream>
#include <map>

void Test1(); // sample
void Test2(); // constructor: string
void Test3(); // constructor: int
void Test4(); // constructor: int, GetRank, GetScores
void Test5(); // constructor: int, comparison
void Test6(); // constructor: int, Average, GetScores
void Test7(); // constructors: default, string, int, copy, casting: bool, Reset

int main() {
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6, Test7 };
    f[id-1]();
}

void Test1()
{
    // Grade 預設初始化後為『未給分』("X") 狀態
    {
        Grade g;
        std::cout << g << std::endl; // 此時印出 "X"
    }

    // Grade 可用 C 風格字串或 C++字串 (std::string 型態) 表示等第成績以進行初始化或賦值運算
    // - 合法的字串包含 "A+", "A-", "A", "B+", "B-", "B", "C+", "C-", "C", "F", "X"
    // - 使用不在上述的字串進行初始化或賦值運算為《未定義行為》(undefined behavior)
    {
        Grade g1 = "A";
        std::cout << g1 << std::endl; // 此時印出 "A"

        g1 = "B+";
        std::cout << g1 << std::endl; // 此時印出 "B+"

        Grade g2 = std::string("A+");
        std::cout << g2 << std::endl; // 此時印出 "A+"

        g2 = std::string("C");
        std::cout << g2 << std::endl; // 此時印出 "C"

        const Grade g3 = g2;
        std::cout << g3 << std::endl; // 此時印出 "C"
    }

    // Grade 可用整數 (int) 表示百分制成績後進行初始化或賦值
    // - 如果輸入的整數超過百分制成績範圍 [0, 100] 的話是《未定義行為》(undefined behavior)
    {
        Grade g = 100;
        std::cout << g << std::endl; // 此時印出 "A+"

        g = 75;
        std::cout << g << std::endl; // 此時印出 "B"
    }

    // Grade 物件提供 GetRank() 操作，回傳其等第字串 (std::string 型態)
    {
        const Grade g1 = "A";
        std::string g2 = g1.GetRank();
        std::cout << g2 << std::endl;             // 印出 "A"
    }

    // Grade 物件提供 GetScores() 操作，回傳百分制分數 (int 型態)
    {
        const Grade g1 = "A";
        std::cout << g1.GetScores() << std::endl; // 此時印出 "87"

        const Grade g2 = 53;
        std::cout << g2.GetScores() << std::endl; // 此時印出 "53"

        const Grade g3 = "X";
        std::cout << g3.GetScores() << std::endl; // 此時印出 "0"
    }

    // Grade 物件可進行比較運算 (>, <, <=, >=, ==, !=)，回傳 bool 型態值
    // - 比較時，不管百分制分數高低，而只以等第高低比較：A+ > A > A- > B+ > B > B- > C+ > C > C- > F == X
    // - 比較時，不管百分制分數高低，同等第表示相等：A+ 與 A+ 相等，A+ 與 A 不相等
    // - 此外，比較時，F 與 X 是相等的
    {
        const Grade g1 = "A", g2 = "A+", g3 = "B-", g4 = "B-", g5 = "F", g6 = "X";
        std::cout << std::boolalpha << (g3 == g4) << std::endl;  // 印出 "true"
        std::cout << std::boolalpha << (g1 != g2) << std::endl;  // 印出 "true"
        std::cout << std::boolalpha << (g1 < g2) << std::endl;   // 印出 "true"
        std::cout << std::boolalpha << (g1 > g2) << std::endl;   // 印出 "false"
        std::cout << std::boolalpha << (g3 <= g4) << std::endl;   // 印出 "true"
        std::cout << std::boolalpha << (g3 >= g4) << std::endl;   // 印出 "true"
        std::cout << std::boolalpha << (g1 <= g2) << std::endl;   // 印出 "true"
        std::cout << std::boolalpha << (g1 >= g2) << std::endl;   // 印出 "false"
        std::cout << std::boolalpha << (g5 == g6) << std::endl;  // 印出 "true"
        std::cout << std::boolalpha << (g4 > g5) << std::endl;   // 印出 "true"
        std::cout << std::boolalpha << (g6 < g2) << std::endl;   // 印出 "true"
        std::cout << std::boolalpha << (Grade(90) == Grade(100)) << std::endl;   // 印出 "true"
    }

    // 提供一般函式 Average(const Grade& g1, const Grade& g2) 回傳兩個成績 (g1 與 g2) 平均的結果
    // - 平均結果是將兩個成績的百分制分數 (g1 與 g2) 相加除二後，四捨五入到整數位。
    // - 平均結果需要在 [0, 100] 之間，否則為《未定義行為》(undefined behavior)
    // - g1 與 g2 如果原本有一個是未給分 ("X")，平均後為依舊需要是未給分 ("X")
    {
        Grade g1 = 80;
        Grade g2;
        //std::cout << g1 << Grade(40) << std::endl;
        std::cout << Average(g1, Grade(40)) << std::endl;      // 印出 "C-"
        std::cout << Average(g1, g2) << std::endl;             // 印出 "X"
        std::cout << Average(Grade(40), Grade(85)).GetScores() << std::endl; // 印出 "63"
    }

    // Grade 物件提供 Reset() 操作，將成績轉成未給分 ("X")
    {
        Grade g = "A+";
        g.Reset();
        std::cout << g << std :: endl; // 此時印出 "X"
    }

    // Grade 物件可隱性轉型成 bool 型態，表示是否已經給分**
    {
        const Grade g1 = "A+";
        bool isG1Graded = g1;
        std::cout << std::boolalpha << isG1Graded << std::endl; // 此時印出 "true"

        const Grade g2;
        bool isG2Graded = g2;
        std::cout << std::boolalpha << isG2Graded << std::endl; // 此時印出 "false"

        if (g1) {
            std::cout << "g1" << std::endl; // 印出 "g1"
        }

        if (g2) {
            std::cout << "g2" << std::endl; // 不會執行到
        }
    }
}

void Test2() {
    // HIDDEN
}

void Test3() {
    // HIDDEN
}

void Test4() {
    // HIDDEN
}

void Test5() {
    // HIDDEN
}

void Test6() {
    // HIDDEN
}

void Test7() {
    // HIDDEN
}