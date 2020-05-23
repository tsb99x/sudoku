#include "button.h"

#include <sdl.h>

static void assign_state(
        button_t *buttons,
        grid_t *grid
) {
        int x, y, pos;

        pos = 0;
        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        buttons[pos].x = x;
                        buttons[pos].y = y;
                        buttons[pos].val = grid->cells[pos];
                        buttons[pos].state = IDLE;
                        ++pos;
                }
}

button_t *buttons_create(
        grid_t *grid
) {
        button_t *buttons;

        buttons = malloc(CELLS_LEN * sizeof(button_t));
        if (!buttons) {
                SDL_Log("Failed to allocate memory for buttons\n");
                return NULL;
        }
        assign_state(buttons, grid);
        return buttons;
}

void buttons_destroy(
        button_t *buttons
) {
        free(buttons);
}
