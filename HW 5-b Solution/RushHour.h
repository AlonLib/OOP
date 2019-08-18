//
// Created by Alon on 15/06/2019.
//

#ifndef PART2_RUSHHOUR_H
#define PART2_RUSHHOUR_H

#include "List.h"
#include "Utilities.h"
#include "CellType.h"
#include "Direction.h"
#include "BoardCell.h"
#include "GameBoard.h"
#include "MoveVehicle.h"

template<typename G, int R, int C, CellType Car>
struct FindCar {
    typedef typename GetAtIndex<C,
            typename GetAtIndex<R, typename G::board>::value>::value board_cell;
    constexpr static bool found_X = board_cell::type == Car;
    typedef typename ConditionalAndValue<found_X, PlaceOnBoard<R, C>,
            FindCar<G, R, C - 1, Car>>::value value;
};

template<typename G, int R, CellType Car>
struct FindCar<G, R, -1, Car> {
    typedef typename FindCar<G, R - 1, G::width - 1, Car>::value value;
};

template<typename G, int C, CellType Car>
struct FindCar<G, -1, C, Car> {
    static_assert(Car > 17, "ERROR08"); // shouldn't get here
};

template<typename G, int R, int C>
struct CheckIfXCanGoRight {
    constexpr static CellType type = GetAtIndex<C,
            typename GetAtIndex<R, typename G::board>::value>::value::type;
    constexpr static bool current_place_ok = (type == X || type == EMPTY);
    constexpr static bool reached_right_end = C + 1 >= G::width;

    typedef typename
    ConditionalAndValue<
            current_place_ok && reached_right_end, ValueWrapperBool<true>,
            ConditionalAndValue<!current_place_ok, ValueWrapperBool<false>,
                    CheckIfXCanGoRight<G, R, C + 1>
            >
    >::value value;
};

template<typename G>
struct CheckWin {
    typedef typename FindCar<G, G::length - 1, G::width - 1, X>
    ::value x_location;

    typedef typename CheckIfXCanGoRight<G, x_location::row, x_location::col>
    ::value value;
    constexpr static bool result = value::result;
};

template<typename G, typename L>
struct CheckSolution;
// this is here because SedMove calls CheckSolution which calls SendMove...

template<typename G, typename L>
struct SendMove {
    typedef typename L::head move;
    typedef typename FindCar<G,
            G::length - 1, G::width - 1, move::type>::value car_location;
    typedef typename MoveVehicle<G, car_location::row, car_location::col,
            move::direction, move::amount>::board new_board;
    typedef typename CheckSolution<new_board,
            typename L::next>::result_wrapper value;
};


template<typename G, typename L>
struct CheckSolution {
    typedef typename ConditionalAndValue<
            L::size >= 1, SendMove<G, L>, CheckWin<G>>::value result_wrapper;
    constexpr static bool result = result_wrapper::result;
};

#endif //PART2_RUSHHOUR_H
