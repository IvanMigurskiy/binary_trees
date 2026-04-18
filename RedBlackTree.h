#pragma once
#include "BaseTree.h"
#include "BinaryTree.h"





class RedBlackTreeException : public TreeException
{
protected:
    using TreeException::msg;
    using TreeException::code;
public:
    RedBlackTreeException(const std::string& msg, const int code) : TreeException(msg, code) {}
    RedBlackTreeException(const RedBlackTreeException& ex) : TreeException(ex) {}
};



template <class key, class val>
class RedBlackTree : public BinaryTree<key, val> {
private:
    TreeNode<key, val>* rotateright(TreeNode<key, val>* p) { TreeNode<key, val>* q = p->left; p->left = q->right; q->right = p; return q; }
    TreeNode<key, val>* rotateleft(TreeNode<key, val>* q) { TreeNode<key, val>* p = q->right; q->right = p->left; p->left = q; return p; }

    void BalanceInsert(TreeNode<key, val>** root);
    bool BalanceRemove1(TreeNode<key, val>** root);
    bool BalanceRemove2(TreeNode<key, val>** root);
    bool Insert(const key& k, const val& v, TreeNode<key, val>** root);
    bool Remove(TreeNode<key, val>** root, const key& k);
    bool GetMin(TreeNode<key, val>** root, TreeNode<key, val>** res);


protected:

    TreeNode<key, val>* add(const key& k, const val& v) { Insert(k, v, &top); return top; }
    TreeNode<key, val>* del(TreeNode<key, val>* p, const key& k) { Remove(&top, k); return top; }

public:
    using BinaryTree<key, val>::top;
    using BinaryTree<key, val>::comp;

    RedBlackTree(Compare<key>* c):BinaryTree<key, val>(c) { comp = c; }
    RedBlackTree(Compare<key>* c, const RedBlackTree& tree) { comp = c; *this = tree; }
    ~RedBlackTree() { BinaryTree<key, val>::trace_del(top); top = nullptr; }


    RedBlackTree& operator=(const BinaryTree<key, val>& tree) { trace_del(top); trace_copy(tree.top, top);  return *this; }
    RedBlackTree& operator*=(const std::pair<key, val>& o) { BinaryTree<key, val>::edit(o.first, o.second); return *this; }
    RedBlackTree& operator-=(const key& k) { del(top, k); return *this; }
    RedBlackTree& operator+=(const std::pair<key, val>& o) {add(o.first, o.second); if (top) top->red = false; return *this;}


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
        printf("%c[%d] (%s)\n", dir, node->k, node->red ? "red" : "black");
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { BinaryTree<key,val>::trace_del(top); delete this; }
};


template <class key, class val>
bool RedBlackTree<key, val>::BalanceRemove1(TreeNode<key, val>** root)
{
    TreeNode<key, val>* node = *root;
    TreeNode<key, val>* p1 = node->left;
    TreeNode<key, val>* p2 = node->right;
    if (p1 && p1->red) {
        p1->red = false;
        return false;
    }
    if (p2 && p2->red) { // случай 1
        node->red = true;
        p2->red = false;
        node = *root = rotateleft(node);
        if (BalanceRemove1(&node->left))
            node->left->red = false;
        return false;
    }
    unsigned int mask = 0;
    TreeNode<key, val>* p21 = p2->left;
    TreeNode<key, val>* p22 = p2->right;
    if (p21 && p21->red) mask |= 1;
    if (p22 && p22->red) mask |= 2;
    switch (mask)
    {
    case 0:		// случай 2 - if((!p21 || !p21->red) && (!p22 || !p22->red))
        p2->red = true;
        return true;
    case 1:
    case 3:		// случай 3 - if(p21 && p21->red)
        p2->red = true;
        p21->red = false;
        p2 = node->right = rotateright(p2);
        p22 = p2->right ;
    case 2:		// случай 4 - if(p22 && p22->red)
        p2->red = node->red;
        p22->red = node->red = false;
        *root = rotateleft(node);
    }
    return false;
}


