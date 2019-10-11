#include <iostream>
#include <utility>
#include <vector>
using namespace std;

class BinaryTree;

class TreeNode {
   public:
    TreeNode(int _value) {
        this->value = _value;
        this->right = this->left = nullptr;
    }
    TreeNode(int _value, TreeNode* _left, TreeNode* _right) {
        this->value = value;
        this->left = _left;
        this->right = _right;
    }
    friend class BinaryTree;
    friend void inorderRecording(vector<pair<int, pair<int, int>>>& recording, const TreeNode*);
    TreeNode* findSucc() {
        TreeNode* current = this->right;
        while (current && current->left)
            current = current->left;
        return current;
    }

   private:
    int value;
    TreeNode *left, *right;
};

class BinaryTree {
   public:
    BinaryTree(int _value) {
        root = new TreeNode(_value);
    }
    const TreeNode* getRoot() const {
        return this->root;
    }
    TreeNode* find(int _value, TreeNode* current) {  //init current is root
        if (current == nullptr)
            return nullptr;
        else if (_value == current->value)
            return current;
        else if (_value > current->value) {
            preverious = current;
            return find(_value, current->right);
        } else {
            preverious = current;
            return find(_value, current->left);
        }
    }
    void deleteNode(TreeNode* _node) {
        if (_node == nullptr)
            cout << "you deleted a null node" << endl;
        else {
            if (_node->left == _node->right && _node->right == nullptr)
                delete _node;
            else if ((_node->left == nullptr && _node->right) ||
                     (_node->right == nullptr && _node->left)) {
                if (_node->value > preverious->value)
                    preverious->right = ((_node->left) ? _node->left : _node->right);  //must use find() first
                else
                    preverious->left = ((_node->left) ? _node->left : _node->right);
            } else {
                TreeNode* successor = _node->findSucc();
                _node->value = successor->value;
                deleteNode(successor);
            }
        }
    }
    void addNode(int _value) {
        preverious = root;
        TreeNode* isInTree = find(_value, root);
        if (isInTree)
            deleteNode(isInTree);
        else {
            TreeNode* newNode = new TreeNode(_value);
            if (newNode->value > preverious->value)
                preverious->right = newNode;
            else
                preverious->left = newNode;
        }
    }

   private:
    TreeNode *root, *preverious = nullptr;
};

void inorderRecording(vector<pair<int, pair<int, int>>>& recording, const TreeNode* current) {
    if (current) {
        inorderRecording(recording, current->left);
        int left, right;
        left = ((current->left) ? current->left->value : 0);
        right = ((current->right) ? current->right->value : 0);
        recording.push_back(make_pair(current->value, make_pair(left, right)));
        inorderRecording(recording, current->right);
    }
}

int main() {
    int n;
    cin >> n;
    BinaryTree* tree = new BinaryTree(n);
    vector<pair<int, pair<int, int>>> recording;
    while (cin >> n && n != -1) {
        tree->addNode(n);
        inorderRecording(recording, tree->getRoot());
        cout << "node: ";
        for (auto i : recording)
            cout << i.first << " ";
        cout << "\nleft: ";
        for (auto i : recording)
            cout << i.second.first << " ";
        cout << "\nright: ";
        for (auto i : recording)
            cout << i.second.second << " ";
        cout << "\n\n";
        recording.clear();
    }
}