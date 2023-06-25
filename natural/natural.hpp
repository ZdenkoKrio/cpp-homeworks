/* In this task, you will implement a class which represents
 2  * arbitrary-size natural numbers (including 0). In addition to the
 3  * methods prescribed below, the class must support the following:
 4  *
 5  *  • arithmetic operators ‹+›, ‹-›, ‹*›, ‹/› and ‹%› (the last two
 6  *    implementing division and reminder),
 7  *  • all relational operators,
 8  *  • bitwise operators ‹^› (xor), ‹&› (and) and ‹|› (or).
 9  *
10  * The usual preconditions apply (divisors are not 0, the second
11  * operand of subtraction is not greater than the first).
12  */
#include <vector>
#include <bitset>
#include <cstddef>

class natural
{
    std::string num;

    char int_to_char(int i) const;
    int char_to_int(char c) const;

    std::string& get_longer(const std::string &first, const std::string &second) const;
    std::string string_sum(const std::string &first, const std::string &second) const;
    std::string string_multiple(const std::string &first, const std::string &second) const;
    std::string string_minus(const std::string &first, const std::string &second) const;
    char string_minus_cycle(std::string &f, const std::string &s) const;
    std::string string_division(const std::string &first, const std::string &second, bool reminder) const;

    natural base_transfer(const natural &base, bool sum) const;
    std::string string_transfer_to_binary(const std::string &n) const;
    std::string to_decimal(const std::string &n) const;
    natural bit_and(const std::string &f, const std::string &s) const;
    natural bit_or(const std::string &f, const std::string &s) const;
    natural bit_xor(const std::string &f, const std::string &s) const;

    std::string get_current(std::string v, size_t s) const;
    
public:
    explicit natural( int v = 0 );
    natural(std::string n);

    natural power(const natural &exponent) const;
    natural power(int exponent) const;
    natural digit_count(const natural &base) const;
    natural digit_sum(const natural &base) const;

    natural operator+(natural o) const;
    natural& operator+=(const natural &o);
    natural operator-(natural o) const;
    natural& operator-=(const natural &o);
    natural operator*(const natural &o) const;
    natural operator*(int o) const;
    natural& operator*=(const natural &o);
    natural operator/(natural o) const;
    natural& operator/=(const natural &o);
    natural operator%(natural o) const;
    natural& operator%=(const natural &o);

    bool operator<(const natural& o) const;
    bool operator>(const natural& o) const;
    bool operator>(int o) const;
    bool operator<=(const natural& o) const;
    bool operator>=(const natural& o) const;
    bool operator==(const natural& o) const;
    bool operator!=(const natural& o) const;

    //bitwise operators
    natural operator^(const natural &o) const;
    natural operator&(const natural &o) const;
    natural operator|(const natural &o) const;
};