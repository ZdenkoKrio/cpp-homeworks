/* The goal of this task is to implement the standard rules of
 * chess. */

struct position
{
    int file; /* column 'letter', a = 1, b = 2, ... */
    int rank; /* row number, starting at 1 */
};

enum class piece_type
{
    pawn, rook, knight, bishop, queen, king
};

enum class player { white, black };

/* The following are the possible outcomes of ‹play›. The outcomes
 * are shown in the order of precedence, i.e. the first applicable
 * is returned.
 *
 * ├┄┄┄┄┄┄┄┄┄┄┄┄┄┄▻┼◅┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┄┤
 * │ ‹capture›     │ the move was legal and resulted in a capture │
 * │ ‹ok›          │ the move was legal and was performed         │
 * │ ‹no_piece›    │ there is no piece on the ‹from› square       │
 * │ ‹bad_piece›   │ the piece on ‹from› is not ours              │
 * │ ‹bad_move›    │ this move is not available for this piece    │
 * │ ‹blocked›     │ another piece is in the way                  │
 * │ ‹lapsed›      │ «en passant» capture is no longer allowed    │
 * │ ‹has_moved›   │ one of the castling pieces has already moved │
 * │ ‹in_check›    │ the player is currently in check and the     │
 * │               │ move does not get them out of it             │
 * │ ‹would_check› │ the move would place the player in check     │
 * │ ‹bad_promote› │ promotion to a pawn or king was attempted    │
 *
 * Attempting an «en passant» when the pieces are in the wrong place
 * is a ‹bad_move›. In addition to ‹has_moved›, (otherwise legal)
 * castling may give:
 *
 *  • ‹blocked› – some pieces are in the way,
 *  • ‹in_check› – the king is currently in check,
 *  • ‹would_check› – would pass through or end up in check. */

enum class result
{
    capture, ok, no_piece, bad_piece, bad_move, blocked, lapsed,
    in_check, would_check, has_moved, bad_promote
};

struct occupant
{
    bool is_empty;
    player owner;
    piece_type piece;
};

class chess
{
	occupant board [8][8];
	player actual_player;
	position last_move_to;
	position last_move_from;
	bool b_right_rook;
	bool b_left_rook;
	bool b_unused_king;
	bool w_right_rook;
	bool w_left_rook;
	bool w_unused_king;
	position b_king;
	position w_king;


	void give_on_position(position pos, player p, piece_type figure);
	void inicialize_player_pieces(player p);
	void move(position from, position to);
	bool is_legal_move(position from, position to) const;
	bool free_lines(position from, position to) const;
	bool free_diagonals(position from, position to) const;
	bool is_possible_move(position from, position to) const;
	bool is_enpasant(position from, position to) const;
	bool is_casteling(position from, position to) const;
	bool is_casteling_possible(position to) const;
	bool is_escape_from_check(position from, position to);
	bool would_check(position from, position to);
	bool is_position_in_check(position king) const;
	bool are_positions_in_check(position from, position to) const;
	bool is_promotion(position from, position to) const;
	bool is_correct_promotion(piece_type p) const;
	void change_player();
	void change_flags(position from);
	void king_position(position from, position to);

	bool is_free_rank(position from, position to) const;
	bool is_free_file(position from, position to) const;
	bool is_in_move_borders(int from, int to, int value) const;
	bool is_on_board(int value) const;
	bool is_pawns_first_move(position from, position to) const;
	bool is_pawns_move(position from, position to) const;
	bool is_pawns_capture(position from, position to) const;

public:

    /* Construct a game of chess in its default starting position.
     * The first call to ‹play› after construction moves a piece of
     * the white player. */

    chess();

    /* Move a piece currently at ‹from› to square ‹to›:
     *
     *  • in case the move places a pawn at its 8th rank (rank 8 for
     *    white, rank 1 for black), it is promoted to the piece given
     *    in ‹promote› (otherwise, the last argument is ignored),
     *  • castling is described as a king move of more than one square,
     *  • if the result is an error (not ‹capture› nor ‹ok›), calling
     *    ‹play› again will attempt another move by the same player. */

    result play( position from, position to,
                 piece_type promote = piece_type::pawn );

    /* Which piece is at the given position? */

    occupant at( position ) const;
};
