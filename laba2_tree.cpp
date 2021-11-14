// laba2_tree.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <iomanip>
#include "Header.h"

using namespace std;

int main()
{
    std::cout << "Hello World!\n";

    StrCompare* s = new StrCompare;
    IntCompare* i = new IntCompare;


    try
    {
        relation<string, int> btr;

        btr.addIndex(tree_types::BT, s, string("BT")).addIndex(tree_types::AVL, s, string("AVL")).addIndex(tree_types::SPLAY, s, string("SPLAY"));
        btr.addIndex(tree_types::RB, s, string("RB")).addIndex(tree_types::T23, s, string("T23"));



        btr.addData(string("10"), 10).addData(string("20"), 20).addData(string("15"), 15).addData(string("30"), 30).addData(string("5"), 5);
        cout << btr.findData(string("15"), string("T23")) << endl;

        btr.remData(string("10"));
        btr.remIndex(string("SPLAY"));

        // проверка на нахождение ошибок
        //btr.addIndex(tree_types::BT, s, string("1BT")); // ключ индекс не правильный в объекте класса типа relation
        // btr.addIndex(tree_types::BT, s, string("BT"));  // ключ индекс уже существует в объекте класса типа relation
        //btr.remIndex(string("RANDOM"));  // отсутсвие ключа индекса в объекте класса типа relation
        //cout << btr.findData(string("10"), string("T23")) << endl;  // отсутсвие ключа в объекте класса типа tree_23
        //btr.addData(string("20"), 20);  // ключ уже существует в дереве
        //btr.remData(string("10")); // // отсутсвие ключа в дереве
    }
    catch (bt_exep& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (AVL_exep& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (splay_exep& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (RB_exep& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (t23_exep& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (relation<string, int>::relation_exception& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }



    return 0;
}

