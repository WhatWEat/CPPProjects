//
// Created by 86133 on 2022/9/29.
//

#include "Function.h"
using namespace std;

int main() {
    Number a("0.03");
    a.Print();
    Number b("0.05");
    b.Print();
    string in;
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
 * 2022/10/10
 * 1.解决除法问题
 * 2.解决小数点问题
 * 3.解决赋值问题
 * */
/*todo
 * 2022/10/11
 * 1.修复与e先关的小bug
 * input:3*-1e3/5+6
 * output:-6-14
 */
/*todo
 * 2022/10/14
 * 1.有多余括号报错
 * 2.补全函数操作
 * */