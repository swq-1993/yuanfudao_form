//
// Created by sunwanqi on 2018/11/9.
//
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
using namespace std;

int main()
{
    vector<int> a{1, 2, 3, 4, 5};
    vector<int> b{2, 4, 1, 6, 7};
    vector<int> inter;
    sort(a.begin(), a.end());
    sort(b.begin(), b.end());
    set_intersection(a.begin(), a.end(), b.begin(), b.end(), back_inserter(inter));
    for (int i : inter)
    {
        cout << i << " ";
    }
    cout << "intersection" << endl;
}
