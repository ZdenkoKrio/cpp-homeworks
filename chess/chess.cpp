#include "chess.hpp"

/* You can put implementations of non-template methods and functions
 * into this file, as you see fit. Do not put anything after the
 * ‹main› function, and make sure its prototype remains untouched.
 * Feel free to add testing code to ‹main›. */


#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <iostream>

void chess::give_on_position(position pos, player p, piece_type figure) {
	board[pos.file - 1][pos.rank - 1].is_empty = false;
	board[pos.file - 1][pos.rank - 1].owner = p;
	board[pos.file - 1][pos.rank - 1].piece = figure;
}

void chess::inicialize_player_pieces(player p) {
	int row = 2;
	if(p == player::black) row = 7;

	for(int i = 1; i < 9; i++)
		give_on_position(position{i, row}, p, piece_type::pawn);
			
	int last = 1;
	if(p == player::black) 
		last = 8;
		
	give_on_position(position{1, last}, p, piece_type::rook);
	give_on_position(position{2, last}, p, piece_type::knight);
	give_on_position(position{3, last}, p, piece_type::bishop);
	give_on_position(position{4, last}, p, piece_type::queen);
	give_on_position(position{5, last}, p, piece_type::king);
	give_on_position(position{6, last}, p, piece_type::bishop);
	give_on_position(position{7, last}, p, piece_type::knight);
	give_on_position(position{8, last}, p, piece_type::rook);
}

void chess::move(position from, position to) {
	board[to.file - 1][to.rank - 1].is_empty = false;
	board[to.file - 1][to.rank - 1].owner = at(from).owner;
	board[to.file - 1][to.rank - 1].piece = at(from).piece;

	board[from.file - 1][from.rank - 1].is_empty = true;
}

bool chess::is_pawns_first_move(position from, position to) const { 
	return from.file == to.file &&
		((from.rank == 2 && to.rank == 4 && at(from).owner == player::white) ||
		(from.rank == 7 && to.rank == 5 && at(from).owner == player::black));
}

bool chess::is_pawns_move(position from, position to) const {
	return from.file == to.file &&
		((from.rank == to.rank - 1 && at(from).owner == player::white) ||
		(from.rank == to.rank + 1 && at(from).owner == player::black));
}

bool chess::is_pawns_capture(position from, position to) const {
	return (from.file == to.file - 1 || from.file == to.file + 1) &&
			!at(to).is_empty && at(from).owner != at(to).owner &&
			((from.rank == to.rank - 1 && at(from).owner == player::white) ||
			 (from.rank == to.rank + 1 && at(from).owner == player::black));
}

bool chess::is_enpasant(position from, position to) const {
	return (at(from).owner == player::black &&
		 at(position{to.file, to.rank + 1}).owner == player::white && 
		 at(position{to.file, to.rank + 1}).piece == piece_type::pawn &&
			 from.rank == 4 && to.rank == 3 &&
			(from.file == to.file - 1 || from.file == to.file + 1)) ||

			(at(from).owner == player::white && 
			at(position{to.file, to.rank - 1}).owner == player::black && 
			at(position{to.file, to.rank - 1}).piece == piece_type::pawn &&
			from.rank == 5 && to.rank == 6 &&
			(from.file == to.file - 1 || from.file == to.file + 1));
}

bool chess::is_legal_move(position from, position to) const {
	if(!is_on_board(to.file) || !is_on_board(to.rank) ||
		!is_on_board(from.file) || !is_on_board(from.rank))
		return false; //po odmazani podmienky zahadne chovanie
		
	if(at(from).is_empty)
		return false;
	
	if(to.file == from.file && to.rank == from.rank)
		return false;

	switch (at(from).piece) {
		case piece_type::pawn: 
			return (is_pawns_first_move(from, to) || is_pawns_move(from, to) ||
					is_pawns_capture(from, to) || is_enpasant(from, to));
			
		case piece_type::rook:
			return ((from.rank == to.rank) || (from.file == to.file));
		
		case piece_type::knight:
			return ((((from.file == to.file - 2) || (from.file == to.file + 2)) && ((from.rank == to.rank - 1) || (from.rank == to.rank + 1))) ||
			 		(((from.rank == to.rank - 2) || (from.rank == to.rank + 2)) && ((from.file == to.file - 1) || (from.file == to.file + 1))));
		
		case piece_type::bishop:
			return std::abs(from.rank - to.rank) - std::abs(from.file - to.file) == 0;
		
		case piece_type::queen:
			return ((from.rank == to.rank) || (from.file == to.file)) ||
					(std::abs(from.rank - to.rank) - std::abs(from.file - to.file) == 0);

		case piece_type::king:
			return ((from.rank == to.rank - 1 || from.rank == to.rank || from.rank == to.rank + 1) &&
					(from.file == to.file - 1 || from.file == to.file || from.file == to.file + 1)) ||
					is_casteling(from, to);
	}
	return false;
}

