#include <sdl.h>
#include <sdl_ttf.h>
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
        SDL_Renderer *renderer;
        SDL_Rect rect, dstrect;
        SDL_Color color;
        SDL_Event event;
        SDL_Surface *text;
        SDL_Texture *texture;
        TTF_Font *font;
        int x, y, mouse_x, mouse_y, mouse_bt, w, h;
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

        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT)
                                quit = 1;
                        if (event.type == SDL_KEYDOWN)
                                quit = 1;
                }

                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0);
                SDL_RenderClear(renderer);

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

                                SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, 0);

                                if (mouse_x >= rect.x && mouse_x <= rect.x + rect.w && mouse_y >= rect.y && mouse_y <= rect.y + rect.h)
                                        if (SDL_BUTTON(mouse_bt) == SDL_BUTTON_LEFT)
                                                SDL_SetRenderDrawColor(renderer, 0, 0, 0xFF, 0);
                                        else
                                                SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0);

                                SDL_RenderFillRect(renderer, &rect);
                        }

                color = (SDL_Color) { 0, 0, 0, 0 };
                text = TTF_RenderUTF8_Blended(font, "Hello, Sudoku!", color);
                texture = SDL_CreateTextureFromSurface(renderer, text);

                SDL_QueryTexture(texture, NULL, NULL, &w, &h);
                dstrect = (SDL_Rect) { 0, 0, w, h };

                SDL_RenderFillRect(renderer, &rect);
                SDL_RenderCopy(renderer, texture, NULL, &dstrect);

                SDL_DestroyTexture(texture);
                SDL_FreeSurface(text);

                SDL_RenderPresent(renderer);
        }

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
