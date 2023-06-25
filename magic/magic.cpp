#include <iostream>
#include "magic.hpp"
#include <cassert>
#include <cmath>

std::tuple<int, int> magic::get_first_empty_position() const {
    for(size_t row = 0; row < square.size(); row++)
        for(size_t col = 0; col < square.size(); col++)
            if(get(row, col) <= 0)
                return {row, col};
    return {-1, -1};
}

bool magic::backtrack(std::vector<std::tuple<int, int>>& values) {
    std::tuple<int, int> position = get_first_empty_position();

    if(std::get<0>(position) == -1)
        return check_control_sum();

    for(std::tuple<int, int> &v:values) {
        if(std::get<1>(v) == 1)
            continue;

        int pos_v = get(std::get<0>(position), std::get<1>(position));
        if(std::abs(pos_v) == std::get<0>(v)) {
            continue;
        }

        set(std::get<0>(position), std::get<1>(position), std::get<0>(v));
        std::get<1>(v) = 1;

        if(backtrack(values))
            return true;

        set(std::get<0>(position), std::get<1>(position), pos_v);
        std::get<1>(v) = -1;
    }
    return false;
}

int magic::check_diagonal(int y_correction) const {
    int result = 0;
    for(size_t i = 0; i < square.size(); i++)
        result += get(i, std::abs(static_cast<int> (y_correction - i)));
    return result;
}

bool magic::check_lines(int magic_constant, bool horizontal = true) const {
    for(size_t col = 0; col < square.size(); col++) {
        int col_sum = 0;
        for(size_t row = 0; row < square.size(); row++) {
            if(horizontal) {
                col_sum += get(row, col);
            } else {
                col_sum += get(col, row);
            }
        }
        if(col_sum != magic_constant)
            return false;
    }
    return true;
}

bool magic::check_control_sum() const {
    int magic_constant = check_diagonal(0);
    int diagonal_sum = check_diagonal(square.size() - 1);

    if(diagonal_sum != magic_constant)
        return false;

    return check_lines(magic_constant) && check_lines(magic_constant, false);
}

magic::magic(int n) {
    for(int i = 0; i < n; i++) {
        std::vector<int> vec(n, 0);
        square.push_back(vec);
    }
}

int magic::get(int x, int y) const {
    return square[x][y];
}

void magic::set(int x, int y, int v) {
    square[x][y] = v;
}

bool magic::solve() {
    std::vector<std::vector<int>> original = square;
    std::vector<std::tuple<int, int>> unused_values;

    std::vector<int> used_values;
    for(size_t row = 0; row < square.size(); row++) {
        for(size_t col = 0; col < square.size(); col++) {
            int value = get(row, col);
            //here   if(value != 0) used_values.push_back(value);
            if(value > 0) used_values.push_back(value);
        }
    }

    for(size_t key = 1; key <= (square.size() * square.size()); key++) {
        if (!(std::find(used_values.begin(), used_values.end(), key) != used_values.end())) {
            unused_values.emplace_back(std::make_tuple(key, -1));
        }
    }

    if(backtrack(unused_values))
        return true;

    square = original;
    return false;
}

int main() {
	return 0;
}