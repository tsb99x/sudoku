#include "layout.h"

#include <stdio.h>

void debug_output(
        SDL_Rect *r
) {
        fprintf(stderr, "{ x=%u, y=%u, w=%u, h=%u }\n",
                        r->x, r->y, r->w, r->h);
}

int test_layout_calc_drawable_space(
        void
) {
        SDL_Rect screen_space;
        SDL_Rect result;

        screen_space.x = 0;
        screen_space.y = 0;
        screen_space.w = 1024;
        screen_space.h = 768;
        result = layout_calc_drawable_space(&screen_space);
        debug_output(&result);
        if (result.x != 128)
                return 1;
        if (result.y != 0)
                return 1;
        if (result.w != 768)
                return 1;
        if (result.h != 768)
                return 1;

        screen_space.x = 0;
        screen_space.y = 0;
        screen_space.w = 768;
        screen_space.h = 1024;
        result = layout_calc_drawable_space(&screen_space);
        debug_output(&result);
        if (result.x != 0)
                return 1;
        if (result.y != 128)
                return 1;
        if (result.w != 768)
                return 1;
        if (result.h != 768)
                return 1;
        return 0;
}

int test_layout_add_padding(
        void
) {
        SDL_Rect canvas;
        SDL_Rect result;

        canvas.x = 128;
        canvas.y = 0;
        canvas.w = 768;
        canvas.h = 768;
        result = layout_add_padding(&canvas, .03f);
        debug_output(&result);
        if (result.x != 151)
                return 1;
        if (result.y != 23)
                return 1;
        if (result.w != 721)
                return 1;
        if (result.h != 721)
                return 1;
        return 0;
}

int test_layout_compensate_timer(
        void
) {
        SDL_Rect canvas;
        SDL_Rect result;

        canvas.x = 151;
        canvas.y = 23;
        canvas.w = 721;
        canvas.h = 721;
        result = layout_compensate_timer(&canvas, 100);
        debug_output(&result);
        if (result.x != 201)
                return 1;
        if (result.y != 123)
                return 1;
        if (result.w != 621)
                return 1;
        if (result.h != 621)
                return 1;
        return 0;
}

int test_layout_find_button_size(
        void
) {
        SDL_Rect canvas;
        int result;

        canvas.x = 201;
        canvas.y = 123;
        canvas.w = 621;
        canvas.h = 621;
        result = layout_find_button_size(&canvas, .01f, .03f);
        fprintf(stderr, "{ %d }\n", result);
        if (result != 60)
                return 1;
        return 0;
}

int main(
        int argc,
        char *argv[]
) {
        return test_layout_calc_drawable_space()
                + test_layout_add_padding()
                + test_layout_compensate_timer()
                + test_layout_find_button_size();
}
