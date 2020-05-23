#include "context.h"

#define DEFAULT_FRAME_RATE 30

context_t *context_create(
        void
) {
        SDL_DisplayMode mode;
        int refresh_rate;
        context_t *ctx = calloc(1, sizeof(context_t));

        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
                SDL_Log("Failed to init SDL: %s\n", SDL_GetError());
                context_destroy(ctx);
                return NULL;
        }

        if (TTF_Init() != 0) {
                SDL_Log("Failed to init TTF: %s\n", TTF_GetError());
                context_destroy(ctx);
                return NULL;
        }

        if (SDL_GetDisplayMode(0, 0, &mode) != 0) {
                SDL_Log("Failed to get best display mode: %s\n", SDL_GetError());
                context_destroy(ctx);
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
                context_destroy(ctx);
                return NULL;
        }

        ctx->renderer = SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_SOFTWARE);
        if (!ctx->renderer) {
                SDL_Log("Failed to create renderer: %s\n", SDL_GetError());
                context_destroy(ctx);
                return NULL;
        }

        ctx->font = TTF_OpenFont("C:/Windows/Fonts/Arial.ttf", 24);
        if (!ctx->font) {
                SDL_Log("Failed to open fond: %s\n", TTF_GetError());
                context_destroy(ctx);
                return NULL;
        }

        return ctx;
}

void context_destroy(
        context_t *ctx
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
