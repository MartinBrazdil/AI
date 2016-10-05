#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Rectangle.h"

void LoadFont(TTF_Font* font);
void BlitSurface(SDL_Surface* src, const SDL_Rect* src_rect, SDL_Texture* dst, SDL_Rect* dstrect);
