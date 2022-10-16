//
// Created by 86133 on 2022/9/29.
//

#include "Function.h"
using namespace std;

int main() {
    string in;
    getline(cin, in);
    while (!(in == "exit")) {
        Number::error = false;
        //判断是否存在set语句
        if(JudgeSet(in)) {
            getline(cin, in);
            continue;
        }
        //去除空格
        Format(in);
        //判断是否是赋值语句
        int whether_assign = JudgeAssign(in);
        if(!whether_assign) {
            Number ans = ReadToCal(in);
            if(!Number::error) ans.Print();
        }
        getline(cin, in);
    }
}
