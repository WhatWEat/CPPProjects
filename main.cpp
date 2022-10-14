#include <iostream>
#include<bits/stdc++.h>
using namespace std;
unordered_map<string,int> Variables;
int main() {
    vector<int> num;
    stack<string> a;
    a.push("1");
    a.push("2");
    a.push("3");
    Variables.insert(make_pair("a",1));
    auto it = Variables.find("a");
    auto it2 = Variables.find("b");
    cout << it2->second << endl;
}