bool chess::is_free_rank(position from, position to) const {
	for (int x = from.rank + 1; x < to.rank; x++)
		if (!at(position{from.file, x}).is_empty)
			return false;
	return true;
}

bool chess::is_free_file(position from, position to) const {
	for (int x = from.file + 1; x < to.file; x++)
		if (!at(position{x, from.rank}).is_empty)
			return false;
	return true;
}

bool chess::free_lines(position from, position to) const {
	return (is_free_rank(from, to) && is_free_rank(to, from) &&
			 is_free_file(from, to) && is_free_file(to, from));		
}

bool chess::is_in_move_borders(int from, int to, int value) const {
	return (from > value && value > to) || (from < value && value < to);
}

bool chess::free_diagonals(position from, position to) const { 
	std::vector<int> directions{-1, +1};
	for(const int& x_correction:directions) 
		for(const int& y_correction:directions)
			for (int x = from.rank + x_correction, y = from.file + y_correction;
				is_in_move_borders(from.rank, to.rank, x) && is_in_move_borders(from.file, to.file, y);
				x += x_correction, y += y_correction)
				if (!at(position{y, x}).is_empty) 
					return false;
	return true;
}

bool chess::is_possible_move(position from, position to) const {
	if(!at(to).is_empty && at(to).owner == at(from).owner) 
		return false;
		
	switch (at(from).piece) {
		case piece_type::pawn: 
			if(is_pawns_first_move(from, to)) 
				return ((at(from).owner == player::black && at(position{to.file, 6}).is_empty) || 
						(at(from).owner == player::white && at(position{to.file, 3}).is_empty)) && at(to).is_empty;
			if(is_pawns_move(from, to))
				return at(to).is_empty;
			return true;				

		case piece_type::queen:
			if(to.file == from.file || to.rank == from.rank) 
				return free_lines(from, to);
			return free_diagonals(from, to); 
			
		case piece_type::bishop:
			return free_diagonals(from, to);
			
		case piece_type::rook:
			return free_lines(from, to);

		case piece_type::king:
			if(is_casteling(from, to)) {
				if (actual_player == player::black)
					return (to.file == 7 && at(position{7, 8}).is_empty && at(position{6, 8}).is_empty) ||
							(to.file == 3 && at(position{2, 8}).is_empty && at(position{3, 8}).is_empty && at(position{4, 8}).is_empty);
				return (to.file == 7 && at(position{7, 1}).is_empty && at(position{6, 1}).is_empty) ||
						(to.file == 3 && at(position{2, 1}).is_empty && at(position{3, 1}).is_empty && at(position{4, 1}).is_empty);
			}
			break;

		default:
			return true;
	}
	return true;
}


bool chess::is_casteling(position from, position to) const {
	return (from.file == 5 &&
			((at(from).owner == player::white && to.rank == 1 && from.rank == 1 &&
				((to.file == 3 && 
					at(position{1, 1}).owner == player::white && 
					at(position{1, 1}).piece == piece_type::rook &&
					!at(position{1, 1}).is_empty) 
				|| (to.file == 7 &&
					at(position{8, 1}).owner == player::white && 
					at(position{8, 1}).piece == piece_type::rook &&
					!at(position{8, 1}).is_empty))) ||
			(at(from).owner == player::black && to.rank == 8 && from.rank == 8 &&
			((to.file == 3 && 
					at(position{1, 8}).owner == player::black && 
					at(position{1, 8}).piece == piece_type::rook &&
					!at(position{1, 8}).is_empty) 
				|| (to.file == 7 &&
					at(position{8, 8}).owner == player::black && 
					at(position{8, 8}).piece == piece_type::rook &&
					!at(position{8, 8}).is_empty))))
			);
}

