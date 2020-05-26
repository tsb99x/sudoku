#include "button.h"

#include "palette.h"

#include <SDL.h>

#include <stdlib.h>

typedef enum state {
        IDLE,
        HOVERED,
        PRESET
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
                        buttons->state = (buttons->val == 0) ? IDLE : PRESET;
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

void buttons_update(
        button_t *self,
        SDL_Point *mouse_pos
) {
        SDL_Rect rect;
        int i;

        for (i = 0; i < ROWS * COLS; ++i) {
                if (self->state != PRESET) {
                        rect.x = self->x;
                        rect.y = self->y;
                        rect.w = rect.h = self->size;
                        if (SDL_PointInRect(mouse_pos, &rect))
                                self->state = HOVERED;
                        else
                                self->state = IDLE;
                }
                ++self;
        }
}

void buttons_click(
        button_t *self,
        unsigned char mouse_bt
) {
        int i;

        for (i = 0; i < ROWS * COLS; ++i) {
                if (self->state == HOVERED) {
                        switch (mouse_bt) {
                        case SDL_BUTTON_LEFT:
                                self->val++;
                                break;
                        case SDL_BUTTON_RIGHT:
                                self->val--;
                                break;
                        }
                        if (self->val > 9)
                                self->val = 0;
                        if (self->val < 0)
                                self->val = 9;
                }
                ++self;
        }
}

void buttons_draw(
        button_t *self,
        context_t *ctx,
        SDL_Texture **digits
) {
        SDL_Rect rect;
        SDL_Texture *tex;
        SDL_Point pos;
        int i;

        for (i = 0; i < ROWS * COLS; ++i) {
                rect.x = self->x;
                rect.y = self->y;
                rect.w = rect.h = self->size;

                switch (self->state) {
                case IDLE:
                        context_set_draw_color(ctx, clickable_color);
                        break;
                case HOVERED:
                        context_set_draw_color(ctx, hovered_color);
                        break;
                case PRESET:
                        context_set_draw_color(ctx, preset_color);
                        break;
                }
                context_draw_rect(ctx, &rect);

                if (self->val > 0) {
                        tex = digits[self->val - 1];
                        pos.x = rect.x + rect.w / 2;
                        pos.y = rect.y + rect.h / 2;
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
                        self->x = rect.x;
                        self->y = rect.y;
                        self->size = rect.w;
                        ++self;
                }
}
