#pragma once

#define ROWS 9
#define COLS 9
#define CELLS_LEN ROWS * COLS

typedef struct grid {
        char cells[CELLS_LEN];
} grid_t;

grid_t grid_create(
        void
);

char grid_cell_at(
        int x,
        int y
);
