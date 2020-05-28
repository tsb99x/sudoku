#include "grid.h"

#include <stdlib.h>
#include <string.h>

struct grid {
        char cells[CELLS_LEN];
};

grid_t *grid_create(
        void
) {
        grid_t *self;

        self = malloc(sizeof(grid_t));
        if (!self)
                return NULL;
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
        memcpy(self->cells, cells_initializer, CELLS_LEN * sizeof(char));
        return self;
}

void grid_destroy(
        grid_t *self
) {
        free(self);
}

static void swap_columns(
        grid_t *self,
        int from,
        int to
) {
        int i, t;

        for (i = 0; i < ROWS * COLS; i += COLS) {
                t = self->cells[i + from];
                self->cells[i + from] = self->cells[i + to];
                self->cells[i + to] = t;
        }
}

static void grid_shuffle(
        grid_t *self
) {
        int a, b;

        a = rand() % 3;
        do
                b = rand() % 3;
        while (b == a);
        swap_columns(self, a, b);
}

static void grid_erase(
        grid_t *self
) {
        int i;

        for (i = 0; i < ROWS * COLS; ++i)
                if (rand() % 2 > 0)
                        self->cells[i] = 0;
}

void grid_prepare(
        grid_t *self
) {
        grid_shuffle(self);
        grid_erase(self);
}

char grid_cell_at(
        grid_t *self,
        int x,
        int y
) {
        return self->cells[y * COLS + x];
}
