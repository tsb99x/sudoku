#pragma once

#include <sdl_pixels.h>

/* Color Scheme Definition */

SDL_Color white       = { 0xFF, 0xFF, 0xFF, 0xFF };
SDL_Color dark_green  = { 0x66, 0x99, 0x22, 0xFF };
SDL_Color light_green = { 0x99, 0xCC, 0x33, 0xFF };
SDL_Color orange      = { 0xFF, 0x60, 0x00, 0xFF };
SDL_Color black       = { 0x00, 0x00, 0x00, 0xFF };

/* Color Scheme Binding */

SDL_Color *background_color = &white;
SDL_Color *digit_color      = &white;
SDL_Color *preset_color     = &dark_green;
SDL_Color *clickable_color  = &light_green;
SDL_Color *hovered_color    = &orange;
SDL_Color *timer_color      = &black;
