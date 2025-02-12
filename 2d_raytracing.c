#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

//IMPORTANT: USE GPU OR CPU RENDERING BASED ON YOU HW
//1 to enable GPU rendering (faster but you have to have properer HW)
//0 o enable CPU in-software rendering
#define USE_GPU 1

//window size
#define WINDOW_HEIGHT 900
#define WINDOW_WIDTH 1600

/*
Objects Hierarchy
+ Circle0 -> Light Source
+ Circle1 -> Sample Light absorbing object

*/

//Circle0 default characteristics
#define CIRCLE0_XS 200
#define CIRCLE0_YS 200
#define CIRCLE0_RS  20

//Circle1 default characteristics
#define CIRCLE1_XS  1200
#define CIRCLE1_YS  450
#define CIRCLE1_RS 100


//colors
SDL_Color WHITE = (SDL_Color) {255, 255, 255, 255}; // {R,G,B,A}
SDL_Color BKG = (SDL_Color) {0, 0, 0, 255};

//Frequence to update screen
#define FPS 60

//structs


typedef struct {
    float centerX, centerY;         //Circle center
    float rad;                     //circle radius
} Circle;



//Funzioni custom
void drawCircle(SDL_Renderer* renderer, Circle circle, SDL_Color color);
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
    if (!window) {
        printf("Errore durante la creazione della finestra: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //Crea Renderer
    #ifdef USE_GPU
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    #else
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    #endif
    if (!renderer) {
        printf("Errore durante la creazione del renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /*
    sintassi SDL_FillRect:
    SDL_FillRect(surface, &rect, color)
    rect è il puntatore ad un oggetto SDL_Rect
    SDL_Rect lo ottieni castando a SDL_Rect un array del tipo {x, y, width, heigth}
    
    SDL_Rect rect = (SDL_Rect) {200, 200, 200, 200};
    SDL_FillRect(surface, &rect, WHITE);
    */
    
    int redraw = 1;
    int delayTime = 1 / FPS; //1 sec / 60 FPS = ca 17 ms
    SDL_Event event;

    //Object Hierarchy
    Circle circle0 = {CIRCLE0_XS, CIRCLE0_YS, CIRCLE0_RS};
    Circle circle1 = {CIRCLE1_XS, CIRCLE1_YS, CIRCLE1_RS};

    while (redraw){
    

        //read user triggered events
        while(SDL_PollEvent(&event)){
            //events written into the event variable
            if (event.type == SDL_QUIT){
                //SDL_QUIT is triggered when users closes the app window
                redraw = 0;
                break;
            }

            if (event.type == SDL_MOUSEMOTION){
                //SDL_MOUSEMOTION triggered when the users moves the mouse

                //if the right mouse button is clicked, redraw the circle at (mouseX, mouseY)
                if (event.motion.state & SDL_BUTTON_RMASK){
                    Sint32 mouseX = event.motion.x;
                    Sint32 mouseY = event.motion.y;

                    circle0.centerX = (float) mouseX;
                    circle0.centerY = (float) mouseY;
                }
            }
        }


        //redraw objects at each frame 
        SDL_SetRenderDrawColor(renderer, BKG.r, BKG.g, BKG.b, BKG.a); //clean the whole screen       
        SDL_RenderClear(renderer);
        
        //draw objects
        drawCircle(renderer, circle0, WHITE);
        drawCircle(renderer, circle1, WHITE);


        //aggiorna la window
        SDL_RenderPresent(renderer);

        //FPS
        SDL_Delay(delayTime);
    
    }
    
    
    
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void drawCircle(SDL_Renderer* renderer , Circle circle, SDL_Color color){
    /*
    Function do draw and fill a circle on the surface   
    */


    float x, y;

    //Set circle color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    //Just loop through the rectangle containing the circle to understand which points of it actually belong to the circle too!
    for (x = circle.centerX - circle.rad; x <= circle.centerX + circle.rad; x++){
        for (y = circle.centerY - circle.rad; y <= circle.centerY + circle.rad; y++){
            //if the pixel located at (x,y) belongs the circle, draw it
            if (belongsToCircle(circle, x, y)){
            
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
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