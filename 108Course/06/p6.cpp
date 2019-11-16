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
    friend class BinaryTree;
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
    BinaryTree(int _value) { root = new TreeNode(_value); }
    void clearTree(TreeNode* current) {
        if (current) return;
        clearTree(current->left);
        clearTree(current->right);
        delete current;
    }
    ~BinaryTree() { clearTree(root); }

    TreeNode* find(int _value, TreeNode* current) {
        if (current == nullptr)
            return nullptr;
        else if (_value == current->value)
            return current;
        else if (_value > current->value)
            return find(_value, current->right);
        else
            return find(_value, current->left);
    }

    TreeNode* deleteNode(TreeNode* current, TreeNode* toBeDeleted) {
        if (current) {
            if (toBeDeleted->value > current->value)
                current->right = deleteNode(current->right, toBeDeleted);
            else if (toBeDeleted->value < current->value)
                current->left = deleteNode(current->left, toBeDeleted);
            else {
                if (current->left == nullptr)
                    return current->right;
                else if (current->right == nullptr)
                    return current->left;

                TreeNode* successor = toBeDeleted->findSucc();
                current->value = successor->value;
                delete successor;
                current->right = deleteNode(current->right, toBeDeleted);
            }
            return current;
        } else
            return nullptr;
    }

    TreeNode* insertNode(TreeNode* current, int _value) {
        if (current == nullptr)
            return new TreeNode(_value);
        else if (_value < current->value)
            current->left = insertNode(current->left, _value);
        else
            current->right = insertNode(current->right, _value);
        return current;
    }

    void addNode(int _value) {
        TreeNode* isInTree = find(_value, root);
        root = ((isInTree) ? deleteNode(root, isInTree) : insertNode(root, _value));
    }

    void inorderRecording(vector<pair<int, pair<int, int>>>& recording) { inorderRecording(recording, root); }
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

   private:
    TreeNode* root;
};

int main() {
    int n;
    while (cin >> n) {
        BinaryTree* tree = new BinaryTree(n);
        vector<pair<int, pair<int, int>>> recording;
        while (cin >> n && n != -1)
            tree->addNode(n);
        tree->inorderRecording(recording);
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
        tree->~BinaryTree();
    }
}