#include <sdl.h>
#include <windows.h>

#include <stdio.h>

int WinMain(
        HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nShowCmd
) {
        SDL_Window *window;
        SDL_Surface *surface;
        int res = 0;

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
        SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(window);
        SDL_Delay(3000);

        SDL_DestroyWindow(window);
cleanup_sdl:
        SDL_Quit();
        return res;
}
