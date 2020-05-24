#include "digit.h"

#include "palette.h"

#include <stdlib.h>

SDL_Texture** digits_create(
        context_t* ctx
) {
        SDL_Texture** digits;

        digits = calloc(9, sizeof(SDL_Texture*));
        if (!digits) {
                SDL_Log("Failed to allocate memory for digits\n");
                return NULL;
        }
        digits_recache(digits, ctx);
        return digits;
}

void digits_destroy(
        SDL_Texture *digits[]
) {
        int i;

        for (i = 0; i < 9; ++i)
                SDL_DestroyTexture(digits[i]);
        free(digits);
}

void digits_recache(
        SDL_Texture *digits[],
        context_t* ctx
) {
        int i;
        char glyph;

        for (i = 0; i < 9; ++i) {
                if (digits[i] != NULL)
                        SDL_DestroyTexture(digits[i]);
                glyph = (char)('1' + i);
                digits[i] = context_prepare_glyph(ctx, glyph, digit_color);
        }
}
