#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constantes.h"
#include "objgraf.h"
#include "traitements.h"

void pause(SDL_Surface *Image, SDL_Renderer *renderer, SDL_Texture *screenTexture, SDL_Surface *screen);
void init(struct verrou v); // A d�finir pour all�ger la lecture du code

struct verrou v;

/*
 * Notes pour conversion Suface vers Texture:
 *   - IMG_Load() devient IMG_LoadTexture(renderer, file)
 *   - Acc�s aux param�tres via SDL_QueryTexture(Image, &formatImage, NULL, &rectImage.w, &rectImage.h);
 *   - Modifier les formats d'image (MapRGB dans constantes.h)
 */

int main(int argc, char *argv[]){
	v.chargementOK=0;
	//     freopen("CON", "w", stdout); // Redirection de la sortie (autorisation en �criture) sur console
	//     freopen("CON", "r", stdin);  // Redirection de l'entr�e (autorisation en lecture) sur console
	//     freopen("CON", "w", stderr); // Redirection de la sortie d'erreur (autorisation en �criture) sur console
	SDL_Surface *screen		=NULL;
	SDL_Renderer *renderer	=NULL;
	SDL_Surface *Image		=NULL;
	SDL_Rect rectImage;
	rectImage.x=0;
	rectImage.y=0;
	SDL_PixelFormat *formatImage = NULL;

	char nom_fichier[50];
	SDL_Init(SDL_INIT_VIDEO);

	if(argc > 1){
		if(Image = IMG_Load(argv[1])){
			v.chargementOK=1;
			formatImage = Image->format;
			rectImage.w = Image->w;
			rectImage.h = Image->h;
		}
	}

	while(!v.chargementOK){
		printf("Vous voulez chargez quelle image ? (ne pas depasser 50 caracteres)\nNom: ");
		scanf("%s",&nom_fichier);
		Image= IMG_Load(nom_fichier);
		if(Image){
			v.chargementOK=1;
			formatImage = Image->format;
			rectImage.w = Image->w;
			rectImage.h = Image->h;
		}
		else
			printf("\nErreur de chargement, verifiez le nom de l'image.\n");
	}

	SDL_Window *window = SDL_CreateWindow("Traitement d'image", 
								SDL_WINDOWPOS_CENTERED, 
								SDL_WINDOWPOS_CENTERED, 
								2*rectImage.w+LARGEUR_SEUIL, rectImage.h, 
								0);
	renderer = SDL_CreateRenderer(window, -1, 0);
	screen = SDL_CreateRGBSurface(0, 2*rectImage.w+LARGEUR_SEUIL, rectImage.h, 32, 
                                        0x00FF0000, 
                                        0x0000FF00, 
                                        0x000000FF, 
                                        0xFF000000);
	SDL_Texture *screenTexture = SDL_CreateTexture(renderer, 
                                            SDL_PIXELFORMAT_ARGB8888, 
                                            SDL_TEXTUREACCESS_STREAMING, 
											2*rectImage.w+LARGEUR_SEUIL, rectImage.h);

	
	SDL_BlitSurface(Image, NULL, screen, &rectImage);
	SDL_UpdateTexture(screenTexture, NULL, screen->pixels, screen->pitch); 
	SDL_RenderClear(renderer); 
	SDL_RenderCopy(renderer, screenTexture, NULL, NULL); 
	SDL_RenderPresent(renderer);
	
	#ifdef INFO
	printf("\nImage format: %s.",formatImage);
	#endif
	
	pause(Image, renderer, screenTexture, screen);

	SDL_Quit();

	return EXIT_SUCCESS;
}

/* Usage:
-1: Demande de changement de couleur 1 (couleur_down)
-2: Demande de changement de couleur 2 (couleur_up)

-b: binarisation (seuillage param�trable)
-i: inversion (n�gatif)
-n: transformation en niveaux de gris (noir et blanc)
-q: quantification (avec nb de bits param�trable)
*/

void pause(SDL_Surface *Image, SDL_Renderer *renderer, SDL_Texture *screenTexture, SDL_Surface *screen){
	int continuer = 1,seuil=0,nb_bits=3,nb_couleurs=1,i=0;
	SDL_Event event;
	SDL_Surface *Image2, *S_seuil=NULL, *S_quant[64];
	SDL_Rect rectImage, rectImg2, rectSeuil, rect_src;
	Uint8 *keystate = NULL;
	
	SDL_PixelFormat *formatImage = 0;
	Uint32 couleur_up, couleur_down;
	Uint32 couleurs[64],couleur; // 64 couleurs => 6 bits

	formatImage = Image->format;
	rectImage.w = Image->w;
	rectImage.h = Image->h;

	// Affectations
	S_seuil=SDL_CreateRGBSurface(0, LARGEUR_SEUIL, rectImage.h, 32, 0, 0, 0, 0);	
	Image2 =SDL_CreateRGBSurface(0, rectImage.w, rectImage.h, 32, 0, 0, 0, 0);

	
	rect_src.x = 0;
	rect_src.y = 0;
	rectImg2.x=rectImage.w;
	rectImg2.y=0;
	rectSeuil.x=2*rectImage.w;
	rectSeuil.y=0;

	couleur_up	= BLANC;
	couleur_down= NOIR;

	couleurs[0]=NOIR;
	couleurs[1]=BLEU;
	couleurs[2]=CYAN;
	couleurs[3]=VERT;
	couleurs[4]=JAUNE;
	couleurs[5]=ORANGE;
	couleurs[6]=ROUGE;
	couleurs[7]=BLANC;

	for(i=0;i<nb_bits;i++){
		nb_couleurs*=2;
	}

	printf("%d %d %d",couleurs[3],SDL_MapRGB(screen->format,255,0,0),formatImage);
	v.binarisation=0; v.quantification=0; v.seuil_haschanged=0; v.lux=0, v.ctst=0;

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				continuer = 0;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym){
					case SDLK_d:
						Image2= detourage(Image);
						break;
					case SDLK_i:
						Image2= inv_img(Image);
						break;
					case SDLK_l:
						seuil=127;
						if(v.lux)
							v.lux=0;
						else
							v.lux=1;
						Image2= lux_img(Image,seuil);
						break;
					case SDLK_c:
						seuil=127;
						if(v.ctst)
							v.ctst=0;
						else
							v.ctst=1;
						Image2= contrast_img(Image,seuil);
						break;						
					case SDLK_m:
						Image2= filtre_moyenneur(Image);
						break;
					case SDLK_n:
						Image2= ng_img(Image);
						break;
					case SDLK_b: // Reset du seuil de binarisation
						seuil=128;
						if(v.binarisation){
							v.binarisation=0;
							SDL_BlitSurface(Image,NULL,Image2,&rect_src);
						}
						else{
							v.binarisation=1;
							Image2= bin_img(Image,seuil,couleur_down,couleur_up);
						}
						break;
					case SDLK_q:
						if(v.quantification)
							v.quantification=0;
						else
							v.quantification=1;
						for(i=0;i<4;i++){
							printf("\n====== Couleurs %d: %2x",i,couleurs[i]);
						}
						Image2= quant_img(Image,seuil,nb_bits,couleurs);
						break;
					case SDLK_s:
						SDL_SaveBMP(Image2,"save.bmp");
						printf("\n\nImage sauvegardee\n\n");
						break;
					case SDLK_DOWN:
						if(v.binarisation){
							if(seuil > 0){
								seuil--;
								v.seuil_haschanged=1;
							} else if(seuil == 0){
								seuil=255;
								v.seuil_haschanged=1;
							}
							Image2= bin_img(Image,seuil,couleur_down,couleur_up);
						}
						else if(v.lux){
							if(seuil > 0){
								seuil--;
								v.seuil_haschanged=1;
							} else if(seuil==0){
								seuil=255;
								v.seuil_haschanged=1;
							}
							Image2= lux_img(Image,seuil);
						}
						else if(v.ctst){
							if(seuil > 0){
								seuil--;
								v.seuil_haschanged=1;
							} else if(seuil==0){
								seuil=255;
								v.seuil_haschanged=1;
							}
							Image2= contrast_img(Image,seuil);
						}
						break;
					case SDLK_UP:
						if(v.binarisation){
							if(seuil < 255){
								seuil++;
								v.seuil_haschanged=1;
							} else if(seuil==255){
								seuil=0;
								v.seuil_haschanged=1;
							}
								Image2= bin_img(Image,seuil,couleur_down,couleur_up);
						}
						else if(v.lux){
							if(seuil < 255){
								seuil++;
								v.seuil_haschanged=1;
							} else if(seuil==255){
								seuil=0;
								v.seuil_haschanged=1;
							}
								Image2= lux_img(Image,seuil);
						}
						else if(v.ctst){
							if(seuil < 255){
								seuil++;
								v.seuil_haschanged=1;
							} else if(seuil==255){
								seuil=0;
								v.seuil_haschanged=1;
							}
							Image2= contrast_img(Image,seuil);
						}
						break;
				}
				break;
			case SDL_KEYUP:
				if(event.key.keysym.sym==SDLK_b){

				}
				break;
		}
		
		//        keystate = SDL_GetKeyState(NULL);
		//        if(keystate[SDLK_b] && keystate[SDLK_UP] || keystate[SDLK_b] && keystate[SDLK_DOWN]){
		//            SDL_BlitSurface(Image2, NULL, screen, &posImg2);
		//            SDL_Flip(screen);
		//        }
		
		if(v.seuil_haschanged)
			printf("Seuil:%d %2x\n",seuil,seuil);
		v.seuil_haschanged=0;
		if(v.binarisation||v.lux){
			SDL_FillRect(S_seuil, NULL, SDL_MapRGB(S_seuil->format, seuil, seuil, seuil));
			SDL_BlitSurface(S_seuil,NULL,screen,&rectSeuil);
		}
		else if(v.ctst){
			S_seuil = Slider(S_seuil->w,S_seuil->h,&rectSeuil);
			SDL_BlitSurface(S_seuil,NULL,screen,&rectSeuil);			
		}
		else if(v.quantification){
			for(i=0;i<nb_couleurs;i++){ // Palette de couleur
				S_quant[i]=SDL_CreateRGBSurface(0, LARGEUR_SEUIL, Image->h/nb_couleurs, 32, 0, 0, 0, 0);
				SDL_FillRect(S_quant[i], NULL, couleurs[i]);
				SDL_BlitSurface(S_quant[i], NULL, screen, &rectSeuil);
				rectSeuil.y+=(Image->h)/nb_couleurs;
			}
			rectSeuil.y=0;
		}
		else{
			SDL_FillRect(S_seuil, NULL, SDL_MapRGB(S_seuil->format, 0, 0, 0));
			SDL_BlitSurface(S_seuil,NULL,screen,&rectSeuil);			
		}
		SDL_BlitSurface(Image2, NULL, screen, &rectImg2);
		
		SDL_UpdateTexture(screenTexture, NULL, screen->pixels, screen->pitch); 
		SDL_RenderClear(renderer); 
		SDL_RenderCopy(renderer, screenTexture, NULL, NULL); 
		SDL_RenderPresent(renderer);
	}
}
