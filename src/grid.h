#pragma once

#define ROWS 9
#define COLS 9
#define CELLS_LEN ROWS * COLS

typedef struct grid grid_t;

grid_t *grid_create(
        void
);

void grid_destroy(
        grid_t *self
);

char grid_cell_at(
        grid_t *self,
        int x,
        int y
);

void grid_prepare(
        grid_t *self
);
