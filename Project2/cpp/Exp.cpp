//
// Created by 86133 on 2022/9/29.
//
#include "Exp.h"

Exp::Exp() {
    num.push_back(0);
    sign = true;
}

Exp::Exp(int a) {
    if (a == 0) {
        num.push_back(0);
    } else {
        num.push_back(1);
        num.push_back(a);
    }
}

//重载加法运算符，用于计算Exp类型和long long类型相加
//采取先将long long加在vector的第二项，之后依次进位的方法
Exp Exp::operator+(long long int_num) const{
    long long length = num[0];
    Exp ans;
    ans.sign = sign;
    ans.num.assign(num.begin(), num.end());
    //分2种大情况讨论
    //一是两者同号
    //二是两者异号 两者异号中大数为正，小数为负
    //           两者异号中大数为负，小数为正
    //三种情况运算过程是统一的
    //使用tmp_sign存储sign的负号,在结尾转换
    //以便让负大数 + 正小数 转换为 正大数 + 负小数
    bool tmp_sign = sign;
    if (!sign && int_num > 0) {
        ans.sign = true;
        int_num = -int_num;
    }
    //大数 与 小数 同为负
    if (int_num <= 0 && !ans.sign) int_num = abs(int_num);
    //如果 大数 为0 ，则只讲将小数压入大数的vector中
    //否则 则将 小数 加在 大数的个位
    if (length == 0) {
        ans.num[0] = 1;
        ans.num.push_back(int_num);
    } else {
        ans.num[1] += int_num;
    }
    //从 大数 的个位开始逐位计算
    //对于已经为负数的位，可以采取向高位借位直到当前位处于 0到10 之间
    //对正数的位，仅需要考虑大于10进位
    for (int i = 1; i < length; i++) {
        if (ans.num[i] < 0) {
            ans.num[i + 1] += (ans.num[i] / 10 - 1);
            if(ans.num[i] % 10 == 0) {
                ans.num[i + 1]++;
                ans.num[i] %= 10;
            } else {
                ans.num[i] = (ans.num[i] % 10) + 10;
            }
        } else {
            ans.num[i + 1] += ans.num[i] / 10;
            ans.num[i] = ans.num[i] % 10;
        }
    }
    //处理最高位绝对值仍大于10的情况
    //做法与从个位开始逐位计算一致
    while (abs(ans.num[ans.num[0]++]) >= 10) {
        length = ans.num[0] - 1;
        ans.num.push_back(ans.num[length] / 10);
            ans.num[length] = abs((ans.num[length] % 10));
    }
    ans.num[0]--;
    //归还为了简便计算而暂存的sign
    //如果最高位为负数，则将符号取反
    ans.sign = tmp_sign;
    if (ans.num[ans.num[0]] < 0) {
        ans.num[ans.num[0]] = -ans.num[ans.num[0]];
        ans.sign = !ans.sign;
    }
    //去除最高位的0
    while (ans.num[0] > 0 && ans.num[ans.num[0]] == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    if(ans.num[0] == 0) ans.sign = true;
    ans.num.shrink_to_fit();
    return ans;
}

//大数减法，
//此处认为减数 和 被减数符号为正，仅计算两者绝对值的差不涉及符号判断
Exp Exp::operator-(const Exp &Exp_num) const{
    Exp ans;
    vector<long> big_num, small_num;
    if (this->CompareNum(Exp_num)) {
        big_num.assign(num.begin(), num.end());
        small_num.assign(Exp_num.num.begin(), Exp_num.num.end());
        ans.sign = true;
        ans.num[0] = num[0];
    } else {
        big_num.assign(Exp_num.num.begin(), Exp_num.num.end());
        small_num.assign(num.begin(), num.end());
        ans.sign = false;
        ans.num[0] = Exp_num.num[0];
    }
    for (long long i = 1; i <= ans.num[0]; i++) ans.num.push_back(big_num[i]);
    for (long long i = 1; i <= small_num[0]; i++) {
        ans.num[i] = ans.num[i] - small_num[i];
        if (ans.num[i] < 0) {
            ans.num[i + 1]--;
            ans.num[i] += 10;
        }
    }
    for (long long i = small_num[0] + 1; i < big_num[0]; i++) {
        if (ans.num[i] < 0) {
            ans.num[i + 1]--;
            ans.num[i] += 10;
        }
    }
    while (ans.num[0] > 0 && ans.num[ans.num[0]] == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    ans.num.shrink_to_fit();
    return ans;
}

Exp Exp::operator+(const Exp &Exp_num) const{
    Exp ans;
    //大数加法分2种情况
    //一是同号
    //二是异号，异号可以调用减法
    if (sign == Exp_num.sign) {
        ans.sign = sign;
        vector<long> big_num, small_num;
        //以下比较两个数的大小，区分出大的和小的大数
        if (num[0] > Exp_num.num[0] || num[0] == Exp_num.num[0] && num[num[0]] >= Exp_num.num[Exp_num.num[0]]) {
            big_num.assign(num.begin(), num.end());
            small_num.assign(Exp_num.num.begin(), Exp_num.num.end());
            ans.num[0] = num[0];
        } else {
            big_num.assign(Exp_num.num.begin(), Exp_num.num.end());
            small_num.assign(num.begin(), num.end());
            ans.num[0] = Exp_num.num[0];
        }
        //将上述比较中大的数存储给ans.num
        for (long long i = 1; i <= ans.num[0]; i++) ans.num.push_back(big_num[i]);
        ans.num[0]++;
        ans.num.push_back(0);
        for (long long i = 1; i <= ans.num[0]; i++) {
            if (i <= small_num[0]) {
                ans.num[i] = ans.num[i] + small_num[i];
                ans.num[i + 1] += ans.num[i] / 10;
                ans.num[i] = ans.num[i] % 10;
            } else {
                ans.num[i + 1] += ans.num[i] / 10;
                ans.num[i] = ans.num[i] % 10;
            }
        }
        if (ans.num[ans.num[0]] >= 10) {
            ans.num.push_back(ans.num[ans.num[0]] / 10);
            ans.num[ans.num[0]++] %= 10;
        }
    } else {
        if (sign) {
            ans = *this - Exp_num;
        } else {
            ans = *this - Exp_num;
            ans.sign = !ans.sign;
        }
        return ans;
    }
    while (ans.num[0] > 0 && ans.num[ans.num[0]] == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    ans.num.shrink_to_fit();
    return ans;
}

bool Exp::operator>=(const Exp &Exp_num) const{
    if(sign && !Exp_num.sign) return true;
    if(!sign && Exp_num.sign) return false;
    long long length1 = num[0],length2 = Exp_num.num[0];
    if(sign) {
        if(length1 > length2) return true;
        if(length1 < length2) return false;
        for(long long i = length1;i >= 1;i--) {
            if(num[i] > Exp_num.num[i]) return true;
            else if(num[i] < Exp_num.num[i]) return false;
        }
        return true;
    } else {
        if(length1 > length2) return false;
        if(length1 < length2) return true;
        for(long long i = length1;i >= 1;i--) {
            if(num[i] > Exp_num.num[i]) return false;
            else if(num[i] < Exp_num.num[i]) return true;
        }
        return true;
    }
}
bool Exp::operator>(const Exp &Exp_num) const{
    if(sign && !Exp_num.sign) return true;
    if(!sign && Exp_num.sign) return false;
    long long length1 = num[0],length2 = Exp_num.num[0];
    if(sign) {
        if(length1 > length2) return true;
        if(length1 < length2) return false;
        for(long long i = length1;i >= 1;i--) {
            if(num[i] > Exp_num.num[i]) return true;
            else if(num[i] < Exp_num.num[i]) return false;
        }
        return false;
    } else {
        if(length1 > length2) return false;
        if(length1 < length2) return true;
        for(long long i = length1;i >= 1;i--) {
            if(num[i] > Exp_num.num[i]) return false;
            else if(num[i] < Exp_num.num[i]) return true;
        }
        return false;
    }
}
//Exp operator + (Exp )
void Exp::Print() {
    if (!sign) cout << "-";
    for (long long i = num[0]; i >= 1; i--) {
        printf("%lld", num[i]);
    }
    printf("\n");
}
bool Exp::CompareNum(const Exp &Exp_num) const{
    long long length1 = num[0],length2 = Exp_num.num[0];
    if(length1 > length2) return true;
    if(length1 < length2) return false;
    for(long long i = length1;i >= 1;i--) {
        if(num[i] > Exp_num.num[i]) return true;
        else if(num[i] < Exp_num.num[i]) return false;
    }
    return true;
}