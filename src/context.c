#include "context.h"

#include "palette.h"

#include <stdlib.h>

struct context {
        SDL_Window *window;
        SDL_Renderer *renderer;
        TTF_Font *font;
        int millis_per_frame;
        unsigned int last_render_time;
};

#define DEFAULT_FRAME_RATE 30
#define FONT_PATH "C:/Windows/Fonts/Arial.ttf"

context_t *context_create(
        void
) {
        SDL_DisplayMode mode;
        int refresh_rate;
        context_t *self;

        self = calloc(1, sizeof(context_t));
        if (!self)
                return NULL;

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                SDL_Log("Failed to init SDL: %s\n", SDL_GetError());
                context_destroy(self);
                return NULL;
        }

        if (TTF_Init() != 0) {
                SDL_Log("Failed to init TTF: %s\n", TTF_GetError());
                context_destroy(self);
                return NULL;
        }

        if (SDL_GetDisplayMode(0, 0, &mode) != 0) {
                SDL_Log("Failed to get best display mode: %s\n", SDL_GetError());
                context_destroy(self);
                return NULL;
        }

        refresh_rate = (mode.refresh_rate == 0)
                ? DEFAULT_FRAME_RATE
                : mode.refresh_rate;
        self->millis_per_frame = 1000 / refresh_rate;

        self->window = SDL_CreateWindow(
                "Sudoku",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                1024, 768,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );
        if (!self->window) {
                SDL_Log("Failed to create window: %s\n", SDL_GetError());
                context_destroy(self);
                return NULL;
        }

        self->renderer = SDL_CreateRenderer(self->window, -1, SDL_RENDERER_SOFTWARE);
        if (!self->renderer) {
                SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
                context_destroy(self);
                return NULL;
        }

        self->font = TTF_OpenFont(FONT_PATH, 24);
        if (!self->font) {
                SDL_Log("Failed to open font: %s\n", TTF_GetError());
                context_destroy(self);
                return NULL;
        }

        return self;
}

void context_destroy(
        context_t *self
) {
        if (self->font)
                TTF_CloseFont(self->font);
        if (self->renderer)
                SDL_DestroyRenderer(self->renderer);
        if (self->window)
                SDL_DestroyWindow(self->window);
        TTF_Quit();
        SDL_Quit();
        free(self);
}

void context_resize_font(
        context_t *self,
        int size
) {
        TTF_CloseFont(self->font);
        self->font = TTF_OpenFont(FONT_PATH, size);
}

void context_set_draw_color(
        context_t *self,
        SDL_Color *col
) {
        SDL_SetRenderDrawColor(self->renderer, col->r, col->g, col->b, col->a);
}

void context_clear_screen(
        context_t *self
) {
        context_set_draw_color(self, background_color);
        SDL_RenderClear(self->renderer);
}

void context_draw_rect(
        context_t *self,
        SDL_Rect *rect
) {
        SDL_RenderFillRect(self->renderer, rect);
}

void context_draw_texture(
        context_t *self,
        SDL_Texture *texture,
        SDL_Point *pos
) {
        SDL_Rect rect;

        rect.x = pos->x;
        rect.y = pos->y;
        SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
        SDL_RenderCopy(self->renderer, texture, NULL, &rect);
}

SDL_Texture *context_prepare_glyph(
        context_t *self,
        char glyph,
        SDL_Color *color
) {
        SDL_Surface *surface;
        SDL_Texture *texture;

        surface = TTF_RenderGlyph_Blended(self->font, glyph, *color);
        texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        SDL_FreeSurface(surface);
        return texture;
}

void context_draw_string(
        context_t *self,
        char *str,
        SDL_Color *color,
        SDL_Point *pos
) {
        SDL_Surface *surface;
        SDL_Texture *texture;

        surface = TTF_RenderUTF8_Blended(self->font, str, *color);
        texture = SDL_CreateTextureFromSurface(self->renderer, surface);
        context_draw_texture(self, texture, pos);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(surface);
}

void context_present_screen(
        context_t *self
) {
        unsigned int min_render_time;

        min_render_time = self->last_render_time + self->millis_per_frame;
        if (SDL_GetTicks() < min_render_time)
                SDL_Delay(min_render_time - SDL_GetTicks());
        self->last_render_time = SDL_GetTicks();
        SDL_RenderPresent(self->renderer);
}

SDL_Rect context_get_screen_size(
        context_t *self
) {
        SDL_Rect screen;

        screen.x = screen.y = 0;
        SDL_GetWindowSize(self->window, &screen.w, &screen.h);
        return screen;
}