template <class key, class val>
bool RedBlackTree<key, val>::BalanceRemove2(TreeNode<key, val>** root)
{
    TreeNode<key, val>* node = *root;
    TreeNode<key, val>* p1 = node->left;
    TreeNode<key, val>* p2 = node->right;
    if (p2 && p2->red)
    {
        p2->red = false;
        return false;
    }
    if (p1 && p1->red) { // случай 1
        node->red = true;
        p1->red = false;
        node = *root = rotateright(node);
        if (BalanceRemove2(&node->right))
            node->right->red = false;
        return false;
    }
    unsigned int mask = 0;
    TreeNode<key, val>* p11 = p1->left;
    TreeNode<key, val>* p12 = p1->right;
    if (p11 && p11->red) mask |= 1;
    if (p12 && p12->red) mask |= 2;
    switch (mask) {
    case 0:		// случай 2 - if((!p12 || !p12->red) && (!p11 || !p11->red))
        p1->red = true;
        return true;
    case 2:
    case 3:		// случай 3 - if(p12 && p12->red)
        p1->red = true;
        p12->red = false;
        p1 = node->left = rotateleft(p1);
        p11 = p1->left;
    case 1:		// случай 4 - if(p11 && p11->red)
        p1->red = node->red;
        p11->red = node->red = false;
        *root = rotateright(node);
    }
    return false;
}


template <class key, class val>
bool RedBlackTree<key, val>::Remove(TreeNode<key, val>** root, const key& k)
{
    TreeNode<key, val>* t, * node = *root;
    if (!node)
        throw RedBlackTreeException(std::string("Key not found (in function Remove RB_tree)."), 0);
    if (comp->compare(node->k, k)  < 0) {
        if (Remove(&node->right, k))
            return BalanceRemove2(root);
    }
    else if (comp->compare(node->k, k) > 0) {
        if (Remove(&node->left, k))
            return BalanceRemove1(root);
    }
    else {
        bool res;
        if (!node->right) {
            *root = node->left;
            res = !node->red;
        }
        else {
            res = GetMin(&node->right, root);
            t = *root;
            t->red = node->red;
            t->left = node->left;
            t->right = node->right;
            if (res) res = BalanceRemove2(root);
        }
        delete node->v;
        delete node;
        return res;
    }
    return 0;
}


template<class key, class val>
bool RedBlackTree<key, val>::GetMin(TreeNode<key, val>** root, TreeNode<key, val>** res)
{
    TreeNode<key, val>* node = *root;
    if (node->left) {
        if (GetMin(&node->left, res))
            return BalanceRemove1(root);
    }
    else {
        *root = node->right;
        *res = node;
        return !node->red;
    }
    return false;
}


template <class key, class val>
void RedBlackTree<key, val>::BalanceInsert(TreeNode<key, val>** root)
{
    TreeNode<key, val>* p1, * p2, * px1, * px2;
    TreeNode<key, val>* node = *root;
    if (node->red)
        return;
    p1 = node->left;
    p2 = node->right;
    if (p1 && p1->red) {
        px2 = p1->right;				// задача найти две рядом стоящие крастные вершины
        if (px2 && px2->red) p1 = node->left = rotateleft(p1);
        px1 = p1->left;
        if (px1 && px1->red) {
            node->red = true;
            p1->red = false;
            if (p2 && p2->red) {	// отделаемся перекраской вершин
                px1->red = true;
                p2->red = false;
                return;
            }
            *root = rotateright(node);
            return;
        }
    }
    // тоже самое в другую сторону
    if (p2 && p2->red) {
        px1 = p2->left;				// задача найти две рядом стоящие крастне вершины
        if (px1 && px1->red)
            p2 = node->right = rotateright(p2);
        px2 = p2->right;
        if (px2 && px2->red) {
            node->red = true;
            p2->red = false;
            if (p1 && p1->red) {	// отделаемся перекраской вершин
                px2->red = true;
                p1->red = false;
                return;
            }
            *root = rotateleft(node);
            return;
        }
    }
}


template <class key, class val>
bool RedBlackTree<key, val>::Insert(const key &k, const val& v, TreeNode<key, val>** root)
{
    TreeNode<key, val>* node = *root;
    if (!node)
    {
        *root = new TreeNode<key, val>;
        (*root)->k = k;
        (*root)->v = new val(v);
    }
    else {
        if (comp->compare(k, node->k)==0 )
            throw RedBlackTreeException(std::string("Key not found (in function Insert RB_tree)."), 0);
        if (Insert(k, v, comp->compare(k, node->k) < 0 ? &node->left : &node->right))
            return true;
        BalanceInsert(root);
    }
    return false;
}

