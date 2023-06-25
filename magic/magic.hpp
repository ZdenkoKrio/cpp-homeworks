/* A magic square is an ⟦n × n⟧ grid of natural numbers ⟦1⟧–⟦n²⟧,
 * such that all rows and columns and both diagonals add up to a
 * fixed ‘magic constant’ and each number appears exactly once.
 * Solving the square means filling in all empty cells in a manner
 * that gives the full square the magic property. The goal of this
 * task is to implement a simple backtracking solver for completing
 * partially filled magic squares. */

#include <vector>
#include <algorithm>
#include <tuple>

class magic
{
	std::vector<std::vector<int>> square;
	
	std::tuple<int, int> get_first_empty_position() const;
	
	int check_diagonal(int y_correction) const;
	
	bool check_lines(int magic_constant, bool horizontal) const;
	
	bool backtrack(std::vector<std::tuple<int, int>>& values);

	bool check_control_sum() const;

public:
    /* Construct an empty ⟦n × n⟧ square. */

    magic( int n );

    /* Get the value at the given position. A return value of 0
     * indicates an empty square. */

    int get( int x, int y ) const;

    /* Set a cell at the given position to a given value. The
     * behaviour is undefined if ‹v› is already present in the
     * square. If ‹v› is negative, the cell is empty, but must not
     * take ‹std::abs( v )› as its value in the solved square. */

    void set( int x, int y, int v );

    /* Solve the square: fill in all empty cells so that the square
     * has the magic property and return ‹true›. If the square
     * cannot be solved, do not change its content and return
     * ‹false›. */

    bool solve();
};
