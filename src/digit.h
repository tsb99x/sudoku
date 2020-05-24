#pragma once

#include "context.h"

#include <SDL.h>

SDL_Texture** digits_create(
        context_t* ctx
);

void digits_destroy(
        SDL_Texture** digits
);
