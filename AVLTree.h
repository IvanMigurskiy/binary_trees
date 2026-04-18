#pragma once
#include "BaseTree.h"
#include "BinaryTree.h"




class AVLTreeException : public TreeException
{
protected:
    using TreeException::msg;
    using TreeException::code;
public:
    AVLTreeException(const std::string& msg, const int code) : TreeException(msg, code) {}
    AVLTreeException(const AVLTreeException& ex) : TreeException(ex) {}
};



template <class key, class val>
class AVLTree : public BinaryTree<key, val> {
private:

    TreeNode<key, val>* rotateright(TreeNode<key, val>* p) { TreeNode<key, val>* q = p->left; p->left = q->right; q->right = p; return q; }
    TreeNode<key, val>* rotateleft(TreeNode<key, val>* q) { TreeNode<key, val>* p = q->right; q->right = p->left; p->left = q; return p; }
    virtual TreeNode<key, val>* add(TreeNode<key, val>* p, const key& k, const val& v);
    virtual TreeNode<key, val>* del(TreeNode<key, val>* p, const key& k);


    TreeNode<key, val>* balance(TreeNode<key, val>* p);
    TreeNode<key, val>* findmin(TreeNode<key, val>* p);
    TreeNode<key, val>* removemin(TreeNode<key, val>* p);

public:
    using BinaryTree<key, val>::top;
    using BinaryTree<key, val>::comp;

    AVLTree(Compare<key>* c) :BinaryTree<key, val>(c){ comp = c; }
    AVLTree(Compare<key>* c, const AVLTree& tree) { comp = c; *this = tree; }
    ~AVLTree() { BinaryTree<key, val>::trace_del(top); top = nullptr; }


    AVLTree& operator=(const BinaryTree<key, val>& tree) { trace_del(top); trace_copy(tree.top, top);  return *this; }
    AVLTree& operator*=(const std::pair<key, val>& o) { BinaryTree<key,val>::edit(o.first, o.second); return *this; }
    AVLTree& operator+=(const std::pair<key, val>& o) { top = add(top, o.first, o.second); return *this; }
    AVLTree& operator-=(const key& k) { top = del(top, k); return *this; }

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }

    void Show(TreeNode<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        std::cout << dir << " [" << node->k << '_' << *node->v << "]\n";
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { BinaryTree<key, val>::trace_del(top); delete this; }
};




template <class key, class val>
TreeNode<key, val>* AVLTree<key, val>::balance(TreeNode<key, val>* p)
{
    p->fixheight();
    if (p->bfactor() == 2)
    {
        if (p->right->bfactor() < 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (p->bfactor() == -2)
    {
        if (p->left->bfactor() > 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p;
}


template <class key, class val>
TreeNode<key, val>* AVLTree<key, val>::add(TreeNode<key, val>* p, const key& k, const val& v)
{
    if (!p)
    {
        TreeNode<key, val>* temp;
        temp = new TreeNode<key, val>;
        temp->v = new val(v);
        temp->k = k;
        return temp;
    }

    if (comp->compare(k, p->k) == 0)
        throw AVLTreeException(std::string("Key already exist (in function add AVLTree)."), 1);
    else if (comp->compare(k, p->k)<0)
        p->left = add(p->left, k, v);
    else
        p->right = add(p->right, k, v);
    return balance(p);
}


template <class key, class val>
TreeNode<key, val>* AVLTree<key, val>::findmin(TreeNode<key, val>* p)
{
    return p->left ? findmin(p->left) : p;
}

template <class key, class val>
TreeNode<key, val>* AVLTree<key, val>::removemin(TreeNode<key, val>* p)
{
    if (p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}


template <class key, class val>
TreeNode<key, val>* AVLTree<key, val>::del(TreeNode<key, val>* p, const key& k)
{
    if (!p) return 0;
    if (comp->compare(k, p->k) < 0)
        p->left = del(p->left, k);
    else if (comp->compare(k, p->k) > 0)
        p->right = del(p->right, k);
    else
    {
        TreeNode<key, val>* q = p->left;
        TreeNode<key, val>* r = p->right;
        delete p->v;
        delete p;
        if (!r) return q;
        TreeNode<key, val>* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}
