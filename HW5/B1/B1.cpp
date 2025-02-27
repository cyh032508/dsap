#include <vector>
#include <iostream>
#include <iomanip>
#include <random>    // For Testing
#include <algorithm> // For Testing
struct TreeNode {
    int value;
    int left = -1;
    int right = -1;
};

std::ostream& operator<<(std::ostream& os, const TreeNode& node) {
    return os << '{' << node.value << ", " << node.left << ", " << node.right << '}';
}

std::ostream& operator<<(std::ostream& os, const std::vector<TreeNode>& nodes) {
    if (nodes.size() == 0) {
        return os << "[]";
    }
    os << '[' << nodes[0];
    for (std::size_t i = 1; i < nodes.size(); ++i) {
        os << ", " << nodes[i];
    }
    return os << ']';
}

bool Check(const std::vector<TreeNode>& nodes, int root);

void Test1(); // Sample
void Test2(); // root >= 0
void Test3(); // root == -1
void Test4(); // All
void Test5(); // root == -1
void Test6(); // All

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5, Test6};
    f[id-1]();
}

void Test1() {
    /***
     * a:       (6)
     *         /   \
     *      (-1)   (13)
     *        \     /\
     *        (2) (9)(17)
     ***/
    std::vector<TreeNode> a = {{9, -1, -1}, {17, -1, -1}, {-1, -1, 4}, {13, 0, 1}, {2, -1, -1}, {6, 2, 3}};
    /***
     * b:      (1)
     *         / \
     *       (2) (3)
     ***/            
    std::vector<TreeNode> b = {{1, 1, 2}, {2, -1, -1}, {3, -1, -1}};
    std::vector<TreeNode> c = {{2, 1, 2}, {1, -1, -1}, {3, -1, -1}, {4, -1, -1}};
    std::cout << "01) " << std::boolalpha << Check(a, -1) << std::endl;
    std::cout << "02) " << std::boolalpha << Check(a, -0)  << std::endl;
    std::cout << "03) " << std::boolalpha << Check(b, -1) << std::endl;
    std::cout << "04) " << std::boolalpha << Check(c, -1) << std::endl;    

    // 01) true
    // 02) false
    // 03) false
    // 04) false

}

namespace Feis { /* HIDDEN */ }

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() {  Test3(); }
void Test6() {  Test4(); }

// [YOUR CODE WILL BE PLACED HERE] 


#include <bits/stdc++.h>
bool Check(const std::vector<TreeNode>& nodes, int root) {
    if (root >= 0 && root < static_cast<int>(nodes.size())) {
        std::vector<bool> visited(nodes.size(), false); 
        std::vector<int> stack; 
        stack.push_back(root);

        while (!stack.empty()) {
            int current = stack.back();
            stack.pop_back();

            if (visited[current]) {
                return false;
            }
            visited[current] = true;

            if (nodes[current].left != -1) {
                if (nodes[current].left >= nodes.size() || nodes[current].value <= nodes[nodes[current].left].value) {
                    return false;
                }
                stack.push_back(nodes[current].left);
            }

            if (nodes[current].right != -1) {
                if (nodes[current].right >= nodes.size() || nodes[current].value >= nodes[nodes[current].right].value) {
                    return false;
                }
                stack.push_back(nodes[current].right);
            }
        }

        for (bool v : visited) {
            if (!v) {
                return false;
            }
        }

        return true;
    } else if (root == -1) {
        for (std::size_t i = 0; i < nodes.size(); ++i) {
            if (Check(nodes, static_cast<int>(i))) {
                return true;
            }
        }
        return false;
    } else {
        return false;
    }
}

