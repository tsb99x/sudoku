#include "layout.h"

SDL_Rect layout_calc_drawable_space(
        SDL_Rect *screen_space
) {
        SDL_Rect canvas;

        if (screen_space->w >= screen_space->h) {
                canvas.x = (screen_space->w - screen_space->h) / 2;
                canvas.y = 0;
                canvas.w = screen_space->h;
                canvas.h = screen_space->h;
        } else {
                canvas.x = 0;
                canvas.y = (screen_space->h - screen_space->w) / 2;
                canvas.w = screen_space->w;
                canvas.h = screen_space->w;
        }
        return canvas;
}

SDL_Rect layout_add_padding(
        SDL_Rect *canvas,
        float padding_in_percents
) {
        SDL_Rect padded;
        int padding_in_px = (int) (padding_in_percents * canvas->w);

        padded.x = canvas->x + padding_in_px;
        padded.y = canvas->y + padding_in_px;
        padded.w = canvas->w - padding_in_px * 2;
        padded.h = canvas->h - padding_in_px * 2;
        return padded;
}

SDL_Rect layout_compensate_timer(
        SDL_Rect *canvas,
        int timer_h
) {
        SDL_Rect field;

        field.x = canvas->x + timer_h / 2;
        field.y = canvas->y + timer_h;
        field.w = canvas->w - timer_h;
        field.h = canvas->h - timer_h;
        return field;
}

int layout_find_button_size(
        SDL_Rect *canvas,
        float small_gap_size,
        float big_gap_size
) {
        int small_gap_in_px = (int) (small_gap_size * canvas->w);
        int big_gap_in_px = (int) (big_gap_size * canvas->w);

        return (canvas->w - (6 * small_gap_in_px) - (2 * big_gap_in_px)) / 9;
}
