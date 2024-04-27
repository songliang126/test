#include "find_if.h"

#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

void findif()
{
    int x = 5;
    vector<int> vec = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto pos = find_if(vec.begin(), vec.end(), [=](int n) {
        return n == x;
    }); 

    if (pos == vec.end()) {
        cout << "vector do not have x!";
    } else {
        cout << "vector have x in pos " << *pos << endl; 
    }
}
