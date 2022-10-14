//
// Created by 86133 on 2022/10/6.
//
#include "Number.h"

#ifndef PROJECTS_MATHFUNCTION_H
#define PROJECTS_MATHFUNCTION_H
//用于判断应当使用哪种处理方式
//返回0代表c作为组成数字字符串的一部分处理
//范围1表示c作为运算符
//范围2表示输入错误
int Judge(const string &c, size_t i);
string Format(string& s);
int GetPriority(const string &op);
Number GetOp(const Number& number1,Number number2,const string& op);
Number GetOp(Number number, const string& op);
//通过输入表达式的字符串，返回Number类型的结果值
Number ReadToCal(string in);
//输入in，判断字符串in是否为赋值语句
//利用int返回3中情况：
//1.返回1代表，in为赋值语句
//2.返回0表示，in不包含赋值语句，且无赋值的语法错误（字母前有数字，有多个等号)
//3.返回-1表示，in含赋值语句，但有语法错误
int JudgeAssign(const string& in);
bool JudgeSet(string& in);
#endif //PROJECTS_MATHFUNCTION_H
