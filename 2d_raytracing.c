#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>


//window size
#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1600

//colors
#define WHITE 0xffffffff


//structs


typedef struct {
    float centerX, centerY;         //Circle center
    float rad;                     //circle radius
} Circle;



//Funzioni custom
void drawCircle(SDL_Surface* surface , Circle circle, Uint32 color);
float distanceSquared(float aX, float aY, float bX, float bY);
int belongsToCircle(Circle circle, float aX, float aY);

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
    
    SDL_Rect rect = (SDL_Rect) {200, 200, 200, 200};
    SDL_FillRect(surface, &rect, WHITE);
    */
    
    Circle circle0 = {200, 200, 20};
    drawCircle(surface, circle0, WHITE);
    
    
    
    
    
    //aggiorna la window
    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);

    return 0;
}


void drawCircle(SDL_Surface* surface , Circle circle, Uint32 color){
    /*
    Function do draw and fill a circle on the surface   
    */


    float x, y;

    //Just loop through the rectangle containing the circle to understand which points of it actually belong to the circle too!
    for (x = circle.centerX - circle.rad; x <= circle.centerX + circle.rad; x++){
        for (y = circle.centerY - circle.rad; y <= circle.centerY + circle.rad; y++){
            //if the pixel located at (x,y) belongs the circle, draw it
            if (belongsToCircle(circle, x, y)){
                SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
                SDL_FillRect(surface, &pixel, WHITE);
            }
        }
    }  

}


float distanceSquared(float aX, float aY, float bX, float bY){
    /* Return Euclidean distance squared*/

    return pow(aX - bX, 2) + pow(aY - bY, 2);
}


int belongsToCircle(Circle circle, float aX, float aY){
    /* Boolean Function, return 1 if point a belongs to Circle circle, otherwise return 0*/

    float radSquared = pow(circle.rad, 2);
    if (distanceSquared(aX, aY, circle.centerX, circle.centerY) <= radSquared) return 1;

    return 0;

}