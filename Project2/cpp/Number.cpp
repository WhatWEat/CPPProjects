//
// Created by 86133 on 2022/9/29.
//

#include "Number.h"

//初始化静态变量
size_t Number::scale = 0;
bool Number::error = false;

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
                return;
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
            return;
        }
    }
    //从顶端往下查找，删去前导0
    while (num[num[0]] == 0 && num[0] > 0) {
        num[0]--;
        num.pop_back();
    }
    num.shrink_to_fit();
    if (num[0] != 0) {
        e = e + tmp_e;
    }
    e = e + (point == -1 ? 0 : -point);
    point = -1;
    if (num[0] == 0 && e.num[0] != 0) {
        num[0]++;
        num.push_back(1);
    }
}

Number::Number() {
    sign = true;
    point = -1;
    Exp new_e;
    e = new_e;
    num.push_back(0);
}

Number Number::operator*(const Number &number) const {
    Number ans("0");
    //乘数中有一个为0则直接返回
    if (number.num[0] == 0 || num[0] == 0) return ans;
    //符号
    ans.sign = (number.sign == sign);
    //小数点和e的位数
    ans.e = number.e;
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
    for (int i = 1; i < ans.num[0] && ans.num[1] == 0;) {
        ans.e = ans.e + 1;
        ans.num.erase(ans.num.begin() + 1);
        ans.num[0]--;
    }
    ans.num.shrink_to_fit();
    return ans;
}

//分两种情况
//有2种除法:1整除,2有精确度除法
//对于整除
//  a.被除数大于除数 直接调用精确除法
//  b.被除数小于除数 输出0
//对于精确度除法
//  除到有对应位数即可
Number Number::operator/(Number &number) const {
    if (scale == 0) {
        long long length_num1 = num[0], length_num2 = number.num[0];
        Number ans;
        number.e.sign = !number.e.sign;
        Exp tmp = e + number.e;
        number.e.sign = !number.e.sign;
        long long n = 0;
        for (long long i = tmp.num[0]; i >= 1; i--) {
            n = n * 10 + tmp.num[i];
        }
        n *= (tmp.sign ? 1 : -1);
        //如果this小于number，则直接输出0
        Exp tmp1 = e + length_num1;
        Exp tmp2 = number.e + length_num2;
        if (e + length_num1 >= number.e + length_num2) {
            if (e + length_num1 > number.e + length_num2) {
                return Div(number, n);
            } else {
                if (length_num1 < length_num2) return ans;
                if (length_num1 > length_num2) return Div(number, n);
                if (CompareNum(number)) {
                    return Div(number, n);
                } else {
                    return ans;
                }
            }
        } else {
            return ans;
        }
    } else {
        return Div(number, scale);
    }
}

//取模直接调用整除
Number Number::operator%(Number &number) const {
    size_t tmp = scale;
    scale = 0;
    Number ans;
    Number reminder = *this / number;
    Number minus = reminder * number;
    minus.sign = !minus.sign;
    ans = *this + minus;
    scale = tmp;
    return ans;
}

