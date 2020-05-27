#include "game_scene.h"

#include "palette.h"

#include <stdio.h>
#include <string.h>

int game_scene_create(
        gc_data_t *data,
        context_t *ctx
) {
        percent_t canvas_padding = .03f;
        percent_t timer_size     = .10f;
        percent_t timer_pad      = .02f;
        percent_t small_gap      = .01f;
        percent_t large_gap      = .03f;

        SDL_Rect screen;
        int timer_height;

        memset(data, 0, sizeof(gc_data_t));
        data->ctx = ctx;

        data->digits = digits_create(data->ctx);
        if (!data->digits) {
                SDL_Log("Failed to create digits\n");
                game_scene_destroy(data);
                return 0;
        }

        data->grid = grid_create();
        if (!data->grid) {
                SDL_Log("Failed to create grid\n");
                game_scene_destroy(data);
                return 0;
        }
        grid_erase(data->grid);

        data->buttons = buttons_create(data->grid);
        if (!data->buttons) {
                SDL_Log("Failed to create buttons\n");
                game_scene_destroy(data);
                return 0;
        }

        data->layout = layout_create(small_gap, large_gap, canvas_padding,
                                timer_size, timer_pad);
        if (!data->layout) {
                SDL_Log("Failed to create layout\n");
                game_scene_destroy(data);
                return 0;
        }

        screen = context_get_screen_size(data->ctx);
        layout_calc(data->layout, &screen);
        timer_height = layout_get_timer_height(data->layout);
        context_resize_font(data->ctx, timer_height);
        digits_recache(data->digits, data->ctx);
        buttons_position(data->buttons, data->layout);
        return 1;
}

void game_scene_destroy(
        gc_data_t *data
) {
        if (data->buttons)
                buttons_destroy(data->buttons);
        if (data->digits)
                digits_destroy(data->digits);
        if (data->grid)
                grid_destroy(data->grid);
        if (data->layout)
                layout_destroy(data->layout);
}

static void on_window_event(
        SDL_WindowEvent *e,
        context_t *ctx,
        SDL_Texture **digits,
        button_t *buttons,
        layout_t *layout
) {
        SDL_Rect screen;
        int timer_height;

        if (e->event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                screen.x = screen.y = 0;
                screen.w = e->data1;
                screen.h = e->data2;
                layout_calc(layout, &screen);
                timer_height = layout_get_timer_height(layout);
                context_resize_font(ctx, timer_height);
                digits_recache(digits, ctx);
                buttons_position(buttons, layout);
        }
}

void game_scene_update(
        void *data,
        SDL_Event *event
) {
        gc_data_t *gc_data = data;

        SDL_Point mouse_pos;

        if (event->type == SDL_WINDOWEVENT)
                on_window_event(&event->window, gc_data->ctx, gc_data->digits, gc_data->buttons, gc_data->layout);
        if (event->type == SDL_MOUSEMOTION) {
                mouse_pos.x = event->motion.x;
                mouse_pos.y = event->motion.y;
                buttons_update(gc_data->buttons, &mouse_pos);
        }
        if (event->type == SDL_MOUSEBUTTONUP)
                buttons_click(gc_data->buttons, event->button.button);
}

static void draw_timer(
        context_t *ctx,
        layout_t *layout
) {
        SDL_Point pos;
        unsigned int seconds_from_start;
        unsigned int timer_seconds;
        unsigned int timer_minutes;
        char timer_str[8];

        seconds_from_start = SDL_GetTicks() / 1000;
        timer_minutes = seconds_from_start / 60;
        timer_seconds = seconds_from_start % 60;
        sprintf(timer_str, "%02u:%02u", timer_minutes, timer_seconds);
        pos = layout_get_timer_pos(layout);
        context_draw_string(ctx, timer_str, timer_color, &pos);
}

void game_scene_render(
        void *data
) {
        gc_data_t *gc_data = data;

        draw_timer(gc_data->ctx, gc_data->layout);
        buttons_draw(gc_data->buttons, gc_data->ctx, gc_data->digits);
}
