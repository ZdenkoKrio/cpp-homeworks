#include <iostream>
#include <cmath>
#include "natural.hpp"
#include <cassert>
#include <algorithm>
#include <climits>


natural::natural(int v) {
    if (v < 0)
        throw std::out_of_range("Sorry, I dont know how to work with negative numbers.");
    num += std::to_string(v);
}

natural::natural(std::string n) {
    num = std::move(n);
}

char natural::int_to_char(int i) const {
    switch (i) {
        case 0:
            return '0';
        case 1:
            return '1';
        case 2:
            return '2';
        case 3:
            return '3';
        case 4:
            return '4';
        case 5:
            return '5';
        case 6:
            return '6';
        case 7:
            return '7';
        case 8:
            return '8';
        case 9:
            return '9';
        default:
            return '!';
    }
}

int natural::char_to_int(char c) const {
    switch (c) {
        case '0':
            return 0;
        case '1':
            return 1;
        case '2':
            return 2;
        case '3':
            return 3;
        case '4':
            return 4;
        case '5':
            return 5;
        case '6':
            return 6;
        case '7':
            return 7;
        case '8':
            return 8;
        case '9':
            return 9;
        default:
            return -1;
    }
}

natural natural::base_transfer(const natural &base, bool sum) const {
    natural result_sum{0};
    natural result_count{0};

    natural first = *this;
    while (first > natural{0}) {
        result_sum += first % base;
        result_count += natural(1);
        first = first / base;
    }

    if (sum) return result_sum;
    return result_count;
}

natural natural::digit_count(const natural &base) const {
    if (*this == natural(0))
        return natural(1);
    return base_transfer(base, false);
}

natural natural::digit_sum(const natural &base) const {
    return base_transfer(base, true);
}

natural natural::power(const natural &exponent) const {
    natural res = natural(1); // nechcem ist od nuly nahodou
    natural one = natural(1);

    for (natural i = natural(); i < exponent; i += one)
        res *= *this;

    return res;
}

natural natural::power(int exponent) const {
    return power(natural(exponent));
}

std::string natural::string_sum(const std::string &f, const std::string &s) const {
    std::string res;

    auto f_pos = f.rbegin();
    auto s_pos = s.rbegin();
    int reminder = 0;
    for (; f_pos != f.rend(); f_pos++) {
        int sumand = reminder;
        if (s_pos != s.rend()) {
            sumand += char_to_int(*s_pos);
            s_pos++;
        }
        int r = char_to_int(*f_pos) + sumand;
        if (r > 9)
            reminder = 1;
        else
            reminder = 0;
        r %= 10;
        res.push_back(int_to_char(r));
    }

    if (reminder > 0)
        res.push_back(int_to_char(reminder));

    std::reverse(res.begin(), res.end());
    return res;
}

natural &natural::operator+=(const natural &o) {
    if (num.size() >= o.num.size())
        num = string_sum(num, o.num);
    else
        num = string_sum(o.num, num);
    return *this;
}

natural natural::operator+(natural o) const {
    if (num.size() >= o.num.size())
        return natural(string_sum(num, o.num));
    return natural(string_sum(o.num, num));
}

std::string natural::string_multiple(const std::string &first, const std::string &second) const {
    std::string result = "0";
    unsigned long long int counter = 0;

    for (auto o_pos = second.rbegin(); o_pos != second.rend(); o_pos++) {
        std::string res;

        for (unsigned long long int i = 0; i < counter; i++) {
            res.push_back('0');
        }

        int reminder = 0;
        for (auto pos = first.rbegin(); pos != first.rend(); pos++) {
            int r = char_to_int(*o_pos) * char_to_int(*pos) + reminder;

            res.push_back(int_to_char(r % 10));

            reminder = r / 10;
        }
        if (reminder > 0)
            res.push_back(int_to_char(reminder));

        std::reverse(res.begin(), res.end());
        if (result.size() >= res.size())
            result = string_sum(result, res);
        else
            result = string_sum(res, result);

        counter++;
    }

    return result;
}

natural &natural::operator*=(const natural &o) {
    num = string_multiple(num, o.num);
    return *this;
}

