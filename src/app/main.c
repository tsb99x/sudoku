#include <sdl.h>
#include <windows.h>

#include <stdio.h>

#define TILE_SIZE 50
#define TILE_GAP 3

int WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nShowCmd
) {
        SDL_Window *window;
        SDL_Surface *surface;
        SDL_Rect rect;
        SDL_Event event;
        int x, y, mouse_x, mouse_y, mouse_bt, color;
        int res = 0, quit = 0;

        if (SDL_Init(SDL_INIT_VIDEO)) {
                SDL_Log("Failed to init SDL: %s\n", SDL_GetError());
                return -1;
        }

        window = SDL_CreateWindow(
                "Sudoku",
                SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                1024, 768,
                SDL_WINDOW_SHOWN
        );

        if (!window) {
                SDL_Log("Failed to create window: %s\n", SDL_GetError());
                res = -2;
                goto cleanup_sdl;
        }

        surface = SDL_GetWindowSurface(window);

        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT)
                                quit = 1;
                        if (event.type == SDL_KEYDOWN)
                                quit = 1;
                }

                SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

                mouse_bt = SDL_GetMouseState(&mouse_x, &mouse_y);

                for (x = 0; x < 9; ++x)
                        for (y = 0; y < 9; ++y) {
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
                                rect.w = rect.h = TILE_SIZE;

                                color = SDL_MapRGB(surface->format, 0xFF, 0, 0xFF);

                                if (mouse_x >= rect.x && mouse_x <= rect.x + rect.w && mouse_y >= rect.y && mouse_y <= rect.y + rect.h)
                                        if (SDL_BUTTON(mouse_bt) == SDL_BUTTON_LEFT)
                                                color = SDL_MapRGB(surface->format, 0, 0, 0xFF);
                                        else
                                                color = SDL_MapRGB(surface->format, 0, 0xFF, 0);

                                SDL_FillRect(surface, &rect, color);
                        }

                SDL_UpdateWindowSurface(window);
        }

        SDL_Log("Exit!\n");

        SDL_DestroyWindow(window);
cleanup_sdl:
        SDL_Quit();
        return res;
}
