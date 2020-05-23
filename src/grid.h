#pragma once

#define CELLS_LEN 81

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