natural natural::operator*(const natural &o) const {
    return natural(string_multiple(num, o.num));
}

natural natural::operator*(int o) const {
    return *this * natural(o);
}


std::string natural::string_minus(const std::string &first, const std::string &second) const {
    std::string res;

    auto f_position = first.rbegin();
    auto s_position = second.rbegin();
    int reminder = 0;
    for (; f_position != first.rend(); f_position++) {
        char res_char = '0';
        int f_i = char_to_int(*f_position);
        int s_i = reminder;
        if (s_position != second.rend()) {
            s_i += char_to_int(*s_position);
            s_position++;
        }

        if (f_i >= s_i) {
            res_char = int_to_char(f_i - s_i);
            reminder = 0;
        } else {
            res_char = int_to_char(10 - (s_i - f_i));
            reminder = 1;
        }
        res.push_back(res_char);
    }

    while (res[res.size() - 1] == '0') {
        if (res.empty() || res == "0")
            break;
        res.pop_back();
    }

    if (res.empty())
        return "0";

    std::reverse(res.begin(), res.end());
    return res;
}

natural &natural::operator-=(const natural &o) {
    if (*this < o)
        throw std::out_of_range("Sorry, I dont know how to work with negative numbers.");

    num = string_minus(num, o.num);
    return *this;
}

natural natural::operator-(const natural o) const {
    if (*this < o)
        throw std::out_of_range("Sorry, I dont know how to work with negative numbers.");

    return natural(string_minus(num, o.num));
}

std::string natural::get_current(std::string v, size_t s) const {
    if (v.size() < s) return v;

    std::string res;

    for (size_t i = 0; i < s; i++) {
        res.push_back(v[i]);
    }

    return res;
}

char natural::string_minus_cycle(std::string &f, const std::string &s) const {
    if (f.size() < s.size()) return '0';

    std::string res;
    int counter = 0;

    while (natural(f) >= natural(s)) {
        f = string_minus(f, s);
        counter++;
    }

    return int_to_char(counter);
}

std::string natural::string_division(const std::string &f, const std::string &s, bool reminder) const {
    std::string result;

    std::string current = get_current(f, s.size());
    auto position = current.size();
    while (true) {
        result.push_back(string_minus_cycle(current, s));
        if (position >= f.size())
            break;
        if (current == "0")
            current = "";
        current.push_back(f[position]);
        position++;
    }
    if (reminder)
        return current;

    std::reverse(result.begin(), result.end());
    while (result[result.size() - 1] == '0') {
        if (result.empty())
            break;
        result.pop_back();
    }

    if (result.empty())
        return "0";
    std::reverse(result.begin(), result.end());
    return result;
}

natural &natural::operator/=(const natural &o) {
    if (o == natural(0))
        throw std::out_of_range("Sorry, I dont know how to divide with zero.");

    num = string_division(num, o.num, false);
    return *this;
}

natural natural::operator/(const natural o) const {
    if (o == natural(0))
        throw std::out_of_range("Sorry, I dont know how to divide with zero.");

    return string_division(num, o.num, false);
}

natural &natural::operator%=(const natural &o) {
    if (o == natural(0))
        throw std::out_of_range("Sorry, I dont know how to divide with zero.");

    num = string_division(num, o.num, true);
    return *this;
}

natural natural::operator%(const natural o) const {
    if (o == natural(0))
        throw std::out_of_range("Sorry, I dont know how to divide with zero.");

    return string_division(num, o.num, true);
}

bool natural::operator<(const natural &o) const {
    if (num.size() < o.num.size()) return true;
    if (num.size() > o.num.size()) return false;

    size_t i = 0;
    for (auto it = num.begin(); it < num.end(); it++, i++) {
        if (char_to_int(num[i]) < char_to_int(o.num[i]))
            return true;
        if (char_to_int(num[i]) > char_to_int(o.num[i]))
            return false;
    }

    return false;
}

