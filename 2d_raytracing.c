#include <SDL2/SDL.h>
#include <stdio.h>

//window size
#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1600

//colors
#define WHITE 0xffffffff

int main(int argc, char* argv[]) {
    //inizializzazione di SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Errore durante l'inizializzazione di SDL: %s\n", SDL_GetError());
        return 1;
    }

    //creazione window
    SDL_Window* window = SDL_CreateWindow("2d Raytracing attempt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    //prendi la surface per disegnare
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    /*
    sintassi SDL_FillRect:
    SDL_FillRect(surface, &rect, color)
    rect è il puntatore ad un oggetto SDL_Rect
    SDL_Rect lo ottieni castando a SDL_Rect un array del tipo {x, y, width, heigth}
    */

    SDL_Rect rect = (SDL_Rect) {200, 200, 200, 200};
    SDL_FillRect(surface, &rect, WHITE);
    
    //aggiorna la window
    SDL_UpdateWindowSurface(window);
    
    
    SDL_Delay(5000);

    return 0;
}