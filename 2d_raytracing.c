#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

//IMPORTANT: USE GPU OR CPU RENDERING BASED ON YOU HW
//1 to enable GPU rendering (faster but you have to have properer HW)
//0 o enable CPU in-software rendering
#define USE_GPU 1

//window size
#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 900

//Rays settings
#define CIRCLE0_RAYS_COUNT  2000         //Number of rays casted (the higher the better, but it's more hw consuming)
#define CIRCLE0_RAYS_OFFSET_COUNT 0.5 * CIRCLE0_RAYS_COUNT
#define CIRCLE0_RAYS_ANGLE_COVERED 2*M_PI  //360: Rays are casted in all directions. The direction angle starts at 0 degrees

/*
Objects Hierarchy
+ Circle0 -> Light Source
+ Circle1 -> Sample Light absorbing object

*/
#define MAX_OBJECTS 10

//Circle0 default characteristics
#define CIRCLE0_XS 200
#define CIRCLE0_YS 200
#define CIRCLE0_RS  20

//Circle1 default characteristics
#define CIRCLE1_XS  450
#define CIRCLE1_YS  450
#define CIRCLE1_RS 100


//colors
SDL_Color WHITE = (SDL_Color) {255, 255, 255, 255}; // {R,G,B,A}
SDL_Color BKG = (SDL_Color) {0, 0, 0, 255};

//Frequence to update screen
#define FPS 60

//structs
typedef struct {
    float startX, startY;           //ray start coordinates
    double m;                        //ray angle in radiant
} Ray;

typedef struct {
    float centerX, centerY;         //Circle center
    float rad;                     //circle radius
} Circle;



/* Funzioni custom */

//Gfx functions
void drawCircle(SDL_Renderer* renderer, Circle circle, SDL_Color color);

//Basic Geometry functions
float distanceSquared(float aX, float aY, float bX, float bY);
int belongsToCircle(Circle circle, float aX, float aY);

//RayCasting functions
void genRays_Circle(Circle sourceCircle, Ray *raysArray, int raysCount, int angleDirections);
void castRays_CircleToCircles(SDL_Renderer* renderer, SDL_Color rayColor, Circle sourceCircle, Ray *raysArray, int raysCount, Circle* otherObjects, int numOtherObjects);



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
    #if USE_GPU
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

    //Rays arrays for the light sources
    Ray raysCircle0[CIRCLE0_RAYS_COUNT];    //uninitialized




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


        //clear window to redraw objects at each frame 
        SDL_SetRenderDrawColor(renderer, BKG.r, BKG.g, BKG.b, BKG.a); //clean the whole screen       
        SDL_RenderClear(renderer);
        
        //draw Rays
        //it's better to draw the rays first since in case of cicles they originated at its center

        //set the array with other objects in the space
        Circle otherCircles[MAX_OBJECTS] = {circle1};

        //Generate Rays starting points and angles
        genRays_Circle(circle0, raysCircle0, CIRCLE0_RAYS_COUNT, CIRCLE0_RAYS_ANGLE_COVERED);

        //trace rays
        castRays_CircleToCircles(renderer, WHITE, circle0, raysCircle0,CIRCLE0_RAYS_COUNT,otherCircles, 1);

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

void genRays_Circle(Circle sourceCircle, Ray *raysArray, int raysCount, int angleDirections){
    int i;
    double baseAngle = (double)angleDirections / (double)(raysCount - CIRCLE0_RAYS_OFFSET_COUNT);
    for (i = 0; i < raysCount; i++){
        Ray r = {sourceCircle.centerX, sourceCircle.centerY, ((double)i)*baseAngle};
        raysArray[i] = r;
        //printf("ray: %f\n", raysArray[i].m);
    }
}

void castRays_CircleToCircles(SDL_Renderer* renderer, SDL_Color rayColor, Circle sourceCircle, Ray *raysArray, int raysCount, Circle* otherObjects, int numOtherObjects){
    int i, j;
    int endScreen;
    int collision;
    int step;
    float nextX, nextY, lastX, lastY;

    //Set rayColor
    SDL_SetRenderDrawColor(renderer, rayColor.r, rayColor.g, rayColor.b, rayColor.a);

    for (i = 0; i < raysCount; i++){
        Ray r = raysArray[i];

        //Calculate actual ray end coordinates
        //These are either end of screen or less if we have a collision between the ray and other objects

        collision = 0;
        endScreen = 0;
        step = 1;
        lastX = r.startX;
        lastY = r.startY;

        while (!collision && !endScreen){
            //Calculate next coordinates of the next pixel composing the ray
            nextX = lastX + step*cos(r.m);
            nextY = lastY + step*sin(r.m);

            //check end of screen for the ray
            if (nextX < 0 || nextX > WINDOW_WIDTH || nextY < 0 || nextY > WINDOW_HEIGHT) {
                endScreen = 1;
            } 

            //check collision with other circles

            for (j = 0; j < numOtherObjects; j++){
                if (belongsToCircle(otherObjects[j], nextX, nextY)){
                    collision = 1;
                    break;  //no sense searching for multiple collisions since tha ray is stopped at the first one
                }
            }

            ++step;

            //Update lastX and lastY
            lastX = nextX;
            lastY = nextY;
    
        }

        //Now I know the ray starts at the center of the sourceCircle and stops at (lastX, lastY)
        //I have to draw it
        SDL_RenderDrawLine(renderer, 
            (int)raysArray[i].startX, (int)raysArray[i].startY, 
            (int)lastX, (int)lastY);

    }



}