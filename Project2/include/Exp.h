//
// Created by 86133 on 2022/9/29.
//

#ifndef PROJECTS_EXP_H
#define PROJECTS_EXP_H
#include <vector>
#include <iostream>
using namespace std;
class Exp {
public:
    bool sign = true;
    vector<long long> num;

    Exp();
    explicit Exp(int a);
    //重载加法运算符，用于计算Exp类型和long long类型相加
    //采取先将long long加在vector的第二项，之后依次进位的方法
    Exp operator+(long long int_num) const;
    //大数减法，
    //此处认为减数 和 被减数符号为正，仅计算两者绝对值的差不涉及符号判断
    Exp operator-(const Exp &Exp_num) const;
    Exp operator+(const Exp &Exp_num) const;
    //判断两个大数的大小
    bool operator>=(const Exp &Exp_num) const;
    bool operator>(const Exp &Exp_num) const;
    void Print();
};
void PrintError(int error);
#endif //PROJECTS_EXP_H
