#include "layout.h"

#include <stdlib.h>

struct layout {
        percent_t small_gap;
        percent_t large_gap;
        percent_t screen_pad;
        percent_t timer_size;
        percent_t timer_pad;
        SDL_Point timer_pos;
        int timer_height;
        SDL_Rect button_canvas;
        int button_size;
};

static SDL_Rect layout_calc_drawable_space(
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

static SDL_Rect layout_add_padding(
        SDL_Rect *canvas,
        percent_t padding
) {
        SDL_Rect padded;
        int padding_in_px = (int) (padding * canvas->w);

        padded.x = canvas->x + padding_in_px;
        padded.y = canvas->y + padding_in_px;
        padded.w = canvas->w - padding_in_px * 2;
        padded.h = canvas->h - padding_in_px * 2;
        return padded;
}

static SDL_Point layout_find_timer_pos(
        SDL_Rect *canvas,
        percent_t timer_size,
        percent_t timer_pad
) {
        SDL_Point timer_pos;
        int timer_h = (int) (timer_size * canvas->w);
        int timer_p = (int) (timer_pad * canvas->w);

        timer_pos.x = canvas->x + canvas->w / 2;
        timer_pos.y = canvas->y + timer_h / 2 + timer_p;

        return timer_pos;
}

static SDL_Rect layout_compensate_timer(
        SDL_Rect *canvas,
        percent_t timer_size,
        percent_t timer_pad
) {
        SDL_Rect field;
        int timer_h = (int) ((timer_size + timer_pad * 2) * canvas->w);

        field.x = canvas->x + timer_h / 2;
        field.y = canvas->y + timer_h;
        field.w = canvas->w - timer_h;
        field.h = canvas->h - timer_h;
        return field;
}

static int layout_find_button_size(
        SDL_Rect *canvas,
        float small_gap_size,
        float big_gap_size
) {
        int small_gap_in_px = (int) (small_gap_size * canvas->w);
        int big_gap_in_px = (int) (big_gap_size * canvas->w);

        return (canvas->w - (6 * small_gap_in_px) - (2 * big_gap_in_px)) / 9;
}

layout_t *layout_create(
        percent_t small_gap,
        percent_t large_gap,
        percent_t screen_pad,
        percent_t timer_size,
        percent_t timer_pad
) {
        layout_t *self;

        self = calloc(1, sizeof(layout_t));
        if (!self)
                return NULL;
        self->small_gap  = small_gap;
        self->large_gap  = large_gap;
        self->screen_pad = screen_pad;
        self->timer_size = timer_size;
        self->timer_pad  = timer_pad;
        return self;
}

void layout_destroy(
        layout_t *self
) {
        free(self);
}

void layout_calc(
        layout_t *self,
        SDL_Rect *screen
) {
        SDL_Rect canvas;

        canvas = layout_calc_drawable_space(screen);
        canvas = layout_add_padding(&canvas, self->screen_pad);
        self->timer_height = self->timer_size * canvas.w;
        self->timer_pos = layout_find_timer_pos(&canvas, self->timer_size, self->timer_pad);
        canvas = layout_compensate_timer(&canvas, self->timer_size, self->timer_pad);
        self->button_canvas = canvas;
        self->button_size = layout_find_button_size(&canvas, self->small_gap, self->large_gap);
}

static int calc_gap(
        int i,
        int sg,
        int lg
) {
        return i * sg + (i >= 3 ? lg : 0) + (i >= 6 ? lg : 0);
}

SDL_Rect layout_get_button_rect(
        layout_t *self,
        int x,
        int y
) {
        SDL_Rect rect;
        int small_gap, large_gap;

        small_gap = (int) (self->button_canvas.w * self->small_gap);
        large_gap = (int) (self->button_canvas.w * self->large_gap);

        rect.x = self->button_canvas.x + x * self->button_size
                + calc_gap(x, small_gap, large_gap);
        rect.y = self->button_canvas.y + y * self->button_size
                + calc_gap(y, small_gap, large_gap);
        rect.w = rect.h = self->button_size;
        return rect;
}

SDL_Point layout_get_timer_pos(
        layout_t *self
) {
        return self->timer_pos;
}

int layout_get_timer_height(
        layout_t *self
) {
        return self->timer_height;
}
