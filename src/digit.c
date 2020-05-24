#include "digit.h"

#include "palette.h"

#include <stdlib.h>

SDL_Texture** digits_create(
        context_t* ctx
) {
        SDL_Texture** digits;
        int i;
        char glyph;

        digits = malloc(9 * sizeof(SDL_Texture*));
        if (!digits) {
                SDL_Log("Failed to allocate memory for digits\n");
                return NULL;
        }
        for (i = 0; i < 9; ++i) {
                glyph = (char)('1' + i);
                digits[i] = context_prepare_glyph(ctx, glyph, digit_color);
        }
        return digits;
}

void digits_destroy(
        SDL_Texture** digits
) {
        int i;

        for (i = 0; i < 9; ++i)
                SDL_DestroyTexture(digits[i]);
        free(digits);
}
