#pragma once
#include "BaseTree.h"







class BinaryTreeException : public TreeException
{
protected:
    using TreeException::msg;
    using TreeException::code;
public:
    BinaryTreeException(const std::string& msg, const int code): TreeException(msg, code) {}
    BinaryTreeException(const BinaryTreeException& ex) : TreeException(ex) {}
};


template <class key, class val>
class BinaryTree : public BaseTree<key, val> {
private:
    TreeNode<key, val>* findmin(TreeNode<key, val>* p){return p->left ? findmin(p->left) : p;}
    TreeNode<key, val>* removemin(TreeNode<key, val>* p){if (p->left == 0) return p->right; p->left = removemin(p->left); return p; }

protected:


    virtual bool tr_pre(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_inf(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_post(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));

    virtual TreeNode<key, val>* add(const key& k, const val& v);
    virtual void edit(const key& k, const val& v);
    virtual TreeNode<key, val>* del(TreeNode<key, val>* p, const key& k);
    virtual val find(const key& k);

    virtual void trace_del(TreeNode<key, val>* node);
    virtual void trace_copy(TreeNode<key, val>* source, TreeNode<key, val>* destination);


public:
    TreeNode<key, val>* top = nullptr;
    Compare<key>* comp = nullptr;

    BinaryTree(Compare<key>* c) { comp = c; }
    BinaryTree(Compare<key>* c, const BinaryTree& tree) { comp = c; *this = tree; }
    ~BinaryTree() { trace_del(top); top = nullptr; }

    BinaryTree& operator=(const BinaryTree& tree) {trace_del(top);trace_copy(tree.top, top);  return *this;}
    BinaryTree& operator+=(const std::pair<key, val>& o) {add(o.first, o.second);return *this;}
    BinaryTree& operator*=(const std::pair<key, val>& o) {edit(o.first, o.second);return *this;}
    BinaryTree& operator-=(const key& k) {top = del(top, k);return *this;}
    val operator[](const key& k) {return this->find(k);}

    void trace_pre(val(*action)(const val&, const key&, const size_t));
    void trace_inf(val(*action)(const val&, const key&, const size_t));
    void trace_post(val(*action)(const val&, const key&, const size_t));

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }
    virtual TreeNode<key, val>* get_top() { return top; }
    void Show(TreeNode<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        std::cout << dir << " [" << node->k << '_' << *node->v << "]\n";
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { trace_del(top); delete this; }
};



template <class key, class val>
TreeNode<key, val>* BinaryTree<key, val>::add(const key& k, const val& v)
{
    TreeNode<key, val>* now = top;
    if (top == nullptr)
    {
        top = new TreeNode<key, val>;
        top->k = k;
        top->v = new val(v);
        return nullptr;
    }

    while (now != nullptr)
    {
        if (comp->compare(k, now->k) == 0)
            throw BinaryTreeException(std::string("Key already exist (in function add BinaryTree)."), 1);
        else if (comp->compare(k, now->k) < 0)
        {
            if (now->left == nullptr)
            {
                now->left = new TreeNode<key, val>;
                now->left->k = k;
                now->left->v = new val(v);
                return nullptr;
            }
            else
                now = now->left;
        }
        else if (comp->compare(k, now->k) > 0)
        {
            if (now->right == nullptr)
            {
                now->right = new TreeNode<key, val>;
                now->right->k = k;
                now->right->v = new val(v);
                return nullptr;
            }
            else
                now = now->right;
        }

    }


    return top;
}


template <class key, class val>
void BinaryTree<key, val>::edit(const key& k, const val& v)
{
    TreeNode<key, val>* now = top;

    while (now != nullptr)
    {
        if (comp->compare(k, now->k) == 0)
        {
            *now->v = v;
            return;
        }
        else if (comp->compare(k, now->k) < 0)
            now = now->left;
        else if (comp->compare(k, now->k) > 0)
            now = now->right;
    }

    throw BinaryTreeException(std::string("Key not found (in function edit BinaryTree)."), 0);
}


template <class key, class val>
TreeNode<key, val>* BinaryTree<key, val>::del(TreeNode<key, val>* p, const key& k)
{
    if (p == nullptr)
        throw BinaryTreeException(std::string("Key not found (in function del BinaryTree)."), 0);


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
        if (!r)
            return q;
        TreeNode<key, val>* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return min;
    }
    return p;
}


template <class key, class val>
val BinaryTree<key, val>::find(const key& k)
{
    TreeNode<key, val>* now = top;

    while (now != nullptr)
    {
        if (comp->compare(k, now->k) == 0)
            return *now->v;
        else if (comp->compare(k, now->k) < 0)
            now = now->left;
        else if (comp->compare(k, now->k) > 0)
            now = now->right;
    }

    throw BinaryTreeException(std::string("Key not found (in function find BinaryTree)."), 0);
}


template <class key, class val>
bool BinaryTree<key, val>::tr_pre(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    action(*node->v, node->k, depth);
    tr_pre(node->left, depth + 1, action);
    tr_pre(node->right, depth + 1, action);

    return true;
}


template <class key, class val>
bool BinaryTree<key, val>::tr_inf(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    tr_inf(node->left, depth + 1, action);
    action(*node->v, node->k, depth);
    tr_inf(node->right, depth + 1, action);

    return true;
}


template <class key, class val>
bool BinaryTree<key, val>::tr_post(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    tr_post(node->left, depth + 1, action);
    tr_post(node->right, depth + 1, action);
    action(*node->v, node->k, depth);

    return true;
}


template <class key, class val>
void BinaryTree<key, val>::trace_pre(val(*action)(const val&, const key&, const size_t))
{
    this->tr_pre(top, 0, action);

    return ;
}


template <class key, class val>
void BinaryTree<key, val>::trace_inf(val(*action)(const val&, const key&, const size_t))
{
    this->tr_inf(top, 0, action);

    return ;
}

template <class key, class val>
void BinaryTree<key, val>::trace_post(val(*action)(const val&, const key&, const size_t))
{

    this->tr_post(top, 0, action);

    return ;
}


template <class key, class val>
void BinaryTree<key, val>::trace_del(TreeNode<key, val>* node)
{
    if (node == nullptr)
        return;

    trace_del(node->left);
    trace_del(node->right);

    delete node->v;
    delete node;

    return;
}


template <class key, class val>
void BinaryTree<key, val>::trace_copy(TreeNode<key, val>* source, TreeNode<key, val>* destination)
{
    if (source == nullptr)
        return ;

    if (source->left != nullptr)
    {
        destination->left = new TreeNode<key, val>;
        destination->left->k = source->left->k;
        destination->left->v = new val(*source->left->v);
        destination->left->red = source->left->red;
        destination->left->height = source->left->height;
    }
    if (source->right != nullptr)
    {
        destination->right = new TreeNode<key, val>;
        destination->right->k = source->right->k;
        destination->right->v = new val(*source->right->v);
        destination->right->red = source->right->red;
        destination->right->height = source->right->height;
    }

    trace_copy(source->left, destination->left);
    trace_copy(source->right, destination->right);


    return ;
}

