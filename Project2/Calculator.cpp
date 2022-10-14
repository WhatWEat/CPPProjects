//
// Created by 86133 on 2022/9/29.
//

#include "Function.h"
using namespace std;

int main() {
    Number a("-1");
    a.Print();
    Number b("0.05");
    b.Print();
    string in;
    (a+b).Print();
    a.Div(b,1).Print();
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
 * 1.有多余括号报错
 * 2.补全函数操作
 * */