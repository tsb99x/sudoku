#include <sdl.h>
#include <sdl_ttf.h>
#include <windows.h>

#include <stdio.h>

#define TILE_SIZE 50
#define TILE_GAP 3

/* Simplify Digit Drawing by Caching */

SDL_Texture *digits_cache[9];

static void buffer_digits(
        SDL_Renderer *renderer,
        TTF_Font *font,
        SDL_Color *color,
        SDL_Texture *texs[]
) {
        SDL_Surface *surface;
        int i;
        char sz[2] = "i";

        for (i = 0; i < 9; ++i) {
                sz[0] = '0' + i + 1;
                surface = TTF_RenderUTF8_Blended(font, sz, *color);
                texs[i] = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
        }
}

static void destroy_digits(
        SDL_Texture *texs[]
) {
        int i;

        for (i = 0; i < 9; ++i) {
                SDL_DestroyTexture(texs[i]);
        }
}

/* Color Scheme Definition */

SDL_Color white       = { 0xFF, 0xFF, 0xFF, 0xFF };
SDL_Color dark_green  = { 0x66, 0x99, 0x22, 0xFF };
SDL_Color light_green = { 0x99, 0xCC, 0x33, 0xFF };
SDL_Color orange      = { 0xFF, 0x60, 0x00, 0xFF };
SDL_Color black       = { 0x00, 0x00, 0x00, 0xFF };

/* Color Scheme Binding */

SDL_Color *background_color = &white;
SDL_Color *digit_color      = &white;
SDL_Color *preset_color     = &dark_green;
SDL_Color *clickable_color  = &light_green;
SDL_Color *hovered_color    = &orange;
SDL_Color *timer_color      = &black;

/* Main GUI Element, Button */

enum state {
        IDLE,
        HOVERED,
        CLICKED
};

struct button {
        int x, y, val;
        enum state state;
};

#define ROWS 9
#define COLS 9

struct button buttons[ROWS * COLS];

static void init_buttons(
        struct button *buttons
) {
        int x, y;

        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        buttons->x = x;
                        buttons->y = y;
                        buttons->val = 1;
                        buttons->state = IDLE;
                        ++buttons;
                }
}

static int is_point_in_rect(
        int x,
        int y,
        SDL_Rect *r
) {
        return x >= r->x
                && x <= r->x + r->w
                && y >= r->y
                && y <= r->y + r->h;
}

/* Utility Wrappers to Simplify SDL for Myself */

static void set_draw_color(
        SDL_Renderer *r,
        SDL_Color *c
) {
        SDL_SetRenderDrawColor(r, c->r, c->g, c->b, c->a);
}

int WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nShowCmd
) {
        SDL_Window *window;
        SDL_Renderer *renderer;
        SDL_Rect rect, dstrect;
        SDL_Color *color;
        SDL_Event event;
        SDL_Surface *text;
        SDL_Texture *texture;
        TTF_Font *font;
        int i, x, y, mouse_x, mouse_y, mouse_bt, w, h;
        int res = 0, quit = 0;

        if (SDL_Init(SDL_INIT_VIDEO)) {
                SDL_Log("Failed to init SDL: %s\n", SDL_GetError());
                return -1;
        }

        if (TTF_Init()) {
                SDL_Log("Failed to init TTF: %s\n", TTF_GetError());
                res = -2;
                goto cleanup_sdl;
        }

        window = SDL_CreateWindow(
                "Sudoku", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                1024, 768, SDL_WINDOW_SHOWN
        );
        if (!window) {
                SDL_Log("Failed to create window: %s\n", SDL_GetError());
                res = -3;
                goto cleanup_ttf;
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (!renderer) {
                SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
                res = -4;
                goto cleanup_window;
        }

        font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24);
        if (!font) {
                SDL_Log("Failed to open fond: %s\n", TTF_GetError());
                res = -5;
                goto cleanup_renderer;
        }

        buffer_digits(renderer, font, digit_color, digits_cache);
        init_buttons(buttons);

        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT)
                                quit = 1;
                        if (event.type == SDL_KEYDOWN)
                                if (event.key.keysym.sym == SDLK_ESCAPE)
                                        quit = 1;
                }

                set_draw_color(renderer, background_color);
                SDL_RenderClear(renderer);

                mouse_bt = SDL_GetMouseState(&mouse_x, &mouse_y);

                rect.w = rect.h = TILE_SIZE;
                for (y = 0; y < ROWS; ++y)
                        for (x = 0; x < COLS; ++x) {
                                rect.x = x * (TILE_SIZE + TILE_GAP);
                                if (x >= 3)
                                        rect.x += TILE_GAP * 2;
                                if (x >= 6)
                                        rect.x += TILE_GAP * 2;
                                rect.y = y * (TILE_SIZE + TILE_GAP);
                                if (y >= 3)
                                        rect.y += TILE_GAP * 2;
                                if (y >= 6)
                                        rect.y += TILE_GAP * 2;

                                set_draw_color(renderer, clickable_color);
                                if (is_point_in_rect(mouse_x, mouse_y, &rect))
                                        set_draw_color(renderer, hovered_color);
                                SDL_RenderFillRect(renderer, &rect);

                                if (buttons[y + x].val > 0) {
                                        SDL_Texture *tex = digits_cache[buttons[y + x].val - 1];
                                        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
                                        dstrect = (SDL_Rect) { rect.x, rect.y, w, h };
                                        SDL_RenderCopy(renderer, tex, NULL, &dstrect);
                                }
                        }

                text = TTF_RenderUTF8_Blended(font, "00:00", *timer_color);
                texture = SDL_CreateTextureFromSurface(renderer, text);

                SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                dstrect = (SDL_Rect) { 0, 0, w, h };

                SDL_RenderCopy(renderer, texture, NULL, &dstrect);

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(text);

                SDL_RenderPresent(renderer);
        }

        destroy_digits(digits_cache);

        TTF_CloseFont(font);
cleanup_renderer:
        SDL_DestroyRenderer(renderer);
cleanup_window:
        SDL_DestroyWindow(window);
cleanup_ttf:
        TTF_Quit();
cleanup_sdl:
        SDL_Quit();
        return res;
}
