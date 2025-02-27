#include <iostream>
#include <random>
#include <functional>
#include <cassert>

// 模板參數 T 的合法型態會支援：
// - 值初始化：歸零
// - 複製語意：複製建構與賦值
// - 同型態間的比較運算：==, !=, >, >=, <, <=
// 符合以上要求的可能型態有：int, double, std::string 等等
template<typename T>
struct Node {
    T data;
    int meta{};
    Node<T>* left{};
    Node<T>* right{};
    explicit Node(const T& data) : data{data} {}
};

template<typename T>
class BalancedBinarySearchTreeSet {
public:
    BalancedBinarySearchTreeSet() = default;
    BalancedBinarySearchTreeSet(const BalancedBinarySearchTreeSet<T>&) = delete;
    ~BalancedBinarySearchTreeSet();
    BalancedBinarySearchTreeSet<T>& operator=(const BalancedBinarySearchTreeSet<T>&) = delete;
    [[nodiscard]] bool Contains(const T &) const;
    bool Add(const T &data);
    bool Remove(const T &);
    [[nodiscard]] std::vector<T> ToVector() const;
    [[nodiscard]] size_t Size() const { return size_; }
    [[nodiscard]] int Height() const;

private:
    Node<T> *root_{};
    size_t size_{};
};

void Test1(); // Sample1
void Test2(); // Add
void Test3(); // Add, ToVector
void Test4(); // Add, Remove, ToVector
void Test5(); // Add, Remove, ToVector

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    int id;
    std::cin >> id;
    void (*f[])() = { Test1, Test2, Test3, Test4, Test5 };
    f[id-1]();
}

namespace {
    template<typename T>
    int GetHeight(Node<T>* root) {
        if (!root) return -1;
        return std::max(GetHeight(root->left), GetHeight(root->right)) + 1;
    }

    template<typename T>
    bool ValidateHeight(BalancedBinarySearchTreeSet<T>& s) {
        return s.Height() <= (2 * log(s.Size() + 2) + 2);
    }
    template<typename T>
    bool Contains(Node<T>* root, const T& data) {
        if (!root) {
            return false;
        }
        if (data < root->data) {
            return Contains(root->left, data);
        }
        if (root->data < data) {
            return Contains(root->right, data);
        }
        return true;
    }
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const BalancedBinarySearchTreeSet<T>& s) {
    std::vector<T> v = s.ToVector();
    os << "{";
    for (size_t i = 0; i < v.size(); ++i) {
        if (i > 0) {
            os << ", ";
        }
        os << v[i];
    }
    os << "}";
    return os;
}

template<typename T>
bool BalancedBinarySearchTreeSet<T>::Contains(const T& data) const {
    return ::Contains(root_, data);
}

template<typename T>
int BalancedBinarySearchTreeSet<T>::Height() const {
    return GetHeight(root_);
}

namespace Feis { /* HIDDEN */ }

void Test1() {
    std::cout << std::boolalpha;

    // 實作 Add(val) 方法
    // - 如果要新增的值 val 尚未存在集合中，則將 val 加入集合並回傳 true
    // - 如果要新增的值 val 已經存在集合中，則回傳 false 而不發生其他作用
    {
        BalancedBinarySearchTreeSet<int> s;
        std::cout << "01) " << ValidateHeight(s) << std::endl;
        std::cout << "02) " << s.Contains(3) << std::endl;
        std::cout << "03) " << s.Add(3) << std::endl;
        std::cout << "04) " << s.Add(5) << std::endl;
        std::cout << "05) " << s.Contains(3) << std::endl;
        std::cout << "06) " << s.Add(3) << std::endl;
        std::cout << "07) " << s.Size() << std::endl;
        std::cout << "08) " << ValidateHeight(s) << std::endl;
    }

    // 實作 ToVector() 方法，回傳一個由集合內所有元素所構成的陣列
    // - 陣列中的元素值由小到大排序
    {
        BalancedBinarySearchTreeSet<int> s;
        s.Add(3);
        s.Add(9);
        s.Add(7);
        std::cout << "09) " << s << std::endl;
        s.Add(1);
        s.Add(5);
        std::cout << "10) " << s << std::endl;
        std::cout << "11) " << ValidateHeight(s) << std::endl;
    }

    // 實作 Remove(val) 方法，移除值為 val 的元素
    // - 如果要新增的值 val 已經存在集合中，則將 val 從集合移除並回傳 true
    // - 如果要新增的值 val 尚未存在集合中，則回傳 false 而不發生其他作用
    {
        BalancedBinarySearchTreeSet<int> s;
        s.Add(3);
        s.Add(9);
        s.Add(7);
        std::cout << "12) " << s.Remove(1) << std::endl;
        std::cout << "13) " << s.Remove(3) << std::endl;
        std::cout << "14) " << s << std::endl;
        std::cout << "15) " << s.Remove(7) << std::endl;
        std::cout << "16) " << s.Remove(9) << std::endl;
        std::cout << "17) " << s << std::endl;
    }
}

