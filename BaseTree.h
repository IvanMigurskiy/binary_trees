#pragma once
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>



template <class key, class val>
val print(const val& v, const key& k, const size_t depth)
{
    std::cout << "depth = " << depth << " ; key = " << k << " ; value = " << v << std::endl;
    return v;
}


template <typename T>
class Compare {
public:
    virtual ~Compare() {};
    virtual int compare(const T& l, const T& r)const = 0;
};

class IntCompare : public Compare<int>
{
public:
    IntCompare() {};
    virtual ~IntCompare() {};
    int compare(const int& l, const int& r) const override
    { return l > r ? 1 : (l < r ? -1 : 0); }
};

class StrCompare : public Compare<std::string>
{
public:
    StrCompare() {};
    virtual ~StrCompare() {};
    int compare(const std::string& l, const std::string& r) const override
    { return l > r ? 1 : (l < r ? -1 : 0); }
};





template <class key, class val>
class TreeNode {
public:
    key k = key();
    val* v = nullptr;
    TreeNode* left = nullptr;
    TreeNode* right = nullptr;

    // AVL
    size_t height = 0;

    int bfactor(){return height_(right) - height_(left);}
    void fixheight(){unsigned char hl = height_(left);unsigned char hr = height_(right); height = (hl > hr ? hl : hr) + 1;}
    unsigned char height_(TreeNode<key, val>* p) {return p ? p->height : 0;}

    // RB
    bool red = true;

    // 2-3

    int size;      // количество занятых ключей
    key keys[3];
    val* vals[3];
    TreeNode* first = nullptr;   // *first <= key[0];
    TreeNode* second = nullptr;  // key[0] <= *second < key[1];
    TreeNode* third = nullptr;   // key[1] <= *third < key[2];
    TreeNode* fourth = nullptr;  // kye[2] <= *fourth.
    TreeNode* parent = nullptr; //Указатель на родителя нужен для того, потому что адрес корня может меняться при удалении

    TreeNode(){}

    TreeNode(const key& k, const val& v) : size(1) { keys[0] = k; keys[1] = keys[2] = { 0 }; vals[0] = new val(v); vals[1] = vals[2] = nullptr; }

    TreeNode(const key& k, const val& v, TreeNode* first_, TreeNode* second_, TreeNode* third_, TreeNode* fourth_, TreeNode * parent_) : size(1) {
        keys[0] = k; keys[1] = keys[2] = { 0 };
        vals[0] = new val(v); vals[1] = vals[2] = nullptr;
        first = first_; second = second_; third = third_; fourth = fourth_; parent = parent_;
    }

    void eq(val* p1, val* p2)
    {
        if (p2)
        {
            if (p1)
                *p1 = *p2;
            else
                p1 = new val(*p2);
        }
        else
            p1 = nullptr;
    }

    bool find(const key& k) { // Этот метод возвращает true, если ключ k находится в вершине, иначе false.
        for (int i = 0; i < size; ++i)
            if (keys[i] == k) return true;
        return false;
    }

    void swap(key& x, val *x_, key& y, val* y_) {
        key r = x;
        x = y;
        y = r;
        val r_ = *x_;
        *x_ = *y_;
        *y_ = r_;
    }

    void sort2(key& x, val* x_, key& y, val* y_) {
        if (x> y) swap(x, x_, y, y_);
    }

    void sort3(key& x, val* x_, key& y, val* y_, key& z, val* z_) {
        if ( x > y) swap(x, x_, y, y_);
        if ( x > z) swap(x, x_, z, z_);
        if ( y > z) swap(y, y_, z, z_);
    }

    void sort() { // Ключи в вершинах должны быть отсортированы
        if (size == 1) return;
        if (size == 2) sort2(keys[0], vals[0], keys[1], vals[1]);
        if (size == 3) sort3(keys[0], vals[0], keys[1], vals[1], keys[2], vals[2]);
    }

    void insert_to_node(const key& k, const val& v) {  // Вставляем ключ k в вершину (не в дерево)
        keys[size] = k;
        vals[size] = new val(v);
        size++;
        sort();
    }

    void remove_from_node(const key& k) { // Удаляем ключ k из вершины (не из дерева)
        if (size >= 1 && keys[0] == k ) {
            keys[0] = keys[1];
            keys[1] = keys[2];
            eq(vals[0], vals[1]);
            eq(vals[1], vals[2]);
            delete vals[2];
            vals[2] = nullptr;
            size--;
        }
        else if (size == 2 && keys[1] == k) {
            keys[1] = keys[2];
            eq(vals[1], vals[2]);
            delete vals[2];
            vals[2] = nullptr;
            size--;
        }
    }

    void become_node2(const key& k, const val &v, TreeNode* first_, TreeNode* second_) {  // Преобразовать в 2-вершину.
        keys[0] = k;
        vals[0] = new val(v);
        first = first_;
        second = second_;
        third = nullptr;
        fourth = nullptr;
        parent = nullptr;
        size = 1;
    }

    bool is_leaf() { // Является ли узел листом; проверка используется при вставке и удалении.
        return (first == nullptr) && (second == nullptr) && (third == nullptr);
    }

};



class TreeException : public std::exception
{
protected:
    std::string msg;
    int code;

public:
    TreeException(const std::string& msg, const int code) { this->msg = msg; this->code = code; }
    TreeException(const TreeException& ex) { this->msg = ex.msg; this->code = ex.code; }

    virtual const char* what() { return msg.c_str(); }

    virtual int cod() { return code; }
};





template <class key, class val>
class BaseTree {
protected:


    virtual bool tr_pre(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t)) = 0;
    virtual bool tr_inf(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t)) = 0;
    virtual bool tr_post(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t)) = 0;

    virtual TreeNode<key, val>* add(const key& k, const val& v) = 0;
    virtual void edit(const key& k, const val& v) = 0;
    virtual TreeNode<key, val>* del(TreeNode<key, val>* p, const key& k) = 0;
    virtual val find(const key& k) = 0;


public:
    BaseTree() {};
    ~BaseTree() {};

    virtual BaseTree& operator+=(const std::pair<key, val>& o) = 0;
    virtual BaseTree& operator*=(const std::pair<key, val>& o) = 0;
    virtual BaseTree& operator-=(const key& k) = 0;
    virtual val operator[](const key& k) = 0;

    virtual void trace_pre(val(*action)(const val&, const key&, const size_t)) = 0;
    virtual void trace_inf(val(*action)(const val&, const key&, const size_t)) = 0;
    virtual void trace_post(val(*action)(const val&, const key&, const size_t)) = 0;

    virtual TreeNode<key, val>* get_top() { return nullptr; }
    void trace(bool type, BaseTree<key, val>& tree, TreeNode<key, val>* node)
    {
        if (!node)
            return;

        if (type)
        {
            if (node->first)
            {
                trace(type, tree, node->first);
                trace(type, tree, node->second);
                if (node->size == 2)
                    trace(type, tree, node->third);
                return;
            }
            if (node->size == 2)
            {
                tree += std::make_pair(node->keys[0], *(node->vals[0]));
                tree += std::make_pair(node->keys[1], *(node->vals[1]));
            }
            else
                tree += std::make_pair(node->keys[0], *(node->vals[0]));
        }
        else
        {
            tree += std::make_pair(node->k, *(node->v));
            trace(type, tree, node->left);
            trace(type, tree, node->right);
        }
    }
    virtual void delete_tree() { delete this; }

};

