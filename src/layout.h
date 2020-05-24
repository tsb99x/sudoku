#pragma once

#include <SDL.h>

typedef float percent_t;
typedef struct layout layout_t;

layout_t *layout_create(
        percent_t small_gap,
        percent_t large_gap,
        percent_t screen_pad,
        percent_t timer_size,
        percent_t timer_pad
);

void layout_destroy(
        layout_t *self
);

void layout_calc(
        layout_t *self,
        SDL_Rect *screen
);

SDL_Rect layout_get_button_rect(
        layout_t *self,
        int x,
        int y
);

SDL_Point layout_get_timer_pos(
        layout_t *self
);

int layout_get_timer_height(
        layout_t *self
);
