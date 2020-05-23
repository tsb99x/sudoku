#include "button.h"

#include "palette.h"
#include "layout.h"

#include <SDL.h>

#include <stdlib.h>

typedef enum state {
        IDLE,
        HOVERED,
        CLICKED
} state_t;

struct button {
        int x, y, size, val;
        state_t state;
};

static void assign_state(
        button_t *buttons,
        grid_t *grid
) {
        int x, y;

        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        buttons->x = x;
                        buttons->y = y;
                        buttons->val = grid_cell_at(grid, x, y);
                        buttons->state = IDLE;
                        ++buttons;
                }
}

button_t *buttons_create(
        grid_t *grid
) {
        button_t *buttons;

        buttons = malloc(CELLS_LEN * sizeof(button_t));
        if (!buttons)
                return NULL;
        assign_state(buttons, grid);
        return buttons;
}

void buttons_destroy(
        button_t *self
) {
        free(self);
}

void buttons_draw(
        button_t *self,
        context_t *ctx,
        SDL_Texture **digits,
        SDL_Point mouse_pos
) {
        SDL_Rect rect;
        SDL_Texture *tex;
        SDL_Point pos;
        int x, y;

        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        rect.x = self->x;
                        rect.y = self->y;
                        rect.w = self->size;
                        rect.h = self->size;

                        context_set_draw_color(ctx, clickable_color);
                        if (SDL_PointInRect(&mouse_pos, &rect))
                                context_set_draw_color(ctx, hovered_color);
                        context_draw_rect(ctx, &rect);

                        if (self->val > 0) {
                                tex = digits[self->val - 1];
                                pos.x = rect.x;
                                pos.y = rect.y;
                                context_draw_texture(ctx, tex, &pos);
                        }
                        ++self;
                }
}

#define CANVAS_PADDING_PC .03f
#define TIMER_H 50
#define S_GAP_PC .01f
#define L_GAP_PC .03f

void buttons_position(
        button_t *self,
        SDL_Rect *screen
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
                        self->x = pos_x;
                        self->y = pos_y;
                        self->size = button_size;
                        ++self;
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
