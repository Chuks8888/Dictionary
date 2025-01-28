#include "dictionary.h"
//#include <locale>

int main()
{
    Dictionary<int, std::string> tree;

    tree.insert(10, "Root");
    tree.insert(5, "Left");
    tree.insert(15, "Right");
    tree.insert(3, "test");
    tree.insert(7, "test1");
    tree.insert(0, "huo");
    tree.insert(30, "hopsiu");
    tree.insert(28, "lala");
    tree.insert(4, "cos");


    tree.drawDictionary();
    tree.remove(3);
    tree.drawDictionary();
    //tree.printKey(30);
    return 0;
}