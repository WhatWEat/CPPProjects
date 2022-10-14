//
// Created by 86133 on 2022/9/29.
//

#include "Function.h"
using namespace std;

int main() {
    Number a("3.1415926535897932384626433832795028");
    a.Print();
    Number b("12100000000000000000000000000000000");
    b.Print();
    string in;
    (a+b).Print();

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
/*todo
 * 2022/10/14
 * 1.补全函数操作
 * 2022/10/15
 * 1.完善sqr精度
 * 2.补全sin
 * */