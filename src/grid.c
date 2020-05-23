#include "grid.h"

#include <string.h>

grid_t grid_create(
        void
) {
        grid_t grid;

        char cells_initializer[CELLS_LEN] = {
                1, 2, 3,  4, 5, 6,  7, 8, 9,
                4, 5, 6,  7, 8, 9,  1, 2, 3,
                7, 8, 9,  1, 2, 3,  4, 5, 6,

                3, 1, 2,  5, 6, 4,  9, 7, 8,
                5, 4, 7,  8, 9, 1,  3, 6, 2,
                6, 9, 8,  2, 3, 7,  5, 1, 4,

                2, 7, 5,  3, 4, 8,  6, 9, 1,
                9, 3, 1,  6, 7, 2,  8, 4, 5,
                8, 6, 4,  9, 1, 5,  2, 3, 7
        };
        memcpy(grid.cells, cells_initializer, CELLS_LEN * sizeof(char));
        return grid;
}
