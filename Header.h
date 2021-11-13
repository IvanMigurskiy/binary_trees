#pragma once
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>



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
    virtual int Compare<int>::compare(const int& l, const int& r) const { return l > r ? 1 : (l < r ? -1 : 0); }
};

class StrCompare : public Compare<std::string>
{
public:
    StrCompare() {};
    virtual ~StrCompare() {};
    virtual int Compare<std::string>::compare(const std::string& l, const std::string& r) const { return l > r ? 1 : (l < r ? -1 : 0); }
};





template <class key, class val>
class one {
public:
    key k = key();
    val* v = nullptr;
    one* left = nullptr;
    one* right = nullptr;

    // AVL
    size_t height = 0;

    int bfactor(){return height_(right) - height_(left);}
    void fixheight(){unsigned char hl = height_(left);unsigned char hr = height_(right); height = (hl > hr ? hl : hr) + 1;}
    unsigned char height_(one<key, val>* p) {return p ? p->height : 0;}

    // RB
    bool red = true;

    // 2-3

    int size;      // количество занятых ключей
    key keys[3];
    val* vals[3];
    one* first = nullptr;   // *first <= key[0];
    one* second = nullptr;  // key[0] <= *second < key[1];
    one* third = nullptr;   // key[1] <= *third < key[2];
    one* fourth = nullptr;  // kye[2] <= *fourth.
    one* parent = nullptr; //Указатель на родителя нужен для того, потому что адрес корня может меняться при удалении

    one(){}

    one(const key& k, const val& v) : size(1) { keys[0] = k; keys[1] = keys[2] = { 0 }; vals[0] = new val(v); vals[1] = vals[2] = nullptr; }

    one(const key& k, const val& v, one* first_, one* second_, one* third_, one* fourth_, one * parent_) : size(1) {
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

    void become_node2(const key& k, const val &v, one* first_, one* second_) {  // Преобразовать в 2-вершину.
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



class tree_exception : public std::exception
{
protected:
    std::string msg;
    int code;

public:
    tree_exception(const std::string& msg, const int code) { this->msg = msg; this->code = code; }
    tree_exception(const tree_exception& ex) { this->msg = ex.msg; this->code = ex.code; }

    tree_exception(tree_exception&& ex) { this->msg = ex.msg; this->code = ex.code; }

    const char* what() { return msg.c_str(); }

    int cod() { return code; }
};

class bt_exep : public tree_exception
{
protected:
    tree_exception::msg;
    tree_exception::code;
public:
    bt_exep(const std::string& msg, const int code): tree_exception(msg, code) {}
    bt_exep(const bt_exep& ex) : tree_exception(ex) {}
    bt_exep(bt_exep&& ex) : tree_exception(ex) {}
};

class AVL_exep : public tree_exception
{
protected:
    tree_exception::msg;
    tree_exception::code;
public:
    AVL_exep(const std::string& msg, const int code) : tree_exception(msg, code) {}
    AVL_exep(const AVL_exep& ex) : tree_exception(ex) {}
    AVL_exep(AVL_exep&& ex) : tree_exception(ex) {}
};

class splay_exep : public tree_exception
{
protected:
    tree_exception::msg;
    tree_exception::code;
public:
    splay_exep(const std::string& msg, const int code) : tree_exception(msg, code) {}
    splay_exep(const splay_exep& ex) : tree_exception(ex) {}
    splay_exep(splay_exep&& ex) : tree_exception(ex) {}
};

class RB_exep : public tree_exception
{
protected:
    tree_exception::msg;
    tree_exception::code;
public:
    RB_exep(const std::string& msg, const int code) : tree_exception(msg, code) {}
    RB_exep(const RB_exep& ex) : tree_exception(ex) {}
    RB_exep(RB_exep&& ex) : tree_exception(ex) {}
};

class t23_exep : public tree_exception
{
protected:
    tree_exception::msg;
    tree_exception::code;
public:
    t23_exep(const std::string& msg, const int code) : tree_exception(msg, code) {}
    t23_exep(const t23_exep& ex) : tree_exception(ex) {}
    t23_exep(t23_exep&& ex) : tree_exception(ex) {}
};






template <class key, class val>
class base_tree {
protected:
    

    virtual bool tr_pre(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t)) = 0;
    virtual bool tr_inf(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t)) = 0;
    virtual bool tr_post(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t)) = 0;

    virtual one<key, val>* add(const key& k, const val& v) = 0;
    virtual void edit(const key& k, const val& v) = 0;
    virtual one<key, val>* del(one<key, val>* p, const key& k) = 0;
    virtual val& find(const key& k) = 0;


public:
    base_tree() {};
    ~base_tree() {};

    virtual base_tree& operator+=(const std::pair<key, val>& o) = 0;
    virtual base_tree& operator*=(const std::pair<key, val>& o) = 0;
    virtual base_tree& operator-=(const key& k) = 0;
    virtual val& operator[](const key& k) = 0;

    virtual void trace_pre(val(*action)(const val&, const key&, const size_t)) = 0;
    virtual void trace_inf(val(*action)(const val&, const key&, const size_t)) = 0;
    virtual void trace_post(val(*action)(const val&, const key&, const size_t)) = 0;

    virtual one<key, val>* get_top() { return nullptr; }
    void trace(bool type, base_tree<key, val>& tree, one<key, val>* node)
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



//////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class key, class val>
class bin_tree : public base_tree<key, val> {
private:
    one<key, val>* findmin(one<key, val>* p){return p->left ? findmin(p->left) : p;}
    one<key, val>* removemin(one<key, val>* p){if (p->left == 0) return p->right; p->left = removemin(p->left); return p; }

protected:


