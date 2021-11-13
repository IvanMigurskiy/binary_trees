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

    //bin_tree<int, int> btr(i);
    relation<int, int> btr;

    try
    {
        btr.addIndex(tree_types::SPLAY, i, string("Splay") );

        for (int i = 10; i <= 30; i += 10) 
            btr.addData(i, i-1);

        /*
        btr.addIndex(tree_types::T23, i, string("TREE23") );
        btr.remIndex(string("Splay"));
        btr.remData(10);
        cout << btr.findData(30, string("TREE23")) << endl;
        */
    




        /*for (int i = 0; i < 5; i++)
        {
            cin >> a;
            btr += make_pair(a, a);
        }

        btr.trace_post(print);

        cin >> a;
        btr -= a;
        
        btr.trace_post(print);*/
        /*
        btr += make_pair(10, 10);
        btr += make_pair(15, 15);
        btr += make_pair(20, 20);
        btr += make_pair(13, 13);
        btr += make_pair(27, 27);
        btr += make_pair(19, 19);
        btr += make_pair(16, 16);
        btr += make_pair(18, 18);
        */

        //for (int i = 1; i < 8; i++)
          //  btr += make_pair(i, i);


        /*
        for (int i = 10; i <= 150; i+=10)
            btr += make_pair(i, i);

        btr += make_pair(5, 5);
        btr += make_pair(15, 15);
        btr += make_pair(25, 25);
        btr += make_pair(8, 8);

        btr *= make_pair(5, 8);
        btr *= make_pair(8, 5);
        btr.Show();
        */


        /*
        btr += make_pair(15, 15);
        btr += make_pair(19, 19);
        btr += make_pair(16, 16);
        btr += make_pair(25, 25);
        */
 
        //btr.trace_post(print);
        //btr -= 15;
        //btr.trace_post(print);

        //btr.pr();
        /*
        btr.trace_post(print);
        btr -= 15;
        btr[10] = 123;
        cout << "\n" << btr[10] << endl << endl;
        cout << endl;

        //btr[20];

        btr.trace_post(print);*/
    }
    catch (bt_exep& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }
    catch(relation<int, int>::relation_exception& ex)
    {
        std::cerr << "Error " << ex.cod() << " : " << ex.what() << endl;
    }



    return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
