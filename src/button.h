#pragma once

#include "context.h"
#include "layout.h"
#include "grid.h"

typedef struct button button_t;

button_t *buttons_create(
        grid_t *grid
);

void buttons_destroy(
        button_t *self
);

void buttons_update(
        button_t *self,
        SDL_Point *mouse_pos
);

void buttons_click(
        button_t *self,
        unsigned char mouse_bt
);

void buttons_draw(
        button_t *self,
        context_t *ctx,
        SDL_Texture **digits
);

void buttons_position(
        button_t *self,
        layout_t *layout
);