Number Number::Div(Number &number, long long n) const {
    Number ans("0"), tmp, reminder;
    //被除数为0则直接返回0
    //如果除数为0则直接报错
    if (!num[0]) {
        return ans;
    } else if (!number.num[0]) {
        PrintError(3);
        return ans;
    }
    ans.sign = (sign == number.sign);
    //变换减数前面的符号，把减法表示成加法
    number.e.sign = !number.e.sign;
    ans.e = e + number.e + -n;
    number.e.sign = !number.e.sign;
    ans.num[0] = this->num[0] - number.num[0] + 1 + n;
    for (long long i = 1; i <= ans.num[0]; i++) {
        ans.num.push_back(0);
    }
    reminder.e = e;
    reminder.MoveDigit(*this, n + 1);
    reminder.sign = true;
    for (long long i = ans.num[0]; i > 0; i--) {
        //将number移位到与商对齐
        tmp.MoveDigit(number, i);
        while (reminder.CompareNum(tmp)) {
            ans.num[i]++;
            reminder = reminder - tmp;
        }
        //去除头部0
        while (reminder.num[0] > 0 && reminder.num.back() == 0) {
            reminder.num.pop_back();
            reminder.num[0]--;
        }
    }
    //去除头部0
    while (ans.num[0] > 0 && ans.num.back() == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    //去除尾部0
    for (int i = 1; i < ans.num[0] && ans.num[1] == 0;) {
        ans.e = ans.e + 1;
        ans.num.erase(ans.num.begin() + 1);
        ans.num[0]--;
    }
    return ans;
}

//先检查两位的e是否对齐
//如果对齐则直接相加
//为对齐，则将大的一方在number后补0以减小e的大小，直到原本偏大的e与较小的e相等，再直接按位相加
Number Number::operator+(const Number &number) const {
    Number ans("0");
    Number big_number, small_number;
    //找出两个e中较大的那一个和较小的那一个
    if (e >= number.e) {
        big_number = *this;
        small_number = number;
    } else {
        small_number = *this;
        big_number = number;
    }
    //将big_number的e表示的后导0存储到ans.num中
    ans.e = small_number.e;
    while (big_number.e > small_number.e) {
        ans.num.push_back(0);
        ans.num[0]++;
        small_number.e = small_number.e + 1;
    }
    //将 big_number 存储的各位数字放入ans.num中
    for (long long i = 1; i <= big_number.num[0]; i++) {
        ans.num.push_back(big_number.num[i]);
        ans.num[0]++;
    }
    //异号用减法处理，同号用加法处理
    if (sign == number.sign) {
        long long dis = 0;
        //将 small_number.num超出ans.num[0]的部分直接读入
        while (ans.num[0] < small_number.num[0]) {
            dis++;
            ans.num.push_back(small_number.num[++ans.num[0]]);
        }
        for (long long i = 0; i < dis; i++) {
            small_number.num.pop_back();
            small_number.num[0]--;
        }
        for (long long i = 1; i <= small_number.num[0] && i < ans.num[0]; i++) {
            ans.num[i] += small_number.num[i];
            ans.num[i + 1] += ans.num[i] / 10;
            ans.num[i] = ans.num[i] % 10;
        }
        if (small_number.num[0] == ans.num[0]) {
            ans.num[ans.num[0]] += small_number.num[ans.num[0]];
            ans.num.push_back(ans.num[ans.num[0]] / 10);
            ans.num[ans.num[0]] %= 10;
            ans.num[0]++;
        }
    } else {
        if (big_number.sign) {
            ans = ans - small_number;
        } else {
            ans = small_number - ans;
        }
    }
    //去除尾部0
    while (ans.num[0] > 0 && ans.num[1] == 0) {
        ans.e = ans.e + 1;
        ans.num.erase(ans.num.begin() + 1);
        ans.num[0]--;
    }
    //弹出最高位的0
    while (ans.num[0] > 0 && ans.num.back() == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    if (!ans.num[0]) {
        Exp new_e;
        ans.e = new_e;
    }
    return ans;
}

//因为减法仅在 加法异号 除法试商 情况调用，
//所以不考虑把两者当做同号处理，同时也不用考虑e的对齐
Number Number::operator-(const Number &number) const {
    Number ans("0"), big_number, small_number;
    ans.sign = true;
    //判断 减数 和 被减数大小
    //若减数小，则把ans符号改为负
    //若被减数小，ans的符号则为正
    if (num[0] < number.num[0]) {
        ans.sign = false;
    } else if (num[0] == number.num[0]) {
        for (long long i = num[0]; i >= 1; i--) {
            if (num[i] < number.num[i]) {
                ans.sign = false;
                break;
            } else if (num[i] > number.num[i]) {
                break;
            } else if (i == 1) {
                ans.point = -1;
                return ans;
            }
        }
    }
    if (ans.sign) {
        big_number = CopyNumber();
        small_number = number.CopyNumber();
    } else {
        small_number = CopyNumber();
        big_number = number.CopyNumber();
    }
    //把big_number中信息存储到ans中
    ans.num[0] = big_number.num[0];
    ans.e = big_number.e;
    ans.point = big_number.point;
    for (long long i = 1; i <= big_number.num[0]; i++) {
        ans.num.push_back(big_number.num[i]);
    }
    //进行减法运算
    for (long long i = 1; i <= small_number.num[0]; i++) {
        ans.num[i] -= small_number.num[i];
        if (ans.num[i] < 0) {
            ans.num[i] += 10;
            ans.num[i + 1]--;
        }
    }
    //处理高位的移除
    for (long long i = small_number.num[0] + 1; i < big_number.num[0]; i++) {
        if (ans.num[i] < 0) {
            ans.num[i] += 10;
            ans.num[i + 1]--;
        }
    }
    //弹出最高位的0
    if (ans.num[0] > 0 && ans.num.back() == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    //压缩vector体积
    ans.num.shrink_to_fit();
    return ans;
}

Number Number::operator+(long number) const {
    Number ans = CopyNumber();
    if (!sign && number > 0) number = -number;
    if (num[0] == 0) {
        ans.num.push_back(number);
        ans.num[0]++;
    } else {
        ans.num[1] += number;
    }
    //从个位开始到最高位进行进位或退位运算
    for (int i = 1; i < num[0]; i++) {
        if (ans.num[i] < 0) {
            ans.num[i + 1] += (ans.num[i] / 10 - 1);
            //如果低位是10的倍数，则需要把多减的一退回，同时对低位进行膜10运算
            //但如果最低位不是10的倍数，则需要多借一位来使低位在膜10后变成正数
            if (ans.num[i] % 10 == 0) {
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
    //处理最高位进位问题
    while (abs(ans.num[ans.num[0]]) >= 10) {
        long long length = ans.num[0]++;
        ans.num.push_back(ans.num[length] / 10);
        ans.num[length] = abs((ans.num[length] % 10));
    }
    //处理最高位的负号
    if (ans.num[ans.num[0]] < 0) {
        ans.num[ans.num[0]] = -ans.num[ans.num[0]];
        ans.sign = !ans.sign;
    }
    //去除最高位的0
    while (ans.num[0] > 0 && ans.num[ans.num[0]] == 0) {
        ans.num.pop_back();
        ans.num[0]--;
    }
    ans.num.shrink_to_fit();
    //如果
    if (ans.num[0] == 0) {
        Exp new_e;
        ans.e = new_e;
    }
    return ans;
}
//逻辑与
Number Number::operator&(const Number &number) const{
    Number ans;
    if(num[0] && number.num[0]) {
        ans.num.push_back(1);
        ans.num[0]++;
    }
    return ans;
}
//逻辑或
Number Number::operator|(const Number &number) const{
    Number ans;
    if(num[0] || number.num[0]) {
        ans.num.push_back(1);
        ans.num[0]++;
    }
    return ans;
}
//逻辑非
Number Number::Not() const{
    Number ans;
    if(!num[0]) {
        ans.num.push_back(1);
        ans.num[0]++;
    }
    return ans;
}
Number Number::CopyNumber() const {
    Number ans("0");
    ans.sign = sign;
    ans.num[0] = num[0];
    ans.e = e;
    ans.point = point;
    for (int i = 1; i <= num[0]; i++) ans.num.push_back(num[i]);
    return ans;
}

void Number::MoveDigit(const Number &number, long long start) {
    this->num.clear();
    this->num.shrink_to_fit();
    this->num.push_back(number.num[0] + start - 1);
    for (long long i = 1; i < start; i++) {
        this->num.push_back(0);
    }
    for (long long i = 1; i <= number.num[0]; i++) {
        this->num.push_back(number.num[i]);
    }
}

int Number::CompareNum(const Number &number) const {
    if (num[0] > number.num[0]) return 1;
    if (num[0] < number.num[0]) return 0;
    for (long long i = num[0]; i > 0; i--) {
        if (num[i] > number.num[i]) return 1;
        else if (num[i] < number.num[i]) return 0;
    }
    return -1;
}
int Number::CompareNumber(const Number &number) const {
    Exp sum_num1 = this->e+num[0],sum_num2 = number.e + number.num[0];
    if(sum_num1 > sum_num2) return 1;
    else if(sum_num1 >= sum_num2) {
        long long i = num[0],j = number.num[0];
        while(i > 0 && j > 0) {
            if(num[i] > number.num[j]) {
                return 1;
            } else if (num[i] < number.num[j]) {
                return -1;
            } else {
                i--;
                j--;
            }
        }
        if(!i && !j) return 0;
        else if(!i) return -1;
        else return 1;
    }
    else {
        return -1;
    }
}
void Number::Print() {
    if (!sign) cout << '-';
    //输出小数点
    //如果e大于-20，则正常输出数字中的小数点
    //lower_point表示小数点的位置
    long long lower_point = 0;
    if (!e.sign) {
        if (e.num[0] == 2 && e.num[2] <= 2) lower_point = e.num[2] * 10 + e.num[1];
        if (e.num[0] == 1) lower_point = e.num[1];
        if (lower_point >= num[0]) {
            printf("0.");
            for (long long i = lower_point; i > num[0]; i--) printf("0");
        }
    }
    for (long long i = num[0]; i >= 1; i--) {
        if (i != num[0] && i == lower_point) printf(".");
        printf("%lld", num[i]);
    }
    //在前面有小数点的情况下
    //如果scale大于num的位数，则通过补0来补充精度
    for (long long i = num[0]; i < scale && lower_point; i++) {
        printf("0");
    }
    //判断是否超出了9个后倒零
    //未超出则正常打出后倒零
    if (num[0] == 0) {
        printf("0");
    } else if (e.num[0] <= 1 && e.num[1] <= 9 && e.sign) {
        for (int i = 1; i <= e.num[1] && point <= 0; i++) printf("0");
        //补精确位数
        for (long long i = e.num[1] + num[0]; i < scale; i++) {
            if (i == e.num[1] + num[0]) printf(".");
            printf("0");
        }
    } else if (e.num[0] > 1 && !lower_point) {
        //补精确位数
        for (long long i = num[0]; i < scale; i++) {
            if (i == scale) printf(".");
            printf("0");
        }
        printf("e");
        if (!e.sign) printf("-");
        for (long long i = e.num[0]; i >= 1; i--) {
            cout << e.num[i];
        }
    }
    printf("\n");
}

//利用二分法,来搜索最接近的根号n
Number Number::Sqr() {
    Number mid, l, r = *this, div("2"), minus("-1"),add("1");
    if (!sign) {
        error = true;
        PrintError(7);
        return mid;
    }
    if (r.num[0] < scale) {
        l.MoveDigit(*this, (long long) scale - num[0] + 1);
    }
    while (r.CompareNumber(l) == 1) {
        mid = (l + r);
        mid = (mid + add);
        mid = mid / div;
        Number tmp = mid * mid;
        if (!tmp.CompareNumber(*this)) {
            l = mid;
            break;
        } else if (tmp.CompareNumber(*this) == 1) {
            r = mid + minus;
        } else {
            l = mid;
        }
    }
    return l;
}

Number Number::sin() {
    Number ans;
    size_t save_scale = scale;
    //计算进行回数
    int count = 1;
    scale = 30;
    Number dis("1");
    //当余项精确到0.1
    while(!(!dis.e.sign && dis.e.num[0] == 1)){
        Number tmp = getPow(count);
        if(count % 2 == 0) tmp.sign = !tmp.sign;
        Number div = getFactor(count * 2 - 1);
        dis = tmp /div;
        ans = ans + dis;
    }
    scale = save_scale;
    return ans;
}
Number Number::cos() {
    Number ans;
    return ans;
}
Number Number::getFactor(int n) {
    Number ans("1");
    Number plus("1"),count("1");
    for(int i = 1;i <= n;i++) {
        ans = ans * count;
        count = count + plus;
    }
    return ans;
}
Number Number::getPow(int n) {
    Number ans = *this;
    for(int i = 2;i <= n;i++) {
        ans = ans * *this;
    }
    return ans;
}
void PrintError(int error_code) {
    switch (error_code) {
        case 0:
            cout << "please input two numbers" << endl;
            break;
        case 1:
            cout << "The input cannot be interpret as numbers!" << endl;
            break;
        case 2:
            cout << "The numbers before 'e' are too many!" << endl;
            break;
        case 3:
            cout << "The divisor cannot be 0!" << endl;
            break;
        case 4:
            cout << "Please input correct variable" << endl;
            break;
        case 5:
            cout << "Please input correct statement to assign a variable" << endl;
            break;
        case 6:
            cout << "Set statement is wrong" << endl;
            break;
        case 7:
            cout << "Number in root can't be negative" << endl;
            break;
        case 8:
            cout << "The expression is wrong" << endl;
            break;
        default:
            break;
    }
    Number::error = true;
}