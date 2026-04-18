#pragma once
#include "BaseTree.h"
#include "BinaryTree.h"









class SplayTreeException : public TreeException
{
protected:
    using TreeException::msg;
    using TreeException::code;
public:
    SplayTreeException(const std::string& msg, const int code) : TreeException(msg, code) {}
    SplayTreeException(const SplayTreeException& ex) : TreeException(ex) {}
};


template <class key, class val>
class SplayTree : public BinaryTree<key, val> {
private:
    TreeNode<key, val>* rotateright(TreeNode<key, val>* p) { TreeNode<key, val>* q = p->left; p->left = q->right; q->right = p; p->fixheight(); q->fixheight(); return q; }
    TreeNode<key, val>* rotateleft(TreeNode<key, val>* q) { TreeNode<key, val>* p = q->right; q->right = p->left; p->left = q; q->fixheight(); p->fixheight(); return p; }

    TreeNode<key, val>* splay(TreeNode<key, val>* root, const key& k);


protected:

    virtual val find(const key& k);

public:
    using BinaryTree<key, val>::top;
    using BinaryTree<key, val>::comp;

    SplayTree(Compare<key>* c) :BinaryTree<key, val>(c){ comp = c; }
    SplayTree(Compare<key>* c, const SplayTree& tree) { comp = c; *this = tree; }
    ~SplayTree() { BinaryTree<key, val>::trace_del(top); top = nullptr; }

    SplayTree& operator=(const BinaryTree<key, val>& tree) { trace_del(top); BinaryTree<key, val>::trace_copy(tree.top, top);  return *this; }
    SplayTree& operator+=(const std::pair<key, val>& o) { BinaryTree<key, val>::add(o.first, o.second); return *this; }
    SplayTree& operator*=(const std::pair<key, val>& o) { find(o.first); *top->v = o.second; return *this; }
    SplayTree& operator-=(const key& k) { top = BinaryTree<key, val>::del(top, k); return *this; }

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }

    TreeNode<key, val>* get_top() { return top; }

    void Show(TreeNode<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        std::cout << dir << " [" << node->k << '_' << *node->v << "]\n";
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { BinaryTree<key, val>::trace_del(top); top = nullptr; delete this; }
};


template <class key, class val>
val SplayTree<key, val>::find(const key& k)
{
    TreeNode<key, val>* node = top;
    while(node)
    {
        if (comp->compare(k, node->k) < 0)
            node = node->left;
        else if (comp->compare(k, node->k) > 0)
            node = node->right;
        else
            break;
    }
    if(!node)
        throw SplayTreeException(std::string("Key not found (in function find AVL_tree)."), 0);
    top = splay(top, k);
    return *top->v;
}


template <class key, class val>
TreeNode<key, val>* SplayTree<key, val>::splay(TreeNode<key, val>* root, const key& k)
{
    if (root == nullptr || comp->compare(root->k, k)==0 )
        return root;

    if (comp->compare(root->k, k) > 0 )
    {
        if (root->left == nullptr) return root;

        if (comp->compare(root->left->k , k)>0 )
        {
            root->left->left = splay(root->left->left, k);
            root = rotateright(root);
        }
        else if (comp->compare(root->left->k, k) < 0)
        {
            root->left->right = splay(root->left->right, k);
            if (root->left->right != nullptr)
                root->left = rotateleft(root->left);
        }
        return (root->left == nullptr) ? root : rotateright(root);
    }
    else
    {
        if (root->right == nullptr) return root;

        if (comp->compare(root->right->k, k) > 0)
        {
            root->right->left = splay(root->right->left, k);

            if (root->right->left != nullptr)
                root->right = rotateright(root->right);
        }
        else if (comp->compare(root->right->k, k) < 0 )
        {
            root->right->right = splay(root->right->right, k);
            root = rotateleft(root);
        }

        return (root->right == nullptr) ? root : rotateleft(root);
    }
}
