#pragma once
#include "BaseTree.h"


class Tree23Exception : public TreeException
{
protected:
    using TreeException::msg;
    using TreeException::code;
public:
    Tree23Exception(const std::string& msg, const int code):TreeException(msg, code) { }
    Tree23Exception(const Tree23Exception& ex) : TreeException(ex) {}
};



template <class key, class val>
class Tree23: public BaseTree<key, val> {
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



    TreeNode<key, val>* split(TreeNode<key, val>* item); // Метод для разделение вершины при переполнении;
    TreeNode<key, val>* insert(TreeNode<key, val>* p, const key& k, const val& v); // Вставка в дерево;
    TreeNode<key, val>* search(TreeNode<key, val>* p, const key& k); // Поиск в дереве;
    TreeNode<key, val>* search_min(TreeNode<key, val>* p); // Поиск минимального элемента в поддереве;
    TreeNode<key, val>* merge(TreeNode<key, val>* leaf); // Слияние используется при удалении;
    TreeNode<key, val>* redistribute(TreeNode<key, val>* leaf); // Перераспределение также используется при удалении;
    TreeNode<key, val>* fix(TreeNode<key, val>* leaf); // Используется после удаления для возвращения свойств дереву (использует merge или redistribute)
    TreeNode<key, val>* remove(TreeNode<key, val>* p, const key& k); // Собственно, из названия понятно;


protected:
    virtual bool tr_pre(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_inf(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));
    virtual bool tr_post(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t));

    virtual void trace_del(TreeNode<key, val>* node);
    virtual void trace_copy(TreeNode<key, val>* source, TreeNode<key, val>* destination);

    virtual TreeNode<key, val>* add(const key& k, const val& v) { return nullptr; }
    virtual void edit(const key& k, const val& v);
    virtual TreeNode<key, val>* del(TreeNode<key, val>* p, const key& k) { return nullptr; }
    virtual val find(const key& k) { val a{ 0 }; return a; }

