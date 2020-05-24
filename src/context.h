#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

typedef struct context context_t;

context_t *context_create(
        void
);

void context_destroy(
        context_t *self
);

void context_resize_font(
        context_t *self,
        int size
);

void context_set_draw_color(
        context_t *self,
        SDL_Color *col
);

void context_clear_screen(
        context_t *self
);

void context_draw_rect(
        context_t *self,
        SDL_Rect *rect
);

void context_draw_texture(
        context_t *self,
        SDL_Texture *texture,
        SDL_Point *pos
);

SDL_Texture *context_prepare_glyph(
        context_t *self,
        char glyph,
        SDL_Color *color
);

void context_draw_string(
        context_t *self,
        char *str,
        SDL_Color *color,
        SDL_Point *pos
);

void context_present_screen(
        context_t *self
);

SDL_Rect context_get_screen_size(
        context_t *self
);
