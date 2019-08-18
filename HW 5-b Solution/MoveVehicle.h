//
// Created by Alon on 18/06/2019.
//

#ifndef PART2_WORK_MOVEVEHICLE_H
#define PART2_WORK_MOVEVEHICLE_H

#include "Direction.h"
#include "CellType.h"
#include "GameBoard.h"
#include "Utilities.h"

template<CellType T, Direction D, int A>
struct Move {
    static_assert(T != EMPTY, "cell type for move is empty");
    constexpr static CellType type = T;
    constexpr static Direction direction = D;
    constexpr static int amount = A;
};

template<int R, int C>
struct PlaceOnBoard {
    constexpr static int row = R;
    constexpr static int col = C;
    typedef PlaceOnBoard<row, col> value;
};

template<typename G, int R, int C, Direction D>
struct GetFront {
    constexpr static int row = R + (D == DOWN) - (D == UP);
    constexpr static int col = C + (D == RIGHT) - (D == LEFT);
    static_assert(row >= 0 && col >= 0, "ERROR02");
    static_assert(row < G::length && col < G::width, "ERROR03");

    typedef typename GetAtIndex<C,
            typename GetAtIndex<R,
                    typename G::board>::value>::value board_old_cell;
    // assuming that the requested play cannot escape the boundaries of the board
    typedef typename GetAtIndex<col,
            typename GetAtIndex<row,
                    typename G::board>::value>::value board_new_cell;
    static_assert(board_old_cell::type == board_new_cell::type ||
                  board_new_cell::type == EMPTY, "cannot move that direction");

    constexpr static bool found_front = board_new_cell::type == EMPTY;
    typedef PlaceOnBoard<row, col> temp_value;
    // returns the first empty cell
    typedef typename ConditionalAndValue<found_front, temp_value,
            GetFront<G, row, col, D>>::value value;
};

template<typename G, int R, int C, Direction D>
struct GetBack {
    typedef typename GetAtIndex<C,
            typename GetAtIndex<R,
                    typename G::board>::value>::value board_old_cell;
    constexpr static int row_before_fix = R - (D == DOWN) + (D == UP);
    constexpr static int col_before_fix = C - (D == RIGHT) + (D == LEFT);
    constexpr static int row =
            row_before_fix < 0 || row_before_fix >= G::length ?
            R : row_before_fix;
    constexpr static int col =
            col_before_fix < 0 || col_before_fix >= G::width ?
            C : col_before_fix;
    static_assert(row >= 0 && col >= 0, "ERROR04");
    static_assert(row < G::length && col < G::width, "ERROR05");
    typedef typename GetAtIndex<col,
            typename GetAtIndex<row,
                    typename G::board>::value>::value board_new_cell;

    constexpr static bool found_back =
            (board_new_cell::type != board_old_cell::type) ||
            (row == R && col == C);
    typedef PlaceOnBoard<R, C> temp_value;
    // returns the last car cell (before empty or other car)
    typedef typename ConditionalAndValue<found_back, temp_value,
            GetBack<G, row, col, D>>::value value;
};

// case last arg is default => cleaning cell. else: filling cell.
template<typename G, typename PlaceOnBoard, typename BoardCell = BoardCell<EMPTY, UP, 0>>
struct FixSpecificCell {
    constexpr static int row = PlaceOnBoard::row;
    constexpr static int col = PlaceOnBoard::col;
    typedef typename GetAtIndex<row, typename G::board>::value line_to_fix;
    typedef typename SetAtIndex<col, BoardCell, line_to_fix>::list line_fixed;
    typedef typename SetAtIndex<row, line_fixed,
            typename G::board>::list ready_lists;
    typedef GameBoard<ready_lists> board;
    // typedef typename GameBoard<ready_lists>::board board_cell;
};

template<typename G, typename PlaceOnBoard, Direction D>
struct OneMoveOnBoard {
    constexpr static int row = PlaceOnBoard::row;
    constexpr static int col = PlaceOnBoard::col;
    typedef typename GetFront<G, row, col, D>::value front;
    typedef typename GetBack<G, row, col, D>::value back;
    typedef typename GetAtIndex<col,
            typename GetAtIndex<row,
                    typename G::board>::value>::value board_cell;
    // fill front:
    typedef typename FixSpecificCell<G, front, board_cell>::board temp_board;
    // clean back:
    typedef typename FixSpecificCell<temp_board, back>::board board;
};

template<typename G, int R, int C, Direction D, int A>
struct MoveAmountOnBoard {
    typedef typename GetFront<G, R, C, D>::value next_step;
    typedef PlaceOnBoard<R, C> current_step;
    typedef typename OneMoveOnBoard<G, current_step, D>::board new_board;
    typedef typename MoveAmountOnBoard<
            new_board, next_step::row, next_step::col, D, A - 1>::board board;
};
template<typename G, int R, int C, Direction D>
struct MoveAmountOnBoard<G, R, C, D, 0> {
    typedef G board;
};


template<typename G, int R, int C, Direction D, int A>
struct MoveVehicle {
    static_assert(R <= G::length, "length - out of board");
    static_assert(C <= G::width, "width - out of board");
    typedef typename GetAtIndex<C,
            typename GetAtIndex<R,
                    typename G::board>::value>::value board_cell;
    static_assert(board_cell::type != EMPTY, "sent board cell is empty");
    constexpr static bool legal_direction =
            (D == UP || D == DOWN) ?
            (board_cell::direction == UP || board_cell::direction == DOWN) :
            (board_cell::direction == LEFT || board_cell::direction == RIGHT);
    static_assert(legal_direction, "car cannot move at that direction");

    typedef typename MoveAmountOnBoard<G, R, C, D, A>::board board;
};

#endif //PART2_WORK_MOVEVEHICLE_H
