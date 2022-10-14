//
// Created by 86133 on 2022/9/16.
//
#include<iostream>
#include <vector>

using namespace std;

class Exp {
public:
    bool sign = true;
    vector<long long> num;

    Exp() {
        num.push_back(0);
    }

    explicit Exp(int a) {
        if (a == 0) {
            num.push_back(0);
        } else {
            num.push_back(1);
            num.push_back(a);
        }
    }

    //重载加法运算符，用于计算Exp类型和long long类型相加
    //采取先将long long加在vector的第二项，之后依次进位的方法
    Exp operator+(long long int_num) {
        long long length = num[0];
        //分2种大情况讨论
        //一是两者同号
        //二是两者异号 两者异号中大数为正，小数为负
        //           两者异号中大数为负，小数为正
        //三种情况运算过程是统一的
        //使用tmp_sign存储sign的负号,在结尾转换
        //以便让负大数 + 正小数 转换为 正大数 + 负小数
        bool tmp_sign = sign;
        if (!sign && int_num > 0) {
            sign = true;
            int_num = -int_num;
        }
        if (sign || int_num <= 0 && !sign) {
            if (int_num <= 0 && !sign) int_num = abs(int_num);
            if (length == 0) {
                num[0] = 1;
                num.push_back(int_num);
            } else {
                num[1] += int_num;
            }
            //如果他只是-6
            for (int i = 1; i < length; i++) {
                if (num[i] < 0) {
                    num[i + 1] += (num[i] / 10 - 1);
                    num[i] = (num[i] % 10) + 10;
                } else {
                    num[i + 1] += num[i] / 10;
                    num[i] = num[i] % 10;
                }
            }
            while (abs(num[num[0]++]) >= 10) {
                length = num[0] - 1;
                num.push_back(num[length] / 10);
                if (num[length] < 0) num[length] = (num[length] % 10) + 10;
                else num[length] = num[length] % 10;
            }
            num[0]--;
        }
        sign = tmp_sign;
        if (num[num[0]] < 0) {
            num[num[0]] = -num[num[0]];
            sign = false;
        }
        //去除最高位的0
        while (num[0] > 0 && num[num[0]] == 0) {
            num.pop_back();
            num[0]--;
        }
        num.shrink_to_fit();
        return *this;
    }

