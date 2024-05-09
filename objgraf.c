#include <SDL2/SDL.h>
#include <math.h>
#include "constantes.h"
#include "objgraf.h"

SDL_Surface* Slider(int w, int h, SDL_Rect *pos0){
	int i, rayon = SDL_min(w,h)/16;
	SDL_Rect posPixel, posCursor, posSlider;
	posCursor.x = w/2;//   + pos0->x;
	posCursor.y = h*5/6;// + pos0->y;
	
	posSlider.x = (w/2) - w/20;
	posSlider.y = h/6;

	SDL_Surface *bg		 =SDL_CreateRGBSurface(0, w,    h,     32, 0, 0, 0, 0);
	SDL_Surface *slider=SDL_CreateRGBSurface(0, w/10, h*2/3, 32, 0, 0, 0, 0);
	SDL_FillRect(slider,NULL,SDL_MapRGB(slider->format,255,255,255));
	SDL_FillRect(bg,NULL,SDL_MapRGB(bg->format,30,30,30));
	SDL_BlitSurface(slider,NULL,bg,&posSlider);

// 	SDL_Surface *p = SDL_CreateRGBSurface(0, 8, 8, 32, 0, 0, 0, 0);
// 	SDL_FillRect(p, NULL, SDL_MapRGB(bg->format,255,255,0));
// 	for (i = 0; i < 512; i++){
// 		posPixel.x = rayon*cos(i*M_PI / 256) + posCursor.x;
// 		posPixel.y = rayon*sin(i*M_PI / 256) + posCursor.y;
// 		SDL_BlitSurface(p, NULL, bg, &posPixel);
// 	}
	

	SDL_Surface *p = SDL_CreateRGBSurface(0, 16, 16, 32, 0, 0, 0, 0);
	SDL_FillRect(p, NULL, SDL_MapRGB(bg->format,255,255,0));
	posPixel.x = w/2 - 7;
	posPixel.y = h*5/6 - 7;
	SDL_BlitSurface(p, NULL, bg, &posPixel);

	return bg;

}
