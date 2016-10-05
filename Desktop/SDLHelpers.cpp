#include "SDLHelpers.h"

#include <algorithm>
using namespace std;

#include "Core.h"
#include "Rectangle.h"

void LoadFont(TTF_Font* font)
{
	if (font == nullptr)
	{
		if (!TTF_WasInit())
			TTF_Init();
	
		if (!font)
			font = TTF_OpenFont("Consolas.ttf", 20);
	
		if (!font)
			printf("TTF_OpenFont: %s\n", TTF_GetError());
	}
}

void BlitSurface(SDL_Surface* src, const SDL_Rect* src_rect, SDL_Texture* dst, SDL_Rect* dstrect)
{
	void* desktop_pixels; int desktop_pitch;
	SDL_LockSurface(src);
	SDL_LockTexture(dst, dstrect, &desktop_pixels, &desktop_pitch);
	Uint32* src_pixels = (Uint32*)src->pixels;
	Uint32* dest_pixels = (Uint32*)desktop_pixels;
	for (int i = 0; i < src->h; ++i)
	{
		memcpy(dest_pixels, src_pixels, src->pitch);
		src_pixels = (Uint32*)((char*)src_pixels + src->pitch);
		dest_pixels = (Uint32*)((char*)dest_pixels + desktop_pitch);
	}
	SDL_UnlockSurface(src);
	SDL_UnlockTexture(dst);
}