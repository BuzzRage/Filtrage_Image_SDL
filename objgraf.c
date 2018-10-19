#include "constantes.h"
#include "objgraf.h"

SDL_Surface * Slider(int w, int h){
	SDL_Surface *slider=SDL_CreateRGBSurface(SDL_HWSURFACE, w, h, 32, 0, 0, 0, 0);
	int rayon = SDL_max(w,h)/4;
	SDL_Rect posPixel;
	SDL_Surface *p = SDL_CreateRGBSurface(SDL_HWSURFACE, 8, 8, 32, 0, 0, 0, 0);
	SDL_FillRect(p, NULL, GRIS);
	SDL_Rect center;
	center.x = 50;
	center.y = 50;
	
	for (i = 0; i < 512; i++){
		posPixel.x = rayon*cos(i * M_PI / 256) + center.x;
		posPixel.y = rayon*sin(i * M_PI / 256) + center.y;
		SDL_BlitSurface(p, NULL, slider, &posPixel);
	}
	
	
}