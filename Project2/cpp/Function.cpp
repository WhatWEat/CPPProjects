//
// Created by 86133 on 2022/10/6.
//

#include "Function.h"
#include <algorithm>
#include <stack>
#include <unordered_map>

using namespace std;
//利用Hashmap来存储变量到数的映射
unordered_map<string, Number> Variables;

//返回值不同表示不同类型的处理方式
//1表示数字，2表示运算符，3表示函数,4表示变量
//-1则表示输入的字符不在此列则，为错误
int Judge(const string &s, size_t i) {
    char c = s[i];
    //判断当前的输入为数字
    if ('0' <= c && c <= '9' || (c == 'e' && s[i + 1] != 'x') || c == '.') return 1;
    //判断当前的负号是否和数字相连
    //'-'为数字的符号仅有2种情况
    //1.字符串以'-'开头
    //2.'-'前仍存在'(','*','/','+','%'等运算符
    if (c == '-') {
        if (i == 0) return 1;
        if (s[i - 1] == '*' || s[i - 1] == '/' || s[i - 1] == '+' ||
            s[i - 1] == '(' || s[i - 1] == '%' || s[i - 1] == '-') {
            return 1;
        } else {
            return 2;
        }
    }
    //判断当前符号是否跟数字为一体
    //判断方法与负号相同
    if (c == '+') {
        if (i == 0) return 1;
        if (s[i - 1] == '*' || s[i - 1] == '/' || s[i - 1] == '+' ||
            s[i - 1] == '(' || s[i - 1] == '%' || s[i - 1] == '-') {
            return 1;
        } else {
            return 2;
        }
    }
    if (c == '*' || c == '/' || c == '(' || c == ')' || c == '%') return 2;
    //如果3个字母和一个括号对比函数库，如果符合则证明为函数
    string tmp = s.substr(i, 4);
    if (tmp == "sqr(" || tmp == "exp(" || tmp == "sin(" || tmp == "cos(") return 3;
    //如果是字母则考虑可能为变量
    if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z') return 4;
    //判断是否为赋值
    //赋值的格式为: X = 表达式
    //所以必须以未保留的关键词(sqrt,exp等开头)
    //要求是:
    //1.有且仅有1个等号
    //2.等号左边有且仅有英文字母
    //3.等式右边是可以计算的表达式
    return 4;
}

//纠正格式，把用户输入的空格全部删去，最前和最后加括号
string Format(string &s) {
    auto re = remove(s.begin(), s.end(), ' ');
    s.erase(re, s.end());
    return s;
}

int GetPriority(const string &op) {
    int priority = -10;
    if (op == to_string('+') || op == to_string('-')) {
        priority = 0;
    } else if (op == to_string('/') || op == to_string('*') || op == to_string('%')) {
        priority = 1;
    } else if (op == "sqr" || op == "sin" || op == "cos" || op == "exp") {
        priority = 2;
    } else if (op == to_string('(')) {
        priority = -1;
    }
    return priority;
}

//通过字符串来进行运算符计算
Number GetOp(const Number &number1, Number number2, const string &op) {
    Number ans;
    if (op == to_string('+')) {
        ans = number1 + number2;
    } else if (op == to_string('-')) {
        number2.sign = !number2.sign;
        ans = number1 + number2;
        number2.sign = !number2.sign;
    } else if (op == to_string('/')) {
        ans = number1 / number2;
    } else if (op == to_string('*')) {
        ans = number1 * number2;
    } else if (op == to_string('%')) {
        ans = number1 % number2;
    }
    return ans;
}

Number GetOp(const Number &number, const string &op) {
    Number ans;
    if (op == "sqr") {
        ans = number + 1;
    } else if (op == "cos") {
        ans = number + 2;
    } else if (op == "sin") {
        ans = number + 3;
    } else if (op == "exp") {
        ans = number + 4;
    }
    return ans;
}

//利用数字栈 符号栈来进行四则运算
Number ReadToCal(string in) {
    stack<Number> numbers;
    stack<string> op;
    //在表达式添加一对括号
    in = "(" + in + ")";
    string string_number;
    for (size_t i = 0; i < in.length() && !Number::error; i++) {
        switch (Judge(in, i)) {
            //表示读入是数字的情况
            case 1:
                string_number += in[i];
                break;
                //表示读入是操作符的情况
            case 2: {
                string string_op = to_string(in[i]);
                if (!string_number.empty()) {
                    Number number(string_number);
                    numbers.push(number);
                    string_number = "";
                }
                if (string_op == to_string(')')) {
                    string_op = op.top();
                    while (string_op != to_string('(')) {
                        Number num1 = numbers.top();
                        numbers.pop();
                        Number num2 = numbers.top();
                        numbers.pop();
                        numbers.push(GetOp(num2, num1, string_op));
                        op.pop();
                        string_op = op.top();
                    }
                    op.pop();
                    //处理函数
                    if (!op.empty() && (op.top() == "sqr" || op.top() == "exp" || op.top() == "sin" || op.top() == "cos")) {
                        string_op = op.top();
                        Number num = numbers.top();
                        numbers.pop();
                        numbers.push(GetOp(num, string_op));
                        op.pop();
                    }
                } else if (string_op == to_string('(') || GetPriority(string_op) > GetPriority(op.top())) {
                    op.push(string_op);
                } else {
                    while (GetPriority(string_op) <= GetPriority(op.top())) {
                        Number num1 = numbers.top();
                        numbers.pop();
                        Number num2 = numbers.top();
                        numbers.pop();
                        numbers.push(GetOp(num2, num1, op.top()));

                        op.pop();
                    }
                    op.push(string_op);
                }
                break;
            }
                //处理读入是函数的情况
            case 3: {
                string string_op = in.substr(i, 3);
                op.push(string_op);
                i += 2;
                break;
            }
                //表示读入是字母的情况
            case 4: {
                if (!string_number.empty()) {
                    PrintError(4);
                } else {
                    size_t index_variable = i;
                    while (Judge(in, ++i) == 4);
                    string variable = in.substr(index_variable, i - index_variable);
                    auto it = Variables.find(variable);
                    if (it != Variables.end()) {
                        numbers.push(it->second);
                    } else {
                        PrintError(4);
                    }
                    i--;
                }
                break;
            }
        }
    }
    return numbers.top();
}

//遍历字符串in，查找其是否有且仅有1个等于，同时标记等于号下标为index_equal
//若无等于号或等于号多于1个，则报错且退出该函数
//第二次在in中从0遍历至index_equal，
//若等号左边全为字母，则认为该表达式存在赋值,把等号右边的表达式交由ReadToCal计算，建立起string到Number的映射
int JudgeAssign(const string &in) {
    size_t index_equal = 0;
    for (size_t i = 0; i < in.length(); i++) {
        if (in[i] == '=') {
            if (index_equal) {
                PrintError(5);
                return -1;
            }
            index_equal = i;
        }
    }
    if (index_equal == 0) {
        return 0;
    }
    for (size_t i = 0; i < index_equal; i++) {
        if (!(in[i] >= 'a' && in[i] <= 'z' || 'A' <= in[i] && in[i] <= 'Z')) {
            PrintError(5);
            return -1;
        }
    }
    string variable = in.substr(0, index_equal);
    string expression = in.substr(index_equal + 1);
    Number tmp = ReadToCal(expression);
    if (Number::error) return -1;
    else Variables.insert(make_pair(variable, tmp));
    return 1;
}

//先通过截取开头4个字符来判断句中是否存在set关键词
bool JudgeSet(string &in) {
    string set_statement;
    set_statement = in.substr(0, 4);
    if (set_statement != "set ") return false;
    //去除空格
    Format(in);
    in = in.substr(3);
    size_t index_equal = 0;
    for (size_t i = 0; i < in.length(); i++) {
        if (in[i] == '=') {
            if (index_equal) {
                PrintError(6);
                return true;
            }
            index_equal = i;
        }
    }
    string variable = in.substr(0, index_equal);
    string expression = in.substr(index_equal + 1);
    if (variable != "scale") {
        PrintError(6);
        return true;
    }
    size_t tmp = 0;
    for (char i: expression) {
        if ('0' <= i && i <= '9') {
            tmp = tmp * 10 + (i - '0');
        } else {
            PrintError(6);
            return true;
        }
    }
    Number::scale = tmp;
    return true;
}