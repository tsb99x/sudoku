#include <layout.h>

#include <sdl.h>
#include <sdl_ttf.h>
#include <windows.h>

#include <stdio.h>

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

/* Main Context Structure */

struct ctx {
        SDL_Window *window;
        SDL_Renderer *renderer;
        TTF_Font *font;
        int millis_per_frame;
        unsigned int last_render_time;
};

#define DEFAULT_FRAME_RATE 30

static void destroy_context(
        struct ctx *ctx
) {
        if (ctx->font)
                TTF_CloseFont(ctx->font);
        if (ctx->renderer)
                SDL_DestroyRenderer(ctx->renderer);
        if (ctx->window)
                SDL_DestroyWindow(ctx->window);
        TTF_Quit();
        SDL_Quit();
        free(ctx);
}

struct ctx *create_context(
        void
) {
        SDL_DisplayMode mode;
        int refresh_rate;
        struct ctx *ctx = calloc(1, sizeof(struct ctx));

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                SDL_Log("Failed to init SDL: %s\n", SDL_GetError());
                destroy_context(ctx);
                return NULL;
        }

        if (TTF_Init() != 0) {
                SDL_Log("Failed to init TTF: %s\n", TTF_GetError());
                destroy_context(ctx);
                return NULL;
        }

        if (SDL_GetDisplayMode(0, 0, &mode) != 0) {
                SDL_Log("Failed to get best display mode: %s\n", SDL_GetError());
                destroy_context(ctx);
                return NULL;
        }

        refresh_rate = (mode.refresh_rate == 0)
                ? DEFAULT_FRAME_RATE
                : mode.refresh_rate;
        ctx->millis_per_frame = 1000 / refresh_rate;

        ctx->window = SDL_CreateWindow(
                "Sudoku",
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
                1024, 768,
                SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
        );
        if (!ctx->window) {
                SDL_Log("Failed to create window: %s\n", SDL_GetError());
                destroy_context(ctx);
                return NULL;
        }

        ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_SOFTWARE);
        if (!ctx->renderer) {
                SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
                destroy_context(ctx);
                return NULL;
        }

        ctx->font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24);
        if (!ctx->font) {
                SDL_Log("Failed to open fond: %s\n", TTF_GetError());
                destroy_context(ctx);
                return NULL;
        }

        return ctx;
}

/* Simplify Digit Drawing by Caching */

static SDL_Texture **create_digits(
        struct ctx *ctx,
        SDL_Color *color
) {
        SDL_Texture **digits;
        SDL_Surface *surface;
        int i;
        char glyph;

        digits = malloc(9 * sizeof(SDL_Texture *));
        if (!digits) {
                SDL_Log("Failed to allocate memory for digits\n");
                return NULL;
        }
        for (i = 0; i < 9; ++i) {
                glyph = '1' + i;
                surface = TTF_RenderGlyph_Blended(ctx->font, glyph, *color);
                digits[i] = SDL_CreateTextureFromSurface(ctx->renderer, surface);
                SDL_FreeSurface(surface);
        }
        return digits;
}

static void destroy_digits(
        SDL_Texture **digits
) {
        int i;

        for (i = 0; i < 9; ++i)
                SDL_DestroyTexture(digits[i]);
        free(digits);
}

/* Main GUI Element, Button */

enum state {
        IDLE,
        HOVERED,
        CLICKED
};

struct button {
        int x, y, size, val;
        enum state state;
};

#define ROWS 9
#define COLS 9

static struct button *create_buttons(
        void
) {
        struct button *buttons;
        int x, y, pos;

        buttons = malloc(ROWS * COLS * sizeof(struct button));
        if (!buttons) {
                SDL_Log("Failed to allocate memory for buttons\n");
                return NULL;
        }
        pos = 0;
        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        buttons[pos].x = x;
                        buttons[pos].y = y;
                        buttons[pos].val = 1;
                        buttons[pos].state = IDLE;
                        ++pos;
                }
        return buttons;
}

static void destroy_buttons(
        struct button *buttons
) {
        free(buttons);
}

/* Utility Wrappers to Simplify SDL */

static void set_draw_color(
        struct ctx *ctx,
        SDL_Color *col
) {
        SDL_SetRenderDrawColor(ctx->renderer, col->r, col->g, col->b, col->a);
}

static void clear_screen(
        struct ctx *ctx
) {
        set_draw_color(ctx, background_color);
        SDL_RenderClear(ctx->renderer);
}

static void draw_buttons(
        struct ctx *ctx,
        SDL_Texture **digits,
        struct button *buttons,
        SDL_Point mouse_pos
) {
        SDL_Rect rect, dstrect;
        SDL_Texture *tex;
        int x, y;

        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        rect.x = buttons->x;
                        rect.y = buttons->y;
                        rect.w = buttons->size;
                        rect.h = buttons->size;

                        set_draw_color(ctx, clickable_color);
                        if (SDL_PointInRect(&mouse_pos, &rect))
                                set_draw_color(ctx, hovered_color);
                        SDL_RenderFillRect(ctx->renderer, &rect);

                        if (buttons->val > 0) {
                                tex = digits[buttons->val - 1];
                                dstrect.x = rect.x;
                                dstrect.y = rect.y;
                                SDL_QueryTexture(tex, NULL, NULL, &dstrect.w, &dstrect.h);
                                SDL_RenderCopy(ctx->renderer, tex, NULL, &dstrect);
                        }
                        ++buttons;
                }
}

