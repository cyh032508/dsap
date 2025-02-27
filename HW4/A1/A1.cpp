#include <iostream>
#include <string>
#include <stack>
#include <cmath>

using namespace std;

// 定義運算符的優先級
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 4; 
    return 0;
}

// 執行運算
double applyOperation(double a, double b, char op) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': 
            if (b == 0) {
                return 0;
            } else {
                return a / b;
            }
        case '^': 
            if (b < 0) {
                return 0;
            } else if(b == 0){
                if(a >= 0) {
                    return 1;
                } else {
                    return -1;
                }
            } else {
                return pow(a, b);
            }
        default: return 0;
    }
}


int evaluateExpression(string expression) {
    stack<int> values;
    stack<char> ops;
    bool lastWasOp = true; 
    
    for (int i = 0; i < expression.length(); i++) {
        if (expression[i] == ' ') continue; 
        
        if (isdigit(expression[i]) || (expression[i] == '-' && (i == 0 || lastWasOp))) {
            // 處理負數
            string numStr = "";
            if(expression[i] == '-') {
                numStr += '-';
                i++;
            }
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                numStr += expression[i++];
            }
            i--;
            int num = stoi(numStr);
            values.push(num);
            lastWasOp = false;
        }
        else if (expression[i] == '(') {
            ops.push(expression[i]);
            lastWasOp = true;
        }
        else if (expression[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                int b = values.top(); values.pop();
                int a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            if (!ops.empty()) ops.pop(); // 移除左括號
            lastWasOp = false;
        }
        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expression[i])) {
                int b = values.top(); values.pop();
                int a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOperation(a, b, op));
            }
            ops.push(expression[i]);
            lastWasOp = true;
        }
    }
    
    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(a, b, op));
    }
    
    return values.top();
}

void replacePowerOperator(string& expression) {
    size_t pos = expression.find("**");
    while (pos != string::npos) {
        expression.replace(pos, 2, "^");
        pos = expression.find("**", pos + 1);
    }
}

int main() {
    string expression;
    while(getline(cin, expression)){
        replacePowerOperator(expression);
        int result = evaluateExpression(expression);
        //int intResult = static_cast<int>(result); 
        cout << result << endl;
    }
    return 0;
}