bool chess::is_casteling_possible(position to) const {
	if (actual_player == player::black)
		return (b_unused_king && ((to.file == 3 && b_left_rook) || (to.file == 7 && b_right_rook)));
	return (w_unused_king && ((to.file == 3 && w_left_rook) || (to.file == 7 && w_right_rook)));
}

bool chess::is_escape_from_check(position from, position to) { 
	position king = b_king;
	if (actual_player == player::white)
		king = w_king;

	if (is_position_in_check(king)) {
			return !would_check(from, to);
	}

	return true;
}

bool chess::would_check(position from, position to) {
	bool result = false;
	bool enpasant = false;
	position enpasant_position{};
	
	position king = b_king;
	if (actual_player == player::white) {
		king = w_king;
	}
	if(at(from).piece == piece_type::king) {
		king = to;
	}

	occupant o{};
	o.is_empty = true;
	if(!at(to).is_empty) {
		o.is_empty = false;
		o.owner = at(to).owner;
		o.piece = at(to).piece;
	} else if(at(from).piece == piece_type::pawn && is_enpasant(from, to)) {
		enpasant = true;
		o.is_empty = false;
		o.piece = piece_type::pawn;
		if(at(from).owner == player::white) {
			o.owner = player::black;
			board[to.file - 1][to.rank - 2].is_empty = true;
			enpasant_position.file = to.file;
			enpasant_position.rank = to.rank - 1;		
		} else {
			o.owner = player::white;
			board[to.file - 1][to.rank].is_empty = true;
			enpasant_position.file = to.file;
			enpasant_position.rank = to.rank + 1;					
		}
	}
	move(from, to);
	
	if (is_position_in_check(king))
		result = true;
	
	move(to, from);
	if(!o.is_empty) {
		if(enpasant) {
			give_on_position(enpasant_position, o.owner, o.piece);
		} else {
			give_on_position(to, o.owner, o.piece); 
		}
	}
	return result;
}

bool chess::is_on_board(int value) const {
	return value > 0 && value < 9;
}


bool chess::is_position_in_check(position king) const {
	int x = king.rank;
	int y = king.file;

	std::vector<int> directions{-1, 0, 1};
	for(const int& x_correction:directions) 
		for(const int& y_correction:directions) {
			if(x_correction == 0 && y_correction == 0) continue;
			
			x += x_correction;
			y += y_correction;	
			while (is_on_board(y) && is_on_board(x) && at(position{y, x}).is_empty) { 
				x += x_correction; 
				y += y_correction; 
			}
			position temp{y, x};
			x = king.rank;
			y = king.file;
			if(is_legal_move(temp , king) && at(temp).owner != actual_player) //legal move kontroluje aj ci pozicia existuje
				if(is_possible_move(temp, king)) { 
						return true;
				}
		}

	std::vector<position> posible_horses = {position{king.file + 1, king.rank + 2},
											position{king.file + 1, king.rank - 2}, 
											position{king.file - 1, king.rank + 2}, 
											position{king.file - 1, king.rank - 2}, 
											position{king.file + 2, king.rank + 1}, 
											position{king.file - 2, king.rank + 1}, 
											position{king.file + 2, king.rank - 1}, 
											position{king.file - 2, king.rank - 1}};
	for(position p:posible_horses)
		if(is_legal_move(p, king) && at(p).owner != actual_player) return true;

	return false;
}

bool chess::are_positions_in_check(position from, position to) const {
	if(to.file == 3)
		return is_position_in_check(position{3, from.rank}) || is_position_in_check(position{4, from.rank});
		
	if(to.file == 7)
		return is_position_in_check(position{6, from.rank}) || is_position_in_check(position{7, from.rank});
	
	return false;
}

bool chess::is_promotion(position from, position to) const {
	return ((at(from).owner == player::black && to.rank == 1) || 
			(at(from).owner == player::white && to.rank == 8));
}

bool chess::is_correct_promotion(piece_type p) const {
	return p == piece_type::rook || p == piece_type::knight || p == piece_type::bishop || p == piece_type::queen;
}

