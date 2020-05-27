#pragma once

#include "button.h"
#include "context.h"
#include "digit.h"
#include "grid.h"
#include "layout.h"

#include <SDL.h>

typedef struct gc_data {
        context_t *ctx;
        SDL_Texture **digits;
        grid_t *grid;
        button_t *buttons;
        layout_t *layout;
} gc_data_t;

int game_scene_create(
        gc_data_t *data,
        context_t *ctx
);

void game_scene_destroy(
        gc_data_t *data
);

void game_scene_update(
        void *data,
        SDL_Event *event
);

void game_scene_render(
        void *data
);
