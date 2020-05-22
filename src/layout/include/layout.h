#include <sdl.h>

SDL_Rect layout_calc_drawable_space(
        SDL_Rect *screen_space
);

SDL_Rect layout_add_padding(
        SDL_Rect *canvas,
        float padding_in_percents
);

SDL_Rect layout_compensate_timer(
        SDL_Rect *canvas,
        int clock_height
);

int layout_find_button_size(
        SDL_Rect *canvas,
        float small_gap_size,
        float big_gap_size
);