public:
    Compare<key>* comp = nullptr;
    TreeNode<key, val>* top = nullptr;

    Tree23(Compare<key>* c) { comp = c; }
    Tree23(Compare<key>* c, const Tree23& tree) { comp = c; *this = tree; }
    ~Tree23() { trace_del(top); top = nullptr; }


    virtual Tree23& operator+=(const std::pair<key, val>& o) { top = insert(top, o.first, o.second); return *this; }
    virtual Tree23& operator*=(const std::pair<key, val>& o) { TreeNode<key, val>* p = search(top, o.first);
    if (!p) throw Tree23Exception(std::string("Key not found (in function operator*= Tree23)."), 0); edit(o.first, o.second); return *this; }
    virtual Tree23& operator-=(const key& k) { top = remove(top, k); return *this; }
    virtual val operator[](const key& k) {
        TreeNode<key, val>* p = search(top, k);
        if (!p)
            throw Tree23Exception(std::string("Key not found (in function operator[] Tree23)."), 0);
        return *p->vals[k > p->keys[0] ? 1 : 0]; }

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
bool Tree23<key, val>::tr_pre(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
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
 bool Tree23<key, val>::tr_inf(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
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
bool Tree23<key, val>::tr_post(TreeNode<key, val>* node, size_t depth, val(*action)(const val&, const key&, const size_t))
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
void Tree23<key, val>::trace_pre(val(*action)(const val&, const key&, const size_t))
{
    this->tr_pre(top, 0, action);
    return ;
}



template <class key, class val>
void Tree23<key, val>::trace_inf(val(*action)(const val&, const key&, const size_t))
{
    this->tr_inf(top, 0, action);

    return ;
}

template <class key, class val>
void Tree23<key, val>::trace_post(val(*action)(const val&, const key&, const size_t))
{

    this->tr_post(top, 0, action);

    return ;
}


template <class key, class val>
void Tree23<key, val>::trace_del(TreeNode<key, val>* node)
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
void Tree23<key, val>::trace_copy(TreeNode<key, val>* source, TreeNode<key, val>* destination)
{
    if (source == nullptr)
        return;

    if (source->left != nullptr)
    {
        destination->left = new TreeNode<key, val>;
        for (int i = 0; i < source->left->size; i++)
        {
            destination->left->keys[i] = source->left->keys[i];
            destination->left->vals[i] = new val(*source->left->vals[i]);
        }
    }
    if (source->right != nullptr)
    {
        destination->right = new TreeNode<key, val>;
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
void Tree23<key, val>::edit(const key& k, const val& v) {
    TreeNode<key, val>* p = search(top, k);
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
TreeNode<key, val>* Tree23<key, val>::search(TreeNode<key, val>* p, const key& k) { // Поиск ключа k в 2-3 дереве с корнем p.
    if (!p) return nullptr;

    if (p->find(k)) return p;
    else if (k < p->keys[0]) return search(p->first, k);
    else if ((p->size == 2) && (k < p->keys[1]) || (p->size == 1)) return search(p->second, k);
    else if (p->size == 2) return search(p->third, k);
	else return nullptr;
}


template<class key, class val>
TreeNode<key, val>* Tree23<key, val>::insert(TreeNode<key, val>* p, const key& k, const val& v) { // вставка ключа k в дерево с корнем p; всегда возвращаем корень дерева, т.к. он может меняться
    if (!p) return new TreeNode<key, val>(k, v); // если дерево пусто, то создаем первую 2-3-вершину (корень)

    if(k == p->k)
        throw Tree23Exception(std::string("Key already exist (in function insert Tree23)."), 1);

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
TreeNode<key, val>* Tree23<key, val>::split(TreeNode<key, val>* item) {
    if (item->size < 3) return item;

    TreeNode<key, val>* x = new TreeNode<key, val>(item->keys[0], *item->vals[0], item->first, item->second, nullptr, nullptr, item->parent); // Создаем две новые вершины,
    TreeNode<key, val>* y = new TreeNode<key, val>(item->keys[2], *item->vals[2], item->third, item->fourth, nullptr, nullptr, item->parent);  // которые имеют такого же родителя, как и разделяющийся элемент.
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

        TreeNode<key, val>* tmp = item->parent;
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
TreeNode<key, val>* Tree23<key, val>::search_min(TreeNode<key, val>* p) { // Поиск узла с минимальным элементов в 2-3-дереве с корнем p.
    if (!p) return p;
    if (!(p->first)) return p;
    else return search_min(p->first);
}


template<class key, class val>
TreeNode<key, val>* Tree23<key, val>::remove(TreeNode<key, val>* p, const key& k) { // Удаление ключа k в 2-3-дереве с корнем p.
    TreeNode<key, val>* item = search(p, k); // Ищем узел, где находится ключ k

    if (!item)
        throw Tree23Exception(std::string("Key not found (in function remove Tree23)."), 0);

    TreeNode<key, val>* min = nullptr;
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
TreeNode<key, val>* Tree23<key, val>::fix(TreeNode<key, val>* leaf) {
    if (leaf->size == 0 && leaf->parent == nullptr) { // Случай 0, когда удаляем единственный ключ в дереве
        delete leaf;
        return nullptr;
    }
    if (leaf->size != 0) { // Случай 1, когда вершина, в которой удалили ключ, имела два ключа
        if (leaf->parent) return fix(leaf->parent);
        else return leaf;
    }

    TreeNode<key, val>* parent = leaf->parent;
    if (parent->first->size == 2 || parent->second->size == 2 || parent->size == 2) leaf = redistribute(leaf); // Случай 2, когда достаточно перераспределить ключи в дереве
    else if (parent->size == 2 && parent->third->size == 2) leaf = redistribute(leaf); // Аналогично
    else leaf = merge(leaf); // Случай 3, когда нужно произвести склеивание и пройтись вверх по дереву как минимум на еще одну вершину

    return fix(leaf);
}


template<class key, class val>
TreeNode<key, val>* Tree23<key, val>::redistribute(TreeNode<key, val>* leaf) {
    TreeNode<key, val>* parent = leaf->parent;
    TreeNode<key, val>* first = parent->first;
    TreeNode<key, val>* second = parent->second;
    TreeNode<key, val>* third = parent->third;

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
TreeNode<key, val>* Tree23<key, val>::merge(TreeNode<key, val>* leaf) {
    TreeNode<key, val>* parent = leaf->parent;

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
        TreeNode<key, val>* tmp = nullptr;
        if (parent->first != nullptr) tmp = parent->first;
        else tmp = parent->second;
        tmp->parent = nullptr;
        delete parent;
        return tmp;
    }
    return parent;
}
