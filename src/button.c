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

void buttons_position(
        button_t *self,
        layout_t *layout
) {
        int y, x;
        SDL_Rect rect;

        for (y = 0; y < ROWS; ++y)
                for (x = 0; x < COLS; ++x) {
                        rect = layout_get_button_rect(layout, x, y);
                        SDL_Log("SDL_rect { x=%d, y=%d, w=%d, h=%d }",
                                rect.x, rect.y, rect.w, rect.h);
                        self->x = rect.x;
                        self->y = rect.y;
                        self->size = rect.w;
                        ++self;
                }
}
