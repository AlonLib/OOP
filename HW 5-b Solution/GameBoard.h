//
// Created by Alon on 15/06/2019.
//

#ifndef PART2_GAMEBOARD_H
#define PART2_GAMEBOARD_H

#include "BoardCell.h"
#include "List.h"

template <typename List>
struct GameBoard {
    typedef List board;
    constexpr static int length = List::size;
    constexpr static int width = List::head::size;
};

#endif //PART2_GAMEBOARD_H
