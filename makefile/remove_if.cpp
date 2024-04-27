#include "remove_if.h"

#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

void dispList(list<int> L)
{
    list<int>::iterator liter;
    cout << "list.size = " << L.size() << endl;
    for_each(L.begin(), L.end(), [](int a) {
        cout << a << " ";
    });
    cout << endl;
}

void removeif()
{
    list<int> iList = {10, 20, 11, 22, 21, -10, -20, 13, 55, 44};

    dispList(iList);

    /*remove_if(iList.begin(), iList.end(), [](int n) {
        return n < 0;
    });

    dispList(iList);*/

    iList.erase(remove_if(iList.begin(), iList.end(), [](int n) {
        return n < 0;
    }), iList.end());

    dispList(iList);
}