    //大数减法，
    //此处认为减数 和 被减数符号为正，仅计算两者绝对值的差不涉及符号判断
    Exp operator-(const Exp &Exp_num) {
        Exp ans;
        vector<long> big_num, small_num;
        if (num[0] > Exp_num.num[0] || num[0] == Exp_num.num[0] && num[num[0]] >= Exp_num.num[Exp_num.num[0]]) {
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

    Exp operator+(const Exp &Exp_num) {
        Exp ans;
        //大数加法分2种情况
        //一是同号
        //二是异号，异号可以调用减法
        if (sign == Exp_num.sign) {
            ans.sign = sign;
            vector<long> big_num, small_num;
            if (num[0] > Exp_num.num[0] || num[0] == Exp_num.num[0] && num[num[0]] >= Exp_num.num[Exp_num.num[0]]) {
                big_num.assign(num.begin(), num.end());
                small_num.assign(Exp_num.num.begin(), Exp_num.num.end());
                ans.num[0] = num[0];
            } else {
                big_num.assign(Exp_num.num.begin(), Exp_num.num.end());
                small_num.assign(num.begin(), num.end());
                ans.num[0] = Exp_num.num[0];
            }
            for (long long i = 1; i <= ans.num[0]; i++) ans.num.push_back(big_num[i]);
            for (long long i = 1; i < ans.num[0]; i++) {
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

    //Exp operator + (Exp )
    void Print() {
        if (!sign) cout << "-";
        for (long long i = num[0]; i >= 1; i--) {
            printf("%lld", num[i]);
        }
        printf("\n");
    }
};

class Number {
public:
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

    //通过传入的Number类型的number输出对应的数字
    //在数字末尾连续的0个数小于等于6时，会直接输出，例如1000000仍输出为1000000
    //在数字末尾连续的0个数大于6时，则会使用科学技术的方法，例如850000000输出为8.5e8
    void Print();

    Number operator*(Number &number) const {
        Number ans("0");
        //乘数中有一个为0则直接返回
        if (number.num[0] == 0 || num[0] == 0) return ans;
        //符号
        ans.sign = (number.sign == sign);
        //小数点和e的位数
        ans.e = number.e + (number.point == -1 ? 0 : -number.point);
        ans.e = ans.e + (point == -1 ? 0 : -point);
        ans.e = ans.e + e;
        ans.point = -1;
        //数字乘法
        ans.num[0] = number.num[0] + num[0];
        for (int i = 1; i <= ans.num[0]; i++) ans.num.push_back(0);
        for (int i = 1; i <= num[0]; i++) {
            for (int j = 1; j <= number.num[0]; j++) {
                int index = i + j - 1;
                ans.num[index] += number.num[j] * num[i];
                ans.num[index + 1] += ans.num[index] / 10;
                ans.num[index] = ans.num[index] % 10;
            }
        }
        //去除头部0
        if (ans.num[0] > 0 && ans.num.back() == 0) {
            ans.num.pop_back();
            ans.num[0]--;
        }
        //去除尾部0
        for (int i = 1; i < ans.num[0] && ans.num[1] == 0; i++) {
            ans.e = ans.e + 1;
            ans.num.erase(ans.num.begin() + 1);
            ans.num[0]--;
        }
        ans.num.shrink_to_fit();
        return ans;
    }
};

//根据输入的错误代码0,1……来输出错误信息
void PrintError(int error);

//先倒序遍历查找输入中带有数学特殊意义的字符'PI''e'等,
//若找到则根据不同字符依据数值大小存储共50位有效数字，结束程序
//若未找到则再次倒序遍历输入，
//将小数点位置，正负号，具体数字，数字长度，
//分别存入point,sign,num,num[0]中
Number::Number(const string &input) {
    num.push_back(0);
    unsigned long long pos = input.find('e'), length = input.length();
    //目的是为了让tmp_string的下标从1开始
    string tmp_string = " " + input;
    if (pos < length) {
        for (unsigned long long i = length; i > pos + 1; i--) {
            char tmp = tmp_string[i];
            if ((tmp == '-' || tmp == '+') && i == pos + 2) {
                e.sign = tmp != '-';
            } else if (tmp > '9' || tmp < '0') {
                PrintError(1);
            } else {
                e.num.push_back(tmp - '0');
                e.num[0]++;
            }
        }
        length = pos;
        //从顶端往下查找，删去前导0
        //条件大于0防止e.num[0]成为-1
        while (e.num[e.num[0]] == 0 && e.num[0] > 0) {
            e.num[0]--;
            e.num.pop_back();
        }

        //缩减vector占用内存
        e.num.shrink_to_fit();
    }
    if (length > LLONG_MAX) PrintError(2);
    //临时存储字符串input中的末尾0
    long long tmp_e = 0;
    for (auto i = (long long) length; i >= 1; i--) {
        if (tmp_string[i] == '0' && num[0] == 0) {
            tmp_e++;
        } else if (tmp_string[i] >= '0' && tmp_string[i] <= '9') {
            num.push_back(tmp_string[i] - '0');
            num[0]++;
        } else if (point == -1 && tmp_string[i] == '.') {
            point = num[0];
            tmp_e = 0;
        } else if (i == 1 && (tmp_string[i] == '+' || tmp_string[i] == '-')) {
            if (tmp_string[i] == '-') sign = false;
        } else {
            PrintError(1);
        }
    }
    //从顶端往下查找，删去前导0
    while (num[num[0]] == 0 && num[0] > 0) {
        num[0]--;
        num.pop_back();
    }
    num.shrink_to_fit();
    if (num[0] != 0) e = e + tmp_e;
}

void Number::Print() {
    if (!sign) cout << '-';

    for (long long i = num[0]; i >= 1; i--) {
        printf("%lld", num[i]);
    }
    //判断是否超出了6个后倒零
    //未超出则正常打出后倒零
    if (num[0] == 0) {
        printf("0");
    } else if (e.num[0] <= 1 && e.num[1] <= 6 && e.sign) {
        for (int i = 1; i <= e.num[1] && point <= 0; i++) printf("0");
    } else if (e.num[0] != 0) {
        printf("e");
        if (!e.sign) printf("-");
        for (long long i = e.num[0]; i >= 1; i--) {
            cout << e.num[i];
        }
    }
    printf("\n");
}

void PrintError(int error) {
    switch (error) {
        case 0:
            cout << "please input two numbers" << endl;
            break;
        case 1:
            cout << "The input cannot be interpret as numbers!" << endl;
            break;
        case 2:
            cout << "The numbers before 'e' are too many!" << endl;
            break;
        default:
            break;
    }
    exit(0);
}

int main(int argc, char *argv[]) {
    Number n1("123456789");
    Number n2("555444556");

    n1 = n1 * n2;
    n1.Print();
}