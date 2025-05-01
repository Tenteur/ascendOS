/*
    TODO: The transition function semi-work (only one time per launch), figure out a way to make it work every time
    TODO: Clean the code and use a bit less the HEAP and a bit more the MALLOC function (for some variables, i don't know).
    TODO: Rewrite completely the transition functions.
    TODO: Remove all the useless comments (code, etc a)
*/

#include <time.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "../include/utils.h"


const SDL_Color SDLRedColor = {255, 0, 0, 255};
const SDL_Color SDLGreenColor = {0, 255, 0, 255};
const SDL_Color SDLBlueColor = {0, 0, 255, 255};
bool isUsingVsync = true; // Not really used. TODO: Will be modified

int main(const int argc, const char *argv[]) {

    // INITIALIZING all the variables that are going to be used. TODO: Maybe clean this area, some variables may be unused.
    int circlePrecision = 100; // TODO: Move this variable
    bool inTransition, isStillInTransition = false; // TODO: Will be removed
    transitionProperties *transitionPtr; // TODO: Will be removed
    long frameStartTime, frameTime; // TODO: Move these variables
    double fps; // TODO: Remove or move these variables
    int roundedFps = 0; // TODO: Remove this variable
    
    SDL_Rect FPSLocation = {650, 0, 150, 200}; // TODO: Move or remove this variable
    SDL_Rect backgroundColorBox = { 200, 150, 400, 300 }; // TODO: Move or remove this variable

    SDL_Rect *whatToMove = &FPSLocation; // TODO: Move or remove this variable

    // Variables definition finished. Initialization of SDL2

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    TTF_Init();
    TTF_Font *TTFSDLFont = TTF_OpenFont("/home/vianney/VSCode/appSDL/static/Roboto-Regular.ttf", 10);
    SDL_Window *window = SDL_CreateWindow("First UI", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0); // SDL_WINDOW_FULLSCREEN
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    AppState state = STATE_LOGIN;

    // SDL2 Initilialized, program running

    // Clean this mess, maybe create a function to remember each input and reactions.
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_RETURN) {
                    state = (state == STATE_LOGIN) ? STATE_HOME : STATE_LOGIN;
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                } else if (event.key.keysym.sym == SDLK_c) {
                    if (whatToMove == &FPSLocation) {
                        whatToMove = &backgroundColorBox;
                    } else {
                        whatToMove = &FPSLocation;
                    }
                } else if (event.key.keysym.sym == SDLK_m) {
                    inTransition = !inTransition;
                    if (inTransition) {
                        printf("Currently in transition\n");
                        transitionPtr = transitionBackgroundColorInit(&SDLRedColor, &SDLBlueColor, (int)240);
                    } else { printf("Currently NOT in transition\n"); }
                } else if(event.key.keysym.sym == SDLK_v) {
                    SDL_DestroyRenderer(renderer);
                    isUsingVsync = !isUsingVsync;
                    if (isUsingVsync) {
                        printf("Using Vsync");
                        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                    } else {
                        printf("NOT using Vsync");
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

        // printf("%d\n", (1000/16) - timeTakenForFrameToDraw);
        char currentFPS[3];
        sprintf(currentFPS, "%u", (int)roundedFps);
        // SDL_Delay((1000/16) - timeTakenForFrameToDraw);
        SDL_Surface *FPSSurface = TTF_RenderText_Blended(TTFSDLFont, currentFPS, SDLBlueColor);
        SDL_Texture *FPSTexture = SDL_CreateTextureFromSurface(renderer, FPSSurface);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen
        SDL_RenderClear(renderer);
        
        if (state == STATE_LOGIN && !inTransition) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for login
        } else if (state == STATE_HOME && !inTransition) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for home
        } else if (inTransition) {
            printf("-");
            isStillInTransition = transitionBackgroundColorNextFrame(renderer, transitionPtr);
            if (!isStillInTransition) {
                inTransition = false;
                printf("Finished transition!\n");
                free(transitionPtr);
            }
        } else { printf("Nothing is happening\n"); }
        
        SDL_RenderFillRect(renderer, &backgroundColorBox);
        SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 100, 100, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_Vertex point1 = {{10, 10}, {255, 0, 0, 255}, {0, 0}};
        SDL_Vertex point2 = {{100, 0}, {0, 255, 0, 255}, {0, 0}};
        SDL_Vertex point3 = {{100, 100}, {0, 0, 255, 255}, {0, 0}};
        SDL_Vertex point4 = {{200, 150}, {255, 255, 255, 255}, {0, 0}};
        SDL_Vertex tempVertex[] = { point1, point2, point3, point1, point4, point2 };
        // if (result == -1) printf("Got an error with the geometry\n");
        // else if (result == 0) printf("Got no error with the geometry\n");
        // else printf("Don't know why i got no result ?\n");
        
        SDL_Color tempColor = {255, 255, 255, 255};
        drawCircleAtCoord(renderer, 100, 100, 100, circlePrecision, tempColor);
        drawCircleAtCoord(renderer, 300, 300, 50, circlePrecision, tempColor);
        SDL_RenderGeometry(renderer, NULL, tempVertex, 3, NULL, 0);
        
        SDL_FreeSurface(tempSurface);
        SDL_DestroyTexture(tempTexture);
        
        SDL_RenderCopy(renderer, FPSTexture, NULL, &FPSLocation);
        SDL_DestroyTexture(FPSTexture);
        SDL_FreeSurface(FPSSurface);


        SDL_RenderPresent(renderer);
        // end = SDL_GetTicks64();
        // timeTakenForFrameToDraw = end - start; // Already in ms (i think) !
        frameTime = SDL_GetTicks() - frameStartTime;
        if (frameTime > 0) {
            fps = 1000.0f / frameTime;
            roundedFps = (int)round(fps);
            // printf("Frame Time: %d ms | FPS: %.2f\n", frameTime, fps);
        }
        // SDL_RenderDrawLines
        // printf("Time taken for drawing a frame: %dms\n", timeTakenForFrameToDraw);
    };

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