void chess::change_player() {
	if (actual_player == player::black) {
		actual_player = player::white;
	} else {
		actual_player = player::black;
	}
}

void chess::change_flags(position from) {
	if (from.file == 1 && from.rank == 1) w_left_rook = false;
	if (from.file == 1 && from.rank == 8) b_left_rook = false;
	if (from.file == 8 && from.rank == 1) w_right_rook = false;
	if (from.file == 8 && from.rank == 8) b_right_rook = false;
		
	if (from.file == 5 && from.rank == 8) b_unused_king = false;
	if (from.file == 5 && from.rank == 1) w_unused_king = false;
}

void chess::king_position(position from, position to) {
	if (at(from).piece == piece_type::king) {
		if (at(from).owner == player::black) b_king = to;
		if (at(from).owner == player::white) w_king = to;
	}
}
	
chess::chess() {
	occupant point = occupant();
	point.is_empty = true;
	point.owner = player::black;
	point.piece = piece_type::pawn;
		
	for(auto & r : board)
		for(auto & c : r)
			c = point;

	inicialize_player_pieces(player::white);
	inicialize_player_pieces(player::black);
		
	actual_player = player::white;
	w_unused_king = true;
	b_unused_king = true;
	w_left_rook = true;
	b_left_rook = true;
	w_right_rook = true;
	b_right_rook = true;

	b_king = position{5, 8};
	w_king = position{5, 1};		
}

result chess::play(position from, position to, piece_type promote) {
		if((!is_on_board(from.file) || !is_on_board(from.rank)) || at(from).is_empty) {
			return result::no_piece;
		}

		if (at(from).owner != actual_player) {
			return result::bad_piece;	
		}		
		
		if(!is_legal_move(from, to)) {
			return result::bad_move;
		}

		if(!is_possible_move(from, to)) {
			return result::blocked;
		}

		if(at(from).piece == piece_type::pawn && is_enpasant(from, to) && 
			at(to).is_empty &&
			(!(((last_move_from.rank == 7 && last_move_to.rank == 5) || 
				(last_move_from.rank == 2 && last_move_to.rank == 4)) && (to.file == last_move_to.file)))) {			
			return result::lapsed;
		}

		if(at(from).piece == piece_type::king && is_casteling(from, to)) {
			if(!is_casteling_possible(to)) {
				return result::has_moved;	
			}
			
			if(is_position_in_check(from)) {
				return result::in_check;
			}
	
			if(are_positions_in_check(from, to)) {
				return result::would_check;
			}
			
			change_player();
			change_flags(from);
			move(from, to);

			int rank = 1;
			if(at(from).owner == player::black) {
				b_king = to;
				rank = 8;
			} else {
				w_king = to;
			}
				
			if(to.file == 3) move(position{1, rank}, position{4, rank});
			if(to.file == 7) move(position{8, rank}, position{6, rank});

			last_move_to = to;
			last_move_from = from;
			return result::ok;
		}

		if(!is_escape_from_check(from, to)) {
			return result::in_check;
		}
		
		if(would_check(from, to)) {
			return result::would_check;
			}

		if(at(from).piece == piece_type::pawn && is_promotion(from, to)) {
			if(!is_correct_promotion(promote)) {
				return result::bad_promote;
			}
			give_on_position(from, at(from).owner, promote);
		}
	
		if (!at(to).is_empty) {
			change_flags(from);
			king_position(from, to);
			move(from, to);
			last_move_to = to;
			last_move_from = from;
			change_player();
			return result::capture;
		}

		if(at(from).piece == piece_type::pawn && is_enpasant(from, to)) {
			if(at(from).owner == player::white) {
				board[to.file - 1][to.rank - 2].is_empty = true;
			} else {
				board[to.file - 1][to.rank].is_empty = true;				
			}
			move(from, to);
			last_move_to = to;
			last_move_from = from;
			change_player();
			return result::capture;
		}
		
		change_flags(from);
		king_position(from, to);
		
		move(from, to);
		last_move_to = to;
		last_move_from = from;
		change_player();
		return result::ok; 
}

occupant chess::at(position pos) const {
	return board[pos.file - 1][pos.rank - 1];
}





int main()
{
    /* tests go here */
    return 0;
}
