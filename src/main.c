#include "context.h"
#include "game_scene.h"

#include <SDL.h>

#include <stdlib.h>
#include <time.h>

int main(
        int argc,
        char *argv[]
) {
        (void) argc;
        (void) argv;

        context_t *ctx;
        gc_data_t gc_data;
        SDL_Event event;
        int quit;

        srand(time(NULL));

        ctx = context_create(800, 800, 30, "C:/Windows/Fonts/Arial.ttf");
        if (!ctx) {
                SDL_Log("Failed to create context\n");
                return -1;
        }

        if (!game_scene_create(&gc_data, ctx)) {
                SDL_Log("Failed to create game scene\n");
                context_destroy(ctx);
                return -1;
        }

        quit = 0;
        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT)
                                quit = 1;
                        if (event.type == SDL_KEYDOWN)
                                if (event.key.keysym.sym == SDLK_ESCAPE)
                                        quit = 1;
                        game_scene_update(&gc_data, &event);
                }
                context_clear_screen(ctx);
                game_scene_render(&gc_data);
                context_present_screen(ctx);
        }

        game_scene_destroy(&gc_data);
        context_destroy(ctx);
        return 0;
}
