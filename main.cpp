#include "dictionary.h"
#include "assert.h"
//#include <locale>

int main()
{
    Dictionary<int, std::string> tree;

    // Insert
    tree.insert(10, "Root");
    tree.insert(5, "Left");
    tree.insert(15, "Right");
    tree.insert(3, "test");
    tree.insert(7, "test1");
    tree.insert(0, "huo");
    tree.insert(30, "hopsiu");
    tree.insert(28, "lala");
    assert(tree.insert(4, "cos") == 1);
    assert(tree.getSize() == 9);

    // Remove
    tree.remove(3);
    assert(tree.getSize() == 8);

    // Copy constructor
    Dictionary<int, std::string> tree2(tree);
    assert(tree2.getSize() == 8);

    // Clear
    assert(tree.clear() == 1);
    assert(tree.clear() == 0);
    assert(tree.getSize() == 0);

    // copy
    assert(tree.copy(tree) == 0);
    assert(tree.copy(tree2) == 1);
    assert(tree.getSize() == 8);

    // add
    tree2.clear();
    tree2.insert(8, "hopis");
    tree2.insert(6, "hos");
    tree2.insert(9, "doom");
    tree.add(tree2);
    assert(tree.getSize() == 11);
    
    // update
    assert(tree.update(28, "szony") == 1);
    assert(tree.update(70, "derby") == 0);

    // operator =
    Dictionary<int, std::string> tree3;
    tree3 = tree2;
    assert(tree3.getSize() == 3);

    tree.clear();
    tree.insert(3, "test");
    tree.insert(7, "test1");
    tree.insert(0, "huo");
    tree.insert(30, "hopsiu");
    tree.insert(28, "lala");
    assert(tree.getSize() == 5);

    tree2 = (tree + tree3);
    assert(tree2.getSize() == 8); 

    Dictionary<std::string, std::string> dict;

    dict.insert("banana", "a yellow fruit");
    dict.insert("apple", "a round fruit");
    dict.insert("grape", "an oddly shaped fruit");
    
    
    dict.drawDictionary();


    return 0;
}