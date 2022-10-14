//
// Created by 86133 on 2022/9/29.
//

#ifndef PROJECTS_NUMBER_H
#define PROJECTS_NUMBER_H
#include "Exp.h"

class Number {
public:
    //记录计算过程中是否存在错误;
    static bool error;
    //记录用户需求的精确位数
    static size_t scale;
    bool sign = true;
    //用于记录小数点的位置，-1表示输入中无小数点
    long long point = -1;
    //此处的e等同于科学计数法中的e
    Exp e;
    //num的下标0用于存储数字总长度，
    //倒序存储数字，用下标1处表示除去小数点后的个位，2表示十位，以此类推
    vector<long long> num;

    //将输入的字符串转化为Number
    //在转换过程中会保留正确输入的正负号(sign)，小数点(point)
    explicit Number(const string &input);
    Number();

    //加减乘除，都模拟竖式计算
    Number operator*(const Number &number) const;
    //整除
    Number operator/(Number &number) const;
    //取模
    Number operator%(Number &number) const;
    //除法多一个参数用于表示精确位数
    Number Div(Number &number,long long n) const;
    Number operator+(const Number &number) const;
    //减法仅在被加号异号情况才被调用
    Number operator-(const Number &number) const;
    //此处大数 + 小数用的方法与Exp类中的加法基本一致
    Number operator+(long number) const;
    //开根号函数
    Number sqr();
    //通过传入的Number类型的number输出对应的数字
    //在数字末尾连续的0个数小于等于9时，会直接输出，例如1000000仍输出为1000000
    //在数字末尾连续的0个数大于9时，则会使用科学技术的方法，例如850000000输出为85e7
    //同时会比较设置的精度，输出位数小于设置的精度scale则会补0
    void Print();
private:
    //MoveDigit函数生成除法过程中的移位操作后的用于试商的数
    void MoveDigit(const Number &number,long long start);
    //用于比较number中的num和this的num
    int CompareNum(const Number &number) const;
    Number CopyNumber() const;
};
#endif //PROJECTS_NUMBER_H