void Test2() { /* HIDDEN */ }
void Test3() { /* HIDDEN */ }
void Test4() { /* HIDDEN */ }
void Test5() { /* HIDDEN */ }

// [YOUR CODE WILL BE PLACED HERE]

template<typename T>
Node<T>* rightRotate(Node<T>* y) {
    Node<T>* x = y->left;
    Node<T>* T2 = x->right;

    x->right = y;
    y->left = T2;
    
    y->meta = std::max((y->left ? y->left->meta : 0), (y->right ? y->right->meta : 0)) + 1;
    x->meta = std::max((x->left ? x->left->meta : 0), (x->right ? x->right->meta : 0)) + 1;

    return x;
}

template<typename T>
Node<T>* leftRotate(Node<T>* x) {
    Node<T>* y = x->right;
    Node<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->meta = std::max((x->left ? x->left->meta : 0), (x->right ? x->right->meta : 0)) + 1;
    y->meta = std::max((y->left ? y->left->meta : 0), (y->right ? y->right->meta : 0)) + 1;

    return y; 
}

template<typename T>
int GetBalanceFactor(Node<T>* root) {
    if (!root) return 0;
    int left_height = root->left ? GetHeight(root->left) : -1;
    int right_height = root->right ? GetHeight(root->right) : -1;
    return left_height - right_height;
}

template<typename T>
bool BalancedBinarySearchTreeSet<T>::Add(const T &data) {
    if(Contains(data))
        return false;
    std::function<Node<T>*(Node<T>*, const T&)> addHelper = [&](Node<T>* root, const T& data) {
        if (root == nullptr) {
            Node<T>* new_node = new Node<T>(data);
            new_node->left = nullptr;
            new_node->right = nullptr;
            new_node->meta = 1;
            size_++;
            return new_node;
        }

        if (data < root->data) {
            root->left = addHelper(root->left, data);
        } else if (data > root->data) {
            root->right = addHelper(root->right, data);
        } else {
            return root; 
        }

        root->meta = GetHeight(root);

        int bf = GetBalanceFactor(root);

        if (bf > 1 && data < root->left->data) {
            return rightRotate(root);
        }
        if (bf > 1 && data > root->left->data) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (bf < -1 && data > root->right->data) {
            return leftRotate(root);
        }
        if (bf < -1 && data < root->right->data) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }; 
    root_ = addHelper(root_, data);
    return true;
}

template<typename T>
bool BalancedBinarySearchTreeSet<T>::Remove(const T &val) {
    std::function<Node<T>*(Node<T>* root, const T& val, bool& found)> removeHelper = [&](Node<T>* root, const T& val, bool& found){
        if (root == nullptr) {
            found = false;
            return static_cast<Node<T>*>(nullptr);
        }

        if (val < root->data) {
            root->left = removeHelper(root->left, val, found);
        } else if (val > root->data) {
            root->right = removeHelper(root->right, val, found);
        } else {
            found = true;
            if (root->left == nullptr) {
                Node<T>* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node<T>* temp = root->left;
                delete root;
                return temp;
            } else {
                Node<T>* successor = root->right;
                while (successor->left != nullptr) {
                    successor = successor->left;
                }
                root->data = successor->data;
                root->right = removeHelper(root->right, successor->data, found);
            }
        }

        if (root == nullptr) {
            return static_cast<Node<T>*>(nullptr);
        }

        root->meta = GetHeight(root);

        int bf = GetBalanceFactor(root);

        if (bf > 2 && GetBalanceFactor(root->left) >= 0) {
            return rightRotate(root);
        }
        if (bf > 2 && GetBalanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }
        if (bf < -2 && GetBalanceFactor(root->right) <= 0) {
            return leftRotate(root);
        }
        if (bf < -2 && GetBalanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root; 
    };
    bool found = false;
    root_ = removeHelper(root_, val, found);
    if (found) {
        size_--;
    }
    return found;
}

template<typename T>
std::vector<T> BalancedBinarySearchTreeSet<T>::ToVector() const {
    std::vector<T> result;
    std::function<void(Node<T>*)> inOrderTraversal = [&](Node<T>* node) {
        if (node == nullptr) {
            return;
        }
        inOrderTraversal(node->left);
        result.push_back(node->data);
        inOrderTraversal(node->right);
    };

    inOrderTraversal(root_);
    return result;
}

template<typename T>
BalancedBinarySearchTreeSet<T>::~BalancedBinarySearchTreeSet() {
    std::function<void(Node<T>*)> postOrderDelete = [&](Node<T>* node) {
        if (node == nullptr) {
            return;
        }
        postOrderDelete(node->left);
        postOrderDelete(node->right);
        delete node;
    };

    postOrderDelete(root_);
    root_ = nullptr; 
}