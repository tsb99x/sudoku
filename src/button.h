#pragma once

#include "context.h"
#include "grid.h"

typedef struct button button_t;

button_t *buttons_create(
        grid_t *grid
);

void buttons_destroy(
        button_t *buttons
);

void buttons_draw(
        button_t *self,
        context_t *ctx,
        SDL_Texture **digits,
        SDL_Point mouse_pos
);

void buttons_position(
        button_t *self,
        SDL_Rect *screen
);