static void draw_timer(
        struct ctx *ctx
) {
        SDL_Surface *text;
        SDL_Texture *texture;
        SDL_Rect dstrect;
        unsigned int seconds_from_start;
        unsigned int timer_seconds;
        unsigned int timer_minutes;
        char timer_str[8];

        seconds_from_start = SDL_GetTicks() / 1000;
        timer_minutes = seconds_from_start / 60;
        timer_seconds = seconds_from_start % 60;
        sprintf(timer_str, "%02u:%02u", timer_minutes, timer_seconds);

        text = TTF_RenderUTF8_Blended(ctx->font, timer_str, *timer_color);
        texture = SDL_CreateTextureFromSurface(ctx->renderer, text);

        dstrect.x = 0;
        dstrect.y = 0;
        SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);

        SDL_RenderCopy(ctx->renderer, texture, NULL, &dstrect);

        SDL_DestroyTexture(texture);
        SDL_FreeSurface(text);
}

static void present_screen(
        struct ctx *ctx
) {
        unsigned int min_render_time;

        min_render_time = ctx->last_render_time + ctx->millis_per_frame;
        if (SDL_GetTicks() < min_render_time)
                SDL_Delay(min_render_time - SDL_GetTicks());
        ctx->last_render_time = SDL_GetTicks();
        SDL_RenderPresent(ctx->renderer);
}

#define CANVAS_PADDING_PC .03f
#define TIMER_H 50
#define S_GAP_PC .01f
#define L_GAP_PC .03f

static void position_buttons(
        SDL_Rect *screen,
        struct button *buttons
) {
        SDL_Rect canvas;
        int button_size, iy, ix, pos_y, pos_x, small_gap, large_gap;

        canvas = layout_calc_drawable_space(screen);
        canvas = layout_add_padding(&canvas, CANVAS_PADDING_PC);
        canvas = layout_compensate_timer(&canvas, TIMER_H);
        button_size = layout_find_button_size(&canvas, S_GAP_PC, L_GAP_PC);

        small_gap = (int) (canvas.w * S_GAP_PC);
        large_gap = (int) (canvas.w * L_GAP_PC);

        pos_y = canvas.y;
        for (iy = 0; iy < ROWS; ++iy) {
                pos_x = canvas.x;
                for (ix = 0; ix < COLS; ++ix) {
                        buttons->x = pos_x;
                        buttons->y = pos_y;
                        buttons->size = button_size;
                        ++buttons;
                        pos_x += button_size;
                        pos_x += (ix + 1) % 3
                                ? small_gap
                                : large_gap;
                }
                pos_y += button_size;
                pos_y += (iy + 1) % 3
                        ? small_gap
                        : large_gap;
        }
}

static void on_window_event(
        SDL_WindowEvent *e,
        struct button *buttons
) {
        SDL_Rect screen;

        if (e->event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                screen.x = 0;
                screen.y = 0;
                screen.w = e->data1;
                screen.h = e->data2;
                position_buttons(&screen, buttons);
        }
}

static void on_keydown_event(
        SDL_KeyboardEvent *e,
        int *quit
) {
        if (e->keysym.sym == SDLK_ESCAPE)
                *quit = 1;
}

int main(
        int argc,
        char *argv[]
) {
        struct ctx *ctx;
        SDL_Texture **digits;
        struct button *buttons;
        SDL_Rect screen;
        SDL_Event event;
        SDL_Point mouse_pos;
        int quit;

        ctx = create_context();
        if (!ctx) {
                SDL_Log("Failed to create context\n");
                return -1;
        }

        digits = create_digits(ctx, digit_color);
        if (!digits) {
                SDL_Log("Failed to create digits\n");
                destroy_context(ctx);
                return -1;
        }

        buttons = create_buttons();
        if (!buttons) {
                SDL_Log("Failed to create buttons\n");
                destroy_context(ctx);
                destroy_digits(digits);
                return -1;
        }

        screen.x = screen.y = 0;
        SDL_GetWindowSize(ctx->window, &screen.w, &screen.h);
        position_buttons(&screen, buttons);

        quit = 0;
        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT)
                                quit = 1;
                        if (event.type == SDL_WINDOWEVENT)
                                on_window_event(&event.window, buttons);
                        if (event.type == SDL_KEYDOWN)
                                on_keydown_event(&event.key, &quit);
                }
                SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);
                clear_screen(ctx);
                draw_buttons(ctx, digits, buttons, mouse_pos);
                draw_timer(ctx);
                present_screen(ctx);
        }

        destroy_buttons(buttons);
        destroy_digits(digits);
        destroy_context(ctx);
        return 0;
}