bool natural::operator>(const natural &o) const {
    if (num.size() > o.num.size()) return true;
    if (num.size() < o.num.size()) return false;

    size_t i = 0;
    for (auto it = num.begin(); it < num.end(); it++, i++) {
        if (char_to_int(num[i]) > char_to_int(o.num[i]))
            return true;
        if (char_to_int(num[i]) < char_to_int(o.num[i]))
            return false;
    }

    return false;
}

bool natural::operator>(int o) const {
    return *this > natural(o);
}

bool natural::operator==(const natural &o) const {
    if (num.size() > o.num.size() ||
        num.size() < o.num.size())
        return false;

    size_t i = num.size() - 1;
    for (auto it = num.rbegin(); it < num.rend(); it++, i--) {
        if (char_to_int(num[i]) != char_to_int(o.num[i]))
            return false;
    }

    return true;
}

bool natural::operator!=(const natural &o) const {
    return !(*this == o);
}

bool natural::operator<=(const natural &o) const {
    return *this == o || *this < o;
}


bool natural::operator>=(const natural &o) const {
    return *this == o || *this > o;
}

std::string natural::to_decimal(const std::string &n) const {
    std::string res = "0";
    std::string counter = "1";

    for (auto i = n.rbegin(); i != n.rend(); i++) {
        if (*i == '1') {
            if (res.size() >= counter.size())
                res = string_sum(res, counter);
            else
                res = string_sum(counter, res);
        }

        counter = string_multiple(counter, "2");
    }

    return res;
}

std::string natural::string_transfer_to_binary(const std::string &n) const {
    std::string res;
    std:: string first = n;

    while (natural(first) > natural(0)) {
        std::string rem = string_division(first, "2", true);
        first = string_division(first, "2", false);
        res.push_back(rem[0]);
    }

    std::reverse(res.begin(), res.end());
    return res;
}

natural natural::bit_and(const std::string &first, const std::string &second) const {
    std::string result;

    auto f_pos = first.rbegin();
    auto s_pos = second.rbegin();
    for (; f_pos != first.rend(); f_pos++) {
        char s = '0';
        if (s_pos != second.rend()) {
            s = *s_pos;
            s_pos++;
        }
        if (*f_pos == '1' && s == '1')
            result.push_back('1');
        else
            result.push_back('0');
    }

    std::reverse(result.begin(), result.end());
    result = to_decimal(result);
    return natural{result};
}

natural natural::operator&(const natural &o) const {
    std::string f = string_transfer_to_binary(num);
    std::string s = string_transfer_to_binary(o.num);

    if (f.size() > s.size()) return bit_and(f, s);
    return bit_and(s, f);
}

natural natural::bit_or(const std::string &first, const std::string &second) const {
    std::string result;

    auto f_pos = first.rbegin();
    auto s_pos = second.rbegin();
    for (; f_pos != first.rend(); f_pos++) {
        char s = '0';
        if (s_pos != second.rend()) {
            s = *s_pos;
            s_pos++;
        }
        if (*f_pos == '1' || s == '1')
            result.push_back('1');
        else
            result.push_back('0');
    }

    std::reverse(result.begin(), result.end());
    result = to_decimal(result);
    return natural{result};
}

natural natural::operator|(const natural &o) const {
    std::string f = string_transfer_to_binary(num);
    std::string s = string_transfer_to_binary(o.num);

    if (f.size() > s.size()) return bit_or(f, s);
    return bit_or(s, f);
}

natural natural::bit_xor(const std::string &first, const std::string &second) const {
    std::string result;

    auto f_pos = first.rbegin();
    auto s_pos = second.rbegin();
    for (; f_pos != first.rend(); f_pos++) {
        char s = '0';
        if (s_pos != second.rend()) {
            s = *s_pos;
            s_pos++;
        }
        if (*f_pos == s)
            result.push_back('0');
        else
            result.push_back('1');
    }

    std::reverse(result.begin(), result.end());
    result = to_decimal(result);
    return natural{result};
}

natural natural::operator^(const natural &o) const {
    std::string f = string_transfer_to_binary(num);
    std::string s = string_transfer_to_binary(o.num);

    if (f.size() > s.size()) return bit_xor(f, s);
    return bit_xor(s, f);
}




int main() {
	return 0;
}