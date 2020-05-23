#pragma once

#include "grid.h"

/* Main GUI Element, Button */

typedef enum state {
        IDLE,
        HOVERED,
        CLICKED
} state_t;

typedef struct button {
        int x, y, size, val;
        state_t state;
} button_t;

button_t *buttons_create(
        grid_t *grid
);

void buttons_destroy(
        button_t *buttons
);
