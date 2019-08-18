//
// Created by Alon on 15/06/2019.
//

#ifndef PART2_BOARDCELL_H
#define PART2_BOARDCELL_H

#include "CellType.h"
#include "Direction.h"

template <CellType T, Direction D, int L>
struct BoardCell {
    constexpr static CellType type = T;
    constexpr static Direction direction = D;
    constexpr static int length = L;
};

#endif //PART2_BOARDCELL_H

