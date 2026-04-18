#pragma once
#include "BaseTree.h"
#include "BinaryTree.h"
#include "AVLTree.h"
#include "SplayTree.h"
#include "RedBlackTree.h"
#include "Tree23.h"

enum TreeTypes { BT, AVL, SPLAY, RB, T23 };



template<class key, class val>
class TreeRegistry
{
private:
    std::map<std::string, BaseTree<key, val>*> arr;


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

public:
    TreeRegistry(){}
    ~TreeRegistry() {}// { auto it = arr.begin(); for (; arr.size() != 0;) remIndex(it->first); }

    TreeRegistry& addIndex( const TreeTypes type, Compare<key>* comp, std::string index);
    TreeRegistry& remIndex(const std::string& index);
    TreeRegistry& addData(const key& k, const val& v);
    TreeRegistry& remData(const key& k);
    val findData(const key& k, const std::string& index);



    class TreeRegistryException : public std::exception
    {
    private:
        std::string msg;
        int code;

    public:
        TreeRegistryException(const std::string& msg, const int code) { this->msg = msg; this->code = code; }
        TreeRegistryException(const TreeRegistryException& ex) { this->msg = ex.msg; this->code = ex.code; }

        TreeRegistryException(TreeRegistryException&& ex) { this->msg = ex.msg; this->code = ex.code; }

        const char* what() { return msg.c_str(); }

        int cod() { return code; }
    };



};






template<class key, class val>
TreeRegistry<key, val>& TreeRegistry<key, val>::addIndex(const TreeTypes type, Compare<key>* comp, std::string index)
{
    if (!checkIndex(index))
        throw TreeRegistryException(std::string(index+std::string(" - this is wrong input index-key (in function addIndex TreeRegistry).")), -2);
    if(arr.find(index)!=arr.end())
        throw TreeRegistryException(std::string(index+std::string(" - this index-key already exist (in function addIndex TreeRegistry).")), -1);

    BaseTree<key, val>* tree = nullptr;

    if (type == TreeTypes::BT)
        tree = new BinaryTree<key, val>(comp);
    else if (type == TreeTypes::AVL)
        tree = new AVLTree<key, val>(comp);
    else if (type == TreeTypes::SPLAY)
        tree = new SplayTree<key, val>(comp);
    else if (type == TreeTypes::RB)
        tree = new RedBlackTree<key, val>(comp);
    else if (type == TreeTypes::T23)
        tree = new Tree23<key, val>(comp);

    if (!arr.empty())
    {
        auto tmp = arr.begin();
        tree->trace(typeid(*(tmp->second)) == typeid(Tree23<key, val>), *tree, tmp->second->get_top());
    }

    arr.insert(std::make_pair(index, tree));

    return *this;
}


template <class key, class val>
TreeRegistry<key, val>& TreeRegistry<key, val>::remIndex(const std::string& index)

{
    auto it = arr.find(index);

    if(it==arr.end())
        throw TreeRegistryException(std::string(index + std::string(" - this index-key not found (in function remIndex TreeRegistry).")), -3);

    it->second->delete_tree();
    arr.erase(it);
    return *this;
}




template <class key, class val>
TreeRegistry<key, val>& TreeRegistry<key, val>::addData(const key& k, const val& v)
{
    auto it = arr.begin();
    for (; it != arr.end(); it++)
        *(it->second)+= std::make_pair(k, v);

    return *this;
}




template <class key, class val>
TreeRegistry<key, val>& TreeRegistry<key, val>::remData(const key& k)
{
    auto tmp = arr.begin();
    for (; tmp != arr.end(); tmp++)
        *(tmp->second) -= k;
    return *this;
}



template <class key, class val>
val TreeRegistry<key, val>::findData(const key& k, const std::string& index)
{

    auto it = arr.find(index);

    if(it == arr.end())
        throw TreeRegistryException(std::string(index + std::string(" - this index-key not found (in function findData TreeRegistry).")), -3);

    return (*(it->second))[k];
}




