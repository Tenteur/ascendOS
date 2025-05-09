/*
    ! // TODO: The transition function semi-work (only one time per launch), figure out a way to make it work every time
    ! // TODO: Clean the code and use a bit less the HEAP and a bit more the MALLOC function (for some variables, i don't know).
    ! // TODO: Rewrite completely the transition functions.
    ! // TODO: Remove all the useless comments (code, etc a)
*/

#include <time.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../include/text.h"
#include "../include/utils.h"
#include "../include/shapes.h"

SDL_Color SDLRedColor = {255, 0, 0, 255};
SDL_Color SDLGreenColor = {0, 255, 0, 255};
SDL_Color SDLBlueColor = {0, 0, 255, 255};

bool isUsingVsync = true; // ! Not really used. // TODO: Will be modified

int main(const int argc, const char *argv[]) {

    // * INITIALIZING all the variables that are going to be used.
    // ! // FIXME: Maybe clean this area, some variables may be unused.
    int circlePrecision = 100; // ? // TODO: Move this variable
    long frameStartTime, frameTime; // ? // TODO: Move these variables
    double fps; // ? // TODO: Remove or move these variables
    char currentFPS[3] = "";
    // int roundedFps = 0; // ! // TODO: Remove this variable
    
    SDL_Rect FPSLocation = {650, 0, 150, 200}; // ? // TODO: Move or remove this variable
    SDL_Rect backgroundColorBox = { 200, 150, 400, 300 }; // ? // TODO: Move or remove this variable

    SDL_Rect *whatToMove = &FPSLocation; // ? // TODO: Move or remove this variable

    // * Variables definition finished. Initialization of SDL2
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    // TTF_Init();
    // utilsInit();
    textInit();

    struct SDL_DisplayMode screenInfo;
    SDL_GetCurrentDisplayMode(0, &screenInfo);
    printf("%u, %u, %u\n", screenInfo.w, screenInfo.h, screenInfo.refresh_rate);
    
    SDL_Window *window = SDL_CreateWindow("First UI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0); // SDL_WINDOW_FULLSCREEN
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    bool running = true;
    AppState state = STATE_LOGIN;
    
    // * SDL2 Initilialized, program running

    signed int success = addFontToList("Roboto-Regular", 10);
    printf("%u\n", success);
    success = addFontToList("BebasNeue-Regular", 50);
    printf("%u\n", success);
    success = addFontToList("CascadiaMono-Regular", 100);
    printf("%u\n", success);
    success = addFontToList("DancingScript-Regular", 200);
    printf("%u\n", success);
    // ! // TODO: Clean this mess, maybe create a function to remember each input and reactions.
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    state = (state == STATE_LOGIN) ? STATE_HOME : STATE_LOGIN;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    break;
                } else if (event.key.keysym.sym == SDLK_c) {
                    if (whatToMove == &FPSLocation) {
                        whatToMove = &backgroundColorBox;
                    } else {
                        whatToMove = &FPSLocation;
                    }
                } else if (event.key.keysym.sym == SDLK_m) {
                    printf("Funtion removed :'(\n");
                    // inTransition = !inTransition;
                    // if (inTransition) {
                    //     printf("Currently in transition\n");
                    //     transitionPtr = transitionBackgroundColorInit(&SDLRedColor, &SDLBlueColor, 240);
                    // } else { printf("Currently NOT in transition\n"); }
                } else if(event.key.keysym.sym == SDLK_v && false) {
                    SDL_DestroyRenderer(renderer);
                    isUsingVsync = !isUsingVsync;
                    if (isUsingVsync) {
                        printf("Using Vsync\n");
                        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                    } else {
                        printf("NOT using Vsync\n");
                        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
                    }
                } else if (event.key.keysym.sym == SDLK_o) {
                    circlePrecision += 1;
                } else if (event.key.keysym.sym == SDLK_l) {
                    circlePrecision -= 1;
                }
                else if (event.key.keysym.sym == SDLK_LEFT) {
                    whatToMove->x -= 1;
                } else if (event.key.keysym.sym == SDLK_RIGHT) {
                    whatToMove->x += 1;
                } else if (event.key.keysym.sym == SDLK_UP) {
                    whatToMove->y -= 1;
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    whatToMove->y += 1;
                } else if (event.key.keysym.sym == SDLK_q) {
                    whatToMove->w -= 1;
                } else if (event.key.keysym.sym == SDLK_d) {
                    whatToMove->w += 1;
                } else if (event.key.keysym.sym == SDLK_z) {
                    whatToMove->h -= 1;
                } else if (event.key.keysym.sym == SDLK_s) {
                    whatToMove->h += 1;
                }
            }
        };
        
        frameStartTime = SDL_GetTicks64();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen
        SDL_RenderClear(renderer);
        
        if (state == STATE_LOGIN) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for login
        } else if (state == STATE_HOME) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for home
        } 
        // else if (inTransition) {
        //     printf("-");
        //     isStillInTransition = transitionBackgroundColorNextFrame(renderer, transitionPtr);
        //     if (!isStillInTransition) {
        //         inTransition = false;
        //         free(transitionPtr);
        //     }
        // }
        
        SDL_RenderFillRect(renderer, &backgroundColorBox);

        // This is the RGB triangle
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 100, 100, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_Vertex point1 = {{10, 10}, {255, 0, 0, 255}, {0, 0}};
        SDL_Vertex point2 = {{100, 0}, {0, 255, 0, 255}, {0, 0}};
        SDL_Vertex point3 = {{100, 100}, {0, 0, 255, 255}, {0, 0}};
        SDL_Vertex point4 = {{200, 150}, {255, 255, 255, 255}, {0, 0}};
        SDL_Vertex tempVertex[] = { point1, point2, point3, point1, point4, point2 };
        SDL_RenderGeometry(renderer, NULL, tempVertex, 3, NULL, 0);
        
        SDL_FreeSurface(tempSurface);
        SDL_DestroyTexture(tempTexture);

        SDL_Color tempColor = {255, 255, 255, 255};
        drawCircleAtCoord(renderer, 100, 100, 100, circlePrecision, tempColor);
        drawCircleAtCoord(renderer, 300, 300, 50, circlePrecision, tempColor);
        
        
        SDL_Rect tempRect = {100, 100, 200, 200};
        selectFontFromList("Roboto-Regular", 100);
        signed int success = renderTextAtCoord(renderer, "Hello world!", 100, 100, &SDLGreenColor, true, &tempRect);
        selectFontFromList("BebasNeue-Regular", 50);
        renderTextAtCoord(renderer, "Hello world!", 100, 100, &SDLGreenColor, false, NULL);
        selectFontFromList("DancingScript-Regular", 200);
        renderTextAtCoord(renderer, "world!", 500, 100, &SDLBlueColor, false, NULL);
        selectFontFromList("CascadiaMono-Regular", 100);
        renderTextAtCoord(renderer, currentFPS, FPSLocation.x, FPSLocation.y, &SDLBlueColor, false, NULL);

        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStartTime;
        if (frameTime > 0) {
            fps = 1000.0f / frameTime;
            sprintf(currentFPS, "%u", (int)round(fps));
        }
    };

    // ? Clear (mostly) everything of the system
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    utilsQuit();
    SDL_Quit();
    return 0;
}