    virtual bool tr_pre(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_inf(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_post(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));

    virtual one<key, val>* add(const key& k, const val& v);
    virtual void edit(const key& k, const val& v);
    virtual one<key, val>* del(one<key, val>* p, const key& k);
    virtual val& find(const key& k);

    virtual void trace_del(one<key, val>* node);
    virtual void trace_copy(one<key, val>* source, one<key, val>* destination);


public:
    one<key, val>* top = nullptr;
    Compare<key>* comp = nullptr;

    bin_tree(Compare<key>* c) { comp = c; }
    bin_tree(Compare<key>* c, const bin_tree& tree) { comp = c; *this = tree; }
    ~bin_tree() { trace_del(top); top = nullptr; }

    bin_tree& operator=(const bin_tree& tree) {trace_del(top);trace_copy(tree.top, top);  return *this;}
    bin_tree& operator+=(const std::pair<key, val>& o) {add(o.first, o.second);return *this;}
    bin_tree& operator*=(const std::pair<key, val>& o) {edit(o.first, o.second);return *this;}
    bin_tree& operator-=(const key& k) {top = del(top, k);return *this;}
    val& operator[](const key& k) {return this->find(k);}

    void trace_pre(val(*action)(const val&, const key&, const size_t));
    void trace_inf(val(*action)(const val&, const key&, const size_t));
    void trace_post(val(*action)(const val&, const key&, const size_t));

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }
    virtual one<key, val>* get_top() { return top; }
    void Show(one<key, val>* node, int depth, char dir)
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
one<key, val>* bin_tree<key, val>::add(const key& k, const val& v)
{
    one<key, val>* now = top;
    if (top == nullptr)
    {
        top = new one<key, val>;
        top->k = k;
        top->v = new val(v);
        return nullptr;
    }

    while (now != nullptr)
    {
        if (comp->compare(k, now->k) == 0)
            throw bt_exep(std::string("Key already exist."), 1);
        else if (comp->compare(k, now->k) == -1)
        {
            if (now->left == nullptr)
            {
                now->left = new one<key, val>;
                now->left->k = k;
                now->left->v = new val(v);
                return nullptr;
            }
            else
                now = now->left;
        }
        else if (comp->compare(k, now->k) == 1)
        {
            if (now->right == nullptr)
            {
                now->right = new one<key, val>;
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
void bin_tree<key, val>::edit(const key& k, const val& v)
{
    one<key, val>* now = top;

    while (now != nullptr)
    {
        if (comp->compare(k, now->k) == 0)
        {
            *now->v = v;
            return;
        }
        else if (comp->compare(k, now->k) == -1)
            now = now->left;
        else if (comp->compare(k, now->k) == 1)
            now = now->right;
    }

    throw bt_exep(std::string("Key not found."), 0);
}


template <class key, class val>
one<key, val>* bin_tree<key, val>::del(one<key, val>* p, const key& k)
{
    if (p == nullptr)
        throw bt_exep(std::string("Key not found."), 0);


    if (comp->compare(k, p->k) == -1)
        p->left = del(p->left, k);
    else if (comp->compare(k, p->k) == 1)
        p->right = del(p->right, k);
    else
    {
        one<key, val>* q = p->left;
        one<key, val>* r = p->right;
        delete p->v;
        delete p;
        if (!r)
            return q;
        one<key, val>* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return min;
    }
    return p;
}


template <class key, class val>
val& bin_tree<key, val>::find(const key& k)
{
    one<key, val>* now = top;

    while (now != nullptr)
    {
        if (comp->compare(k, now->k) == 0)
            return *now->v;
        else if (comp->compare(k, now->k) == -1)
            now = now->left;
        else if (comp->compare(k, now->k) == 1)
            now = now->right;
    }

    throw bt_exep(std::string("Key not found."), 0);
}


template <class key, class val>
bool bin_tree<key, val>::tr_pre(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    action(*node->v, node->k, depth);
    tr_pre(node->left, depth + 1, action);
    tr_pre(node->right, depth + 1, action);

    return true;
}


template <class key, class val>
bool bin_tree<key, val>::tr_inf(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    tr_inf(node->left, depth + 1, action);
    action(*node->v, node->k, depth);
    tr_inf(node->right, depth + 1, action);

    return true;
}


template <class key, class val>
bool bin_tree<key, val>::tr_post(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    tr_post(node->left, depth + 1, action);
    tr_post(node->right, depth + 1, action);
    action(*node->v, node->k, depth);

    return true;
}


template <class key, class val>
void bin_tree<key, val>::trace_pre(val(*action)(const val&, const key&, const size_t))
{
    this->tr_pre(top, 0, action);

    return ;
}


template <class key, class val>
void bin_tree<key, val>::trace_inf(val(*action)(const val&, const key&, const size_t))
{
    this->tr_inf(top, 0, action);

    return ;
}

template <class key, class val>
void bin_tree<key, val>::trace_post(val(*action)(const val&, const key&, const size_t))
{

    this->tr_post(top, 0, action);

    return ;
}


template <class key, class val>
void bin_tree<key, val>::trace_del(one<key, val>* node)
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
void bin_tree<key, val>::trace_copy(one<key, val>* source, one<key, val>* destination)
{
    if (source == nullptr)
        return ;

    if (source->left != nullptr)
    {
        destination->left = new one<key, val>;
        destination->left->k = source->left->k;
        destination->left->v = new val(*source->left->v);
        destination->left->red = source->left->red;
        destination->left->height = source->left->height;
    }
    if (source->right != nullptr)
    {
        destination->right = new one<key, val>;
        destination->right->k = source->right->k;
        destination->right->v = new val(*source->right->v);
        destination->right->red = source->right->red;
        destination->right->height = source->right->height;
    }

    trace_copy(source->left, destination->left);
    trace_copy(source->right, destination->right);


    return ;
}


template <class key, class val>
val print(const val& v, const key& k, const size_t depth)
{
    std::cout << "depth = " << depth << " ; key = " << k << " ; value = " << v << std::endl;
    return v;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class key, class val>
class AVL_tree : public bin_tree<key, val> {
private:

    one<key, val>* rotateright(one<key, val>* p) { one<key, val>* q = p->left; p->left = q->right; q->right = p; return q; }
    one<key, val>* rotateleft(one<key, val>* q) { one<key, val>* p = q->right; q->right = p->left; p->left = q; return p; }
    virtual one<key, val>* add(one<key, val>* p, const key& k, const val& v);
    virtual one<key, val>* del(one<key, val>* p, const key& k);


    one<key, val>* balance(one<key, val>* p);
    one<key, val>* findmin(one<key, val>* p);
    one<key, val>* removemin(one<key, val>* p);



protected:

    //void add(const key& k, const val& v);
    //void edit(const key& k, const val& v);
    //virtual void bin_tree<key, val>::trace_del(one<key, val> *node);
    //bool trace_copy(one_h<key, val> *source, one_h<key, val> *destination);


public:
    bin_tree<key, val>::top;
    bin_tree<key, val>::comp;

    AVL_tree(Compare<key>* c) :bin_tree<key, val>(c){ comp = c; }
    AVL_tree(Compare<key>* c, const AVL_tree& tree) { comp = c; *this = tree; }
    ~AVL_tree() { bin_tree<key, val>::trace_del(top); top = nullptr; }


    AVL_tree& operator=(const bin_tree<key, val>& tree) { trace_del(top); trace_copy(tree.top, top);  return *this; }
    AVL_tree& operator*=(const std::pair<key, val>& o) { bin_tree<key,val>::edit(o.first, o.second); return *this; }
    AVL_tree& operator+=(const std::pair<key, val>& o) { top = add(top, o.first, o.second); return *this; }
    AVL_tree& operator-=(const key& k) { top = del(top, k); return *this; }

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }

    void Show(one<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        std::cout << dir << " [" << node->k << '_' << *node->v << "]\n";
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { bin_tree<key, val>::trace_del(top); delete this; }
};


template <class key, class val>
one<key, val>* AVL_tree<key, val>::balance(one<key, val>* p)
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
one<key, val>* AVL_tree<key, val>::add(one<key, val>* p, const key& k, const val& v)
{
    if (!p) 
    {
        one<key, val>* temp;
        temp = new one<key, val>;
        temp->v = new val(v);
        temp->k = k;
        return temp;
    }

    if (comp->compare(k, p->k) == 0)
        throw AVL_exep(std::string("Key already exist."), 1);
    else if (comp->compare(k, p->k)==-1)
        p->left = add(p->left, k, v);
    else
        p->right = add(p->right, k, v);
    return balance(p);
}


template <class key, class val>
one<key, val>* AVL_tree<key, val>::findmin(one<key, val>* p)
{
    return p->left ? findmin(p->left) : p;
}

template <class key, class val>
one<key, val>* AVL_tree<key, val>::removemin(one<key, val>* p)
{
    if (p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}


template <class key, class val>
one<key, val>* AVL_tree<key, val>::del(one<key, val>* p, const key& k)
{
    if (!p) return 0;
    if (comp->compare(k, p->k) == -1)
        p->left = del(p->left, k);
    else if (comp->compare(k, p->k) == 1)
        p->right = del(p->right, k);
    else
    {
        one<key, val>* q = p->left;
        one<key, val>* r = p->right;
        delete p->v;
        delete p;
        if (!r) return q;
        one<key, val>* min = findmin(r);
        min->right = removemin(r);
        min->left = q;
        return balance(min);
    }
    return balance(p);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class key, class val>
class splay_tree : public bin_tree<key, val> {
private:
    one<key, val>* rotateright(one<key, val>* p) { one<key, val>* q = p->left; p->left = q->right; q->right = p; p->fixheight(); q->fixheight(); return q; }
    one<key, val>* rotateleft(one<key, val>* q) { one<key, val>* p = q->right; q->right = p->left; p->left = q; q->fixheight(); p->fixheight(); return p; }

    one<key, val>* splay(one<key, val>* root, const key& k);


protected:

    virtual val& find(const key& k);

public:
    bin_tree<key, val>::top;
    bin_tree<key, val>::comp;

    splay_tree(Compare<key>* c) :bin_tree<key, val>(c){ comp = c; }
    splay_tree(Compare<key>* c, const splay_tree& tree) { comp = c; *this = tree; }
    ~splay_tree() { bin_tree<key, val>::trace_del(top); top = nullptr; }

    splay_tree& operator=(const bin_tree<key, val>& tree) { trace_del(top); bin_tree<key, val>::trace_copy(tree.top, top);  return *this; }
    splay_tree& operator+=(const std::pair<key, val>& o) { bin_tree<key, val>::add(o.first, o.second); return *this; }
    splay_tree& operator*=(const std::pair<key, val>& o) { find(o.first); *top->v = o.second; return *this; }
    splay_tree& operator-=(const key& k) { top = bin_tree<key, val>::del(top, k); return *this; }

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }

    one<key, val>* get_top() { return top; }

    void Show(one<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        std::cout << dir << " [" << node->k << '_' << *node->v << "]\n";
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { bin_tree<key, val>::trace_del(top); top = nullptr; delete this; }
};


template <class key, class val>
val& splay_tree<key, val>::find(const key& k)
{
    top = splay(top, k);;
    if(top->k != k)
        throw splay_exep(std::string("Key not found."), 0);
    return *top->v;
}


template <class key, class val>
one<key, val>* splay_tree<key, val>::splay(one<key, val>* root, const key& k)
{
    if (root == nullptr || comp->compare(root->k, k)==0 )
        return root;

    if (comp->compare(root->k, k) == 1 )
    {
        if (root->left == nullptr) return root;

        if (comp->compare(root->left->k , k)==1 )
        {
            root->left->left = splay(root->left->left, k);
            root = rotateright(root);
        }
        else if (comp->compare(root->left->k, k)==-1 )
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

        if (comp->compare(root->right->k, k)==1)
        {
            root->right->left = splay(root->right->left, k);

            if (root->right->left != nullptr)
                root->right = rotateright(root->right);
        }
        else if (comp->compare(root->right->k, k)==-1 )
        {
            root->right->right = splay(root->right->right, k);
            root = rotateleft(root);
        }

        return (root->right == nullptr) ? root : rotateleft(root);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class key, class val>
class RBtree : public bin_tree<key, val> {
private:
    one<key, val>* rotateright(one<key, val>* p) { one<key, val>* q = p->left; p->left = q->right; q->right = p; return q; }
    one<key, val>* rotateleft(one<key, val>* q) { one<key, val>* p = q->right; q->right = p->left; p->left = q; return p; }
    //virtual one<key, val>* add(one<key, val>* p, const key& k, const val& v);
    //virtual one<key, val>* del(one<key, val>* p, const key& k);


    void BalanceInsert(one<key, val>** root);
    bool BalanceRemove1(one<key, val>** root);
    bool BalanceRemove2(one<key, val>** root);
    bool Insert(const key& k, const val& v, one<key, val>** root);
    bool Remove(one<key, val>** root, const key& k);
    bool GetMin(one<key, val>** root, one<key, val>** res);


protected:

    one<key, val>* add(const key& k, const val& v) { Insert(k, v, &top); return top; }
    one<key, val>* del(one<key, val>* p, const key& k) { Remove(&top, k); return top; }

public:
    bin_tree<key, val>::top;
    bin_tree<key, val>::comp;

    RBtree(Compare<key>* c):bin_tree<key, val>(c) { comp = c; }
    RBtree(Compare<key>* c, const RBtree& tree) { comp = c; *this = tree; }
    ~RBtree() { bin_tree<key, val>::trace_del(top); top = nullptr; }


    RBtree& operator=(const bin_tree<key, val>& tree) { trace_del(top); trace_copy(tree.top, top);  return *this; }
    RBtree& operator*=(const std::pair<key, val>& o) { bin_tree<key, val>::edit(o.first, o.second); return *this; }
    RBtree& operator-=(const key& k) { del(top, k); return *this; }
    RBtree& operator+=(const std::pair<key, val>& o) {add(o.first, o.second); if (top) top->red = false; return *this;}


    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }

    void Show(one<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        printf("%c[%d] (%s)\n", dir, node->k, node->red ? "red" : "black");
        Show(node->left, depth + 2, '-');
        Show(node->right, depth + 2, '+');
    }
    virtual void delete_tree() { bin_tree<key,val>::trace_del(top); delete this; }
};


template <class key, class val>
bool RBtree<key, val>::BalanceRemove1(one<key, val>** root)
{
    one<key, val>* node = *root;
    one<key, val>* p1 = node->left;
    one<key, val>* p2 = node->right;
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
    one<key, val>* p21 = p2->left;
    one<key, val>* p22 = p2->right;
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
bool RBtree<key, val>::BalanceRemove2(one<key, val>** root)
{
    one<key, val>* node = *root;
    one<key, val>* p1 = node->left;
    one<key, val>* p2 = node->right;
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
    one<key, val>* p11 = p1->left;
    one<key, val>* p12 = p1->right;
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
bool RBtree<key, val>::Remove(one<key, val>** root, const key& k)
{
    one<key, val>* t, * node = *root;
    if (!node)
        throw RB_exep(std::string("Key not found."), 0);
    if (comp->compare(node->k, k) == -1) {
        if (Remove(&node->right, k))
            return BalanceRemove2(root);
    }
    else if (comp->compare(node->k, k) == 1) {
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
bool RBtree<key, val>::GetMin(one<key, val>** root, one<key, val>** res)
{
    one<key, val>* node = *root;
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
void RBtree<key, val>::BalanceInsert(one<key, val>** root)
{
    one<key, val>* p1, * p2, * px1, * px2;
    one<key, val>* node = *root;
    if (node->red) 
        return;
    p1 = node->left;
    p2 = node->right;
    if (p1 && p1->red) {
        px2 = p1->right;				// задача найти две рядом стоящие крастне вершины
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
bool RBtree<key, val>::Insert(const key &k, const val& v, one<key, val>** root)
{
    one<key, val>* node = *root;
    if (!node)
    {
        *root = new one<key, val>;
        (*root)->k = k;
        (*root)->v = new val(v);
    }
    else {
        if (comp->compare(k, node->k)==0 )
            throw RB_exep(std::string("Key not found."), 0);
        if (Insert(k, v, comp->compare(k, node->k) == -1 ? &node->left : &node->right)) 
            return true;
        BalanceInsert(root);
    }
    return false;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class key, class val>
class tree_23: public base_tree<key, val> {
private:

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



    one<key, val>* split(one<key, val>* item); // Метод для разделение вершины при переполнении;
    one<key, val>* insert(one<key, val>* p, const key& k, const val& v); // Вставка в дерево;
    one<key, val>* search(one<key, val>* p, const key& k); // Поиск в дереве;
    one<key, val>* search_min(one<key, val>* p); // Поиск минимального элемента в поддереве; 
    one<key, val>* merge(one<key, val>* leaf); // Слияние используется при удалении;
    one<key, val>* redistribute(one<key, val>* leaf); // Перераспределение также используется при удалении;
    one<key, val>* fix(one<key, val>* leaf); // Используется после удаления для возвращения свойств дереву (использует merge или redistribute) 
    one<key, val>* remove(one<key, val>* p, const key& k); // Собственно, из названия понятно;


protected:
    virtual bool tr_pre(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_inf(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_post(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));

    virtual void trace_del(one<key, val>* node);
    virtual void trace_copy(one<key, val>* source, one<key, val>* destination);

    virtual one<key, val>* add(const key& k, const val& v) { return nullptr; }
    virtual void edit(const key& k, const val& v);
    virtual one<key, val>* del(one<key, val>* p, const key& k) { return nullptr; }
    virtual val& find(const key& k) { val a; return a; }

public:
    Compare<key>* comp = nullptr;
    one<key, val>* top = nullptr;

    tree_23(Compare<key>* c) { comp = c; }
    tree_23(Compare<key>* c, const tree_23& tree) { comp = c; *this = tree; }
    ~tree_23() { trace_del(top); top = nullptr; }


    virtual tree_23& operator+=(const std::pair<key, val>& o) { top = insert(top, o.first, o.second); return *this; }
    virtual tree_23& operator*=(const std::pair<key, val>& o) { one<key, val>* p = search(top, o.first); if (!p) throw t23_exep(std::string("Key not found."), 0); edit(o.first, o.second); return *this; }
    virtual tree_23& operator-=(const key& k) { top = remove(top, k); return *this; }
    virtual val& operator[](const key& k) { one<key, val>* p = search(top, k); if (!p) throw t23_exep(std::string("Key not found."), 0); return *p->vals[p->keys[0] >= k ? 0 : 1]; }

    void Show()
    {
        printf("[tree]\n");
        Show(top, 0, '*');
    }
    virtual one<key, val>* get_top() { return top; }
    void Show(one<key, val>* node, int depth, char dir)
    {
        int n;
        if (!node) return;
        for (n = 0; n < depth; n++) putchar(' ');
        if (node->size == 1)
            std::cout << dir << " [" << node->keys[0] << '_' << *node->vals[0] << "]\n";
        else if (node->size == 2)
            std::cout << dir << " [" << node->keys[0] << '_' << *node->vals[0] << " ; " << node->keys[1] << '_' << *node->vals[1] << "]\n";
        Show(node->first, depth + 2, '-');
        Show(node->second, depth + 2, '/');
        Show(node->third, depth + 2, '+');
    }
    virtual void delete_tree() { trace_del(top); delete this; }

    void trace_pre(val(*action)(const val&, const key&, const size_t));
    void trace_inf(val(*action)(const val&, const key&, const size_t));
    void trace_post(val(*action)(const val&, const key&, const size_t));
};


template <class key, class val>
bool tree_23<key, val>::tr_pre(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    action(*node->v, node->k, depth);
    if (node->size == 1)
    {
        tr_pre(node->first, depth + 1, action);
        tr_pre(node->second, depth + 1, action);
    }
    else if (node->size == 2)
    {
        tr_pre(node->first, depth + 1, action);
        tr_pre(node->second, depth + 1, action);
        tr_pre(node->third, depth + 1, action);
    }

    return true;
}


template <class key, class val>
 bool tree_23<key, val>::tr_inf(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    if (node->size == 1)
    {
        tr_pre(node->first, depth + 1, action);
        action(*node->v, node->k, depth);
        tr_pre(node->second, depth + 1, action);
    }
    else if (node->size == 2)
    {
        tr_pre(node->first, depth + 1, action);
        action(*node->v, node->k, depth);
        tr_pre(node->second, depth + 1, action);
        tr_pre(node->third, depth + 1, action);
    }

    return true;
}


template <class key, class val>
bool tree_23<key, val>::tr_post(one<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
{
    if (node == nullptr)
        return false;

    if (node->size == 1)
    {
        tr_pre(node->first, depth + 1, action);
        tr_pre(node->second, depth + 1, action);
        action(*node->v, node->k, depth);
    }
    else if (node->size == 2)
    {
        tr_pre(node->first, depth + 1, action);
        tr_pre(node->second, depth + 1, action);
        tr_pre(node->third, depth + 1, action);
        action(*node->v, node->k, depth);
    }

    return true;
}


template <class key, class val>
void tree_23<key, val>::trace_pre(val(*action)(const val&, const key&, const size_t))
{
    this->tr_pre(top, 0, action);
    return ;
}



template <class key, class val>
void tree_23<key, val>::trace_inf(val(*action)(const val&, const key&, const size_t))
{
    this->tr_inf(top, 0, action);

    return ;
}

template <class key, class val>
void tree_23<key, val>::trace_post(val(*action)(const val&, const key&, const size_t))
{

    this->tr_post(top, 0, action);

    return ;
}


template <class key, class val>
void tree_23<key, val>::trace_del(one<key, val>* node)
{
    if (node == nullptr)
        return;

    trace_del(node->left);
    trace_del(node->right);

    for(int i=0; i<node->size;i++)
        delete node->vals[i];
    delete node;

    return;
}


template <class key, class val>
void tree_23<key, val>::trace_copy(one<key, val>* source, one<key, val>* destination)
{
    if (source == nullptr)
        return;

    if (source->left != nullptr)
    {
        destination->left = new one<key, val>;
        for (int i = 0; i < source->left->size; i++)
        {
            destination->left->keys[i] = source->left->keys[i];
            destination->left->vals[i] = new val(*source->left->vals[i]);
        }
    }
    if (source->right != nullptr)
    {
        destination->right = new one<key, val>;
        for (int i = 0; i < source->right->size; i++)
        {
            destination->right->keys[i] = source->right->keys[i];
            destination->right->vals[i] = new val(*source->right->vals[i]);
        }
    }

    trace_copy(source->left, destination->left);
    trace_copy(source->right, destination->right);


    return;
}


template<class key, class val>
void tree_23<key, val>::edit(const key& k, const val& v) {
    one<key, val>* p = search(top, k);
    while (p)
    {
        if (p->find(k))
            *p->vals[p->keys[0] >= k ? 0 : 1] = v;
        if (p->keys[0] >= k)
            p = p->first;
        else if (p->keys[1] >= k)
            p = p->second;
        else
            p = p->third;
    }
}


template<class key, class val>
one<key, val>* tree_23<key, val>::search(one<key, val>* p, const key& k) { // Поиск ключа k в 2-3 дереве с корнем p.
    if (!p) return nullptr;

    if (p->find(k)) return p;
    else if (k < p->keys[0]) return search(p->first, k);
    else if ((p->size == 2) && (k < p->keys[1]) || (p->size == 1)) return search(p->second, k);
    else if (p->size == 2) return search(p->third, k);
}


template<class key, class val>
one<key, val>* tree_23<key, val>::insert(one<key, val>* p, const key& k, const val& v) { // вставка ключа k в дерево с корнем p; всегда возвращаем корень дерева, т.к. он может меняться
    if (!p) return new one<key, val>(k, v); // если дерево пусто, то создаем первую 2-3-вершину (корень)

    if(k == p->k)
        throw t23_exep(std::string("Key already exist."), 1);

    if (p->is_leaf())
        p->insert_to_node(k, v);
    else if (k <= p->keys[0])
        insert(p->first, k, v);
    else if ((p->size == 1) || ((p->size == 2) && k <= p->keys[1]))
        insert(p->second, k, v);
    else
        insert(p->third, k, v);

    return split(p);
}


template<class key, class val>
one<key, val>* tree_23<key, val>::split(one<key, val>* item) {
    if (item->size < 3) return item;

    one<key, val>* x = new one<key, val>(item->keys[0], *item->vals[0], item->first, item->second, nullptr, nullptr, item->parent); // Создаем две новые вершины,
    one<key, val>* y = new one<key, val>(item->keys[2], *item->vals[2], item->third, item->fourth, nullptr, nullptr, item->parent);  // которые имеют такого же родителя, как и разделяющийся элемент.
    if (x->first)  x->first->parent = x;    // Правильно устанавливаем "родителя" "сыновей".
    if (x->second) x->second->parent = x;   // После разделения, "родителем" "сыновей" является "дедушка",
    if (y->first)  y->first->parent = y;    // Поэтому нужно правильно установить указатели.
    if (y->second) y->second->parent = y;

    if (item->parent) {
        item->parent->insert_to_node(item->keys[1], *item->vals[1] );

        if (item->parent->first == item) item->parent->first = nullptr;
        else if (item->parent->second == item) item->parent->second = nullptr;
        else if (item->parent->third == item) item->parent->third = nullptr;

        // Дальше происходит своеобразная сортировка ключей при разделении.
        if (item->parent->first == nullptr) {
            item->parent->fourth = item->parent->third;
            item->parent->third = item->parent->second;
            item->parent->second = y;
            item->parent->first = x;
        }
        else if (item->parent->second == nullptr) {
            item->parent->fourth = item->parent->third;
            item->parent->third = y;
            item->parent->second = x;
        }
        else {
            item->parent->fourth = y;
            item->parent->third = x;
        }

        one<key, val>* tmp = item->parent;
        delete item;
        return tmp;
    }
    else {
        x->parent = item;   // Так как в эту ветку попадает только корень,
        y->parent = item;   // то мы "родителем" новых вершин делаем разделяющийся элемент.
        item->become_node2(item->keys[1], *item->vals[1], x, y);
        return item;
    }
}


template<class key, class val>
one<key, val>* tree_23<key, val>::search_min(one<key, val>* p) { // Поиск узла с минимальным элементов в 2-3-дереве с корнем p.
    if (!p) return p;
    if (!(p->first)) return p;
    else return search_min(p->first);
}


template<class key, class val>
one<key, val>* tree_23<key, val>::remove(one<key, val>* p, const key& k) { // Удаление ключа k в 2-3-дереве с корнем p.
    one<key, val>* item = search(p, k); // Ищем узел, где находится ключ k

    if (!item) 
        throw t23_exep(std::string("Key not found."), 0);

    one<key, val>* min = nullptr;
    if (item->keys[0] == k) 
        min = search_min(item->second); // Ищем эквивалентный ключ
    else
        min = search_min(item->third);

    if (min) { // Меняем ключи местами
        key& z = (k == item->keys[0] ? item->keys[0] : item->keys[1]);
        val *z_ = (k == item->keys[0] ? item->vals[0] : item->vals[1]);
        item->swap(z, z_, min->keys[0], min->vals[0] );
        item = min; // Перемещаем указатель на лист, т.к. min - всегда лист
    }

    item->remove_from_node(k); // И удаляем требуемый ключ из листа
    return fix(item); // Вызываем функцию для восстановления свойств дерева.
}


template<class key, class val>
one<key, val>* tree_23<key, val>::fix(one<key, val>* leaf) {
    if (leaf->size == 0 && leaf->parent == nullptr) { // Случай 0, когда удаляем единственный ключ в дереве
        delete leaf;
        return nullptr;
    }
    if (leaf->size != 0) { // Случай 1, когда вершина, в которой удалили ключ, имела два ключа
        if (leaf->parent) return fix(leaf->parent);
        else return leaf;
    }

    one<key, val>* parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
    else if (parent->size == 2 && parent->third->size == 2) leaf = redistribute(leaf); // Аналогично
    else leaf = merge(leaf); // Случай 3, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину

    return fix(leaf);
}


template<class key, class val>
one<key, val>* tree_23<key, val>::redistribute(one<key, val>* leaf) {
    one<key, val>* parent = leaf->parent;
    one<key, val>* first = parent->first;
    one<key, val>* second = parent->second;
    one<key, val>* third = parent->third;

    if ((parent->size == 2) && (first->size < 2) && (second->size < 2) && (third->size < 2)) {
        if (first == leaf) {
            parent->first = parent->second;
            parent->second = parent->third;
            parent->third = nullptr;
            parent->first->insert_to_node(parent->keys[0], *parent->vals[0] );
            parent->first->third = parent->first->second;
            parent->first->second = parent->first->first;

            if (leaf->first != nullptr) parent->first->first = leaf->first;
            else if (leaf->second != nullptr) parent->first->first = leaf->second;

            if (parent->first->first != nullptr) parent->first->first->parent = parent->first;

            parent->remove_from_node(parent->keys[0]);
            delete first;
        }
        else if (second == leaf) {
            first->insert_to_node(parent->keys[0], *parent->vals[0] );
            parent->remove_from_node(parent->keys[0]);
            if (leaf->first != nullptr) first->third = leaf->first;
            else if (leaf->second != nullptr) first->third = leaf->second;

            if (first->third != nullptr) first->third->parent = first;

            parent->second = parent->third;
            parent->third = nullptr;

            delete second;
        }
        else if (third == leaf) {
            second->insert_to_node(parent->keys[1], *parent->vals[1] );
            parent->third = nullptr;
            parent->remove_from_node(parent->keys[1]);
            if (leaf->first != nullptr) second->third = leaf->first;
            else if (leaf->second != nullptr) second->third = leaf->second;

            if (second->third != nullptr)  second->third->parent = second;

            delete third;
        }
    }
    else if ((parent->size == 2) && ((first->size == 2) || (second->size == 2) || (third->size == 2))) {
        if (third == leaf) {
            if (leaf->first != nullptr) {
                leaf->second = leaf->first;
                leaf->first = nullptr;
            }

            leaf->insert_to_node(parent->keys[1], *parent->vals[1] );
            if (second->size == 2) {
                parent->keys[1] = second->keys[1];
                eq(parent->vals[1], second->vals[1]);
                second->remove_from_node(second->keys[1]);
                leaf->first = second->third;
                second->third = nullptr;
                if (leaf->first != nullptr) leaf->first->parent = leaf;
            }
            else if (first->size == 2) {
                parent->keys[1] = second->keys[0];
                eq(parent->vals[1], second->vals[0]);
                leaf->first = second->second;
                second->second = second->first;
                if (leaf->first != nullptr) leaf->first->parent = leaf;

                second->keys[0] = parent->keys[0];
                eq(second->vals[0], parent->vals[0]);
                parent->keys[0] = first->keys[1];
                eq(parent->vals[0], first->vals[1]);
                first->remove_from_node(first->keys[1]);
                second->first = first->third;
                if (second->first != nullptr) second->first->parent = second;
                first->third = nullptr;
            }
        }
        else if (second == leaf) {
            if (third->size == 2) {
                if (leaf->first == nullptr) {
                    leaf->first = leaf->second;
                    leaf->second = nullptr;
                }
                second->insert_to_node(parent->keys[1], *parent->vals[1]);
                parent->keys[1] = third->keys[0];
                eq(parent->vals[1], third->vals[0]);
                third->remove_from_node(third->keys[0]);
                second->second = third->first;
                if (second->second != nullptr) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = nullptr;
            }
            else if (first->size == 2) {
                if (leaf->second == nullptr) {
                    leaf->second = leaf->first;
                    leaf->first = nullptr;
                }
                second->insert_to_node(parent->keys[0], *parent->vals[0] );
                parent->keys[0] = first->keys[1];
                eq(parent->vals[0], first->vals[1]);
                first->remove_from_node(first->keys[1]);
                second->first = first->third;
                if (second->first != nullptr) second->first->parent = second;
                first->third = nullptr;
            }
        }
        else if (first == leaf) {
            if (leaf->first == nullptr) {
                leaf->first = leaf->second;
                leaf->second = nullptr;
            }
            first->insert_to_node(parent->keys[0], *parent->vals[0] );
            if (second->size == 2) {
                parent->keys[0] = second->keys[0];
                eq(parent->vals[0], second->vals[0]);
                second->remove_from_node(second->keys[0]);
                first->second = second->first;
                if (first->second != nullptr) first->second->parent = first;
                second->first = second->second;
                second->second = second->third;
                second->third = nullptr;
            }
            else if (third->size == 2) {
                parent->keys[0] = second->keys[0];
                eq(parent->vals[0], second->vals[0]);
                second->keys[0] = parent->keys[1];
                eq(second->vals[0], parent->vals[1]);
                parent->keys[1] = third->keys[0];
                eq(parent->vals[1], third->vals[0]);
                third->remove_from_node(third->keys[0]);
                first->second = second->first;
                if (first->second != nullptr) first->second->parent = first;
                second->first = second->second;
                second->second = third->first;
                if (second->second != nullptr) second->second->parent = second;
                third->first = third->second;
                third->second = third->third;
                third->third = nullptr;
            }
        }
    }
    else if (parent->size == 1) {
        leaf->insert_to_node(parent->keys[0], *parent->vals[0]);

        if (first == leaf && second->size == 2) {
            parent->keys[0] = second->keys[0];
            eq(parent->vals[0], second->vals[0]);
            second->remove_from_node(second->keys[0]);

            if (leaf->first == nullptr) leaf->first = leaf->second;

            leaf->second = second->first;
            second->first = second->second;
            second->second = second->third;
            second->third = nullptr;
            if (leaf->second != nullptr) leaf->second->parent = leaf;
        }
        else if (second == leaf && first->size == 2) {
            parent->keys[0] = first->keys[1];
            eq(parent->vals[0], first->vals[1]);
            first->remove_from_node(first->keys[1]);

            if (leaf->second == nullptr) leaf->second = leaf->first;

            leaf->first = first->third;
            first->third = nullptr;
            if (leaf->first != nullptr) leaf->first->parent = leaf;
        }
    }
    return parent;
}


template<class key, class val>
one<key, val>* tree_23<key, val>::merge(one<key, val>* leaf) {
    one<key, val>* parent = leaf->parent;

    if (parent->first == leaf) {
        parent->second->insert_to_node(parent->keys[0], *parent->vals[0]);
        parent->second->third = parent->second->second;
        parent->second->second = parent->second->first;

        if (leaf->first != nullptr) parent->second->first = leaf->first;
        else if (leaf->second != nullptr) parent->second->first = leaf->second;

        if (parent->second->first != nullptr) parent->second->first->parent = parent->second;

        parent->remove_from_node(parent->keys[0]);
        delete parent->first;
        parent->first = nullptr;
    }
    else if (parent->second == leaf) {
        parent->first->insert_to_node(parent->keys[0], *parent->vals[0]);

        if (leaf->first != nullptr) parent->first->third = leaf->first;
        else if (leaf->second != nullptr) parent->first->third = leaf->second;

        if (parent->first->third != nullptr) parent->first->third->parent = parent->first;

        parent->remove_from_node(parent->keys[0]);
        delete parent->second;
        parent->second = nullptr;
    }

    if (parent->parent == nullptr) {
        one<key, val>* tmp = nullptr;
        if (parent->first != nullptr) tmp = parent->first;
        else tmp = parent->second;
        tmp->parent = nullptr;
        delete parent;
        return tmp;
    }
    return parent;
}
















///////////////////////////////////////////////////////////////////////////////////////

enum tree_types { BT, AVL, SPLAY, RB, T23 };



template<class key, class val>
class relation
{
private:
    std::map<std::string, base_tree<key, val>*> arr;

    
    bool checkIndex(const std::string& index)
    {
        if (!isalpha(index[0]) || index.size()>32)
            return false;
        for (int i = 1; i < index.size(); i++)
        {
            if (!isalnum(index[i]))
                return false;
        }
        return true;
    }

    //void tr_copy(const tree_types type, one<key, val>* node, base_tree<key, val>* tree);
    //void trace_copy(base_tree<key, val>* tree);

    //void trace23(one<key, val>* node, tree_23<key, val>* tr);
    //void trace(one<key, val>* node, bin_tree<key, val>* tr);
public:
    relation(){}
    ~relation(){}

    relation& addIndex( const tree_types type, Compare<key>* comp, std::string index);
    relation& remIndex(const std::string& index);
    relation& addData(const key& k, const val& v);
    relation& remData(const key& k);
    val findData(const key& k, const std::string& index);



    class relation_exception : public std::exception
    {
    private:
        std::string msg;
        int code;

    public:
        relation_exception(const std::string& msg, const int code) { this->msg = msg; this->code = code; }
        relation_exception(const relation_exception& ex) { this->msg = ex.msg; this->code = ex.code; }

        relation_exception(relation_exception&& ex) { this->msg = ex.msg; this->code = ex.code; }

        const char* what() { return msg.c_str(); }

        int cod() { return code; }
    };



};






template<class key, class val>
relation<key, val>& relation<key, val>::addIndex(const tree_types type, Compare<key>* comp, std::string index)
{
    if (!checkIndex(index))
        throw relation_exception(index+std::string(" - this is wrong input index-key."), -2);
    if(arr.find(index)!=arr.end())
        throw relation_exception(index+std::string(" - this index-key already exist."), -1);

    base_tree<key, val>* tree = nullptr;

    if (type == tree_types::BT)
        tree = new bin_tree<key, val>(comp);
    else if (type == tree_types::AVL)
        tree = new AVL_tree<key, val>(comp);
    else if (type == tree_types::SPLAY)
        tree = new splay_tree<key, val>(comp);
    else if (type == tree_types::RB)
        tree = new RBtree<key, val>(comp);
    else if (type == tree_types::T23)
        tree = new tree_23<key, val>(comp);

    if (!arr.empty())
    {
        auto tmp = arr.begin();
        tree->trace(typeid(*(tmp->second)) == typeid(tree_23<key, val>), *tree, tmp->second->get_top());
    }

    arr.insert(std::make_pair(index, tree));

    return *this;
}


template <class key, class val>
relation<key, val>& relation<key, val>::remIndex(const std::string& index)

{
    auto it = arr.find(index);

    if(it==arr.end())
        throw relation_exception(index + std::string(" - this index-key not found."), -3);

    it->second->delete_tree();
    arr.erase(it);
    return *this;
}




template <class key, class val>
relation<key, val>& relation<key, val>::addData(const key& k, const val& v)
{
    auto it = arr.begin();
    for (; it != arr.end(); it++)//std::map::iterator)
        *(it->second)+= std::make_pair(k, v);

    return *this;
}




template <class key, class val>
relation<key, val>& relation<key, val>::remData(const key& k)
{
    auto tmp = arr.begin();
    for (; tmp != arr.end(); tmp++)
        *(tmp->second) -= k;
    return *this;
}



template <class key, class val>
val relation<key, val>::findData(const key& k, const std::string& index)
{

    auto it = arr.find(index);

    if(it == arr.end())
        throw relation_exception(index + std::string(" - this index-key not found."), -3);

    return (*(it->second))[k];
}







