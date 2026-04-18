#include <iostream>
#include <iomanip>
#include "TreeRegistry.h"

using namespace std;

int main()
{
    std::cout << "Hello World!\n";

    StrCompare* s = new StrCompare;
    IntCompare* i = new IntCompare;


    try
    {
        TreeRegistry<string, int> btr;

        btr.addIndex(TreeTypes::BT, s, string("BT")).addIndex(TreeTypes::AVL, s, string("AVL")).addIndex(TreeTypes::SPLAY, s, string("SPLAY"));
        btr.addIndex(TreeTypes::RB, s, string("RB")).addIndex(TreeTypes::T23, s, string("T23"));



        btr.addData(string("10"), 10).addData(string("20"), 20).addData(string("15"), 15).addData(string("30"), 30).addData(string("5"), 5);
        cout << btr.findData(string("15"), string("T23")) << endl;

        btr.remData(string("10"));
        btr.remIndex(string("SPLAY"));

        // проверка на нахождение ошибок
        //btr.addIndex(TreeTypes::BT, s, string("1BT")); // ключ индекс не правильный в объекте класса типа TreeRegistry
        //btr.addIndex(TreeTypes::BT, s, string("BT"));  // ключ индекс уже существует в объекте класса типа TreeRegistry
        //btr.remIndex(string("RANDOM"));  // отсутсвие ключа индекса в объекте класса типа TreeRegistry
        //cout << btr.findData(string("10"), string("T23")) << endl;  // отсутсвие ключа в объекте класса типа tree_23
        //btr.addData(string("20"), 20);  // ключ уже существует в дереве
        //btr.remData(string("10")); // // отсутсвие ключа в дереве
    }
    catch (BinaryTreeException& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (AVLTreeException& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (SplayTreeException& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (RedBlackTreeException& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (Tree23Exception& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch (TreeRegistry<string, int>::TreeRegistryException& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }

	delete i;
	delete s;

    return 0;
}
