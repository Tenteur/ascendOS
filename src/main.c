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
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../include/text.h"
#include "../include/utils.h"
#include "../include/shapes.h"
#include "../include/images.h"
#include "../include/render_manager.h"
#include "../include/interface_handler.h"

#include <unistd.h>
#include <stdio.h>
// #include <libxml/tree.h>
#include <linux/limits.h>
// #include <libxml/parser.h>

SDL_FColor SDLRedColor = {255, 0, 0, 255};
SDL_FColor SDLGreenColor = {0, 255, 0, 255};
SDL_FColor SDLBlueColor = {0, 0, 255, 255};

int main(const int argc, const char *argv[]) {

    // * INITIALIZING all the variables that are going to be used.
    // ! // FIXME: Maybe clean this area, some variables may be unused.

    long frameStartTime, frameTime; // ? // TODO: Move these variables
    double fps; // ? // TODO: Remove or move these variables
    char currentFPS[3] = "";
    
    SDL_Rect FPSLocation = {650, 0, 150, 200}; // ? // TODO: Move or remove this variable
    SDL_Rect backgroundColorBox = { 200, 150, 400, 300 }; // ? // TODO: Move or remove this variable

    SDL_Rect *whatToMove = &FPSLocation; // ? // TODO: Move or remove this variable

    char cwd[500];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    // * Variables definition finished. Initialization of SDL3
    
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD);
    
    // struct SDL_DisplayMode screenInfo;
    // SDL_GetCurrentDisplayMode(&screenInfo);
    // printf("%u, %u, %u\n", screenInfo.w, screenInfo.h, screenInfo.refresh_rate);
    
    SDL_Window *window = SDL_CreateWindow("First UI", 800, 600, 0); // SDL_WINDOW_FULLSCREEN
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    unsigned int hi = SDL_GetWindowPixelFormat(window);
    char *hiName = (char*)SDL_GetPixelFormatName(hi);
    printf("%u, %s\n", hi, hiName);

    unsigned int numberOfSpacePressed = 0;

    textInit();
    imagesInit();
    render_managerInit(renderer);
    interface_handlerInit(renderer);
    // char template[15] = "example.jsonUI";
    // char empty[15] = "interface.xml";
    // interface_handlerGenerateScene(interface_handlerLoadScene(empty));
    
    
    bool running = true;
    AppState state = STATE_LOGIN;
    
    // * SDL2 Initialized, program running
    
    addFontToList("Roboto-Regular", 10);
    signed int resultFont = addFontToList("comic", 100);
    resultFont = resultFont | selectFontFromList("comic", 100);
    printf("font: %i\n", resultFont);
    // addFontToList("BebasNeue-Regular", 50);
    // addFontToList("CascadiaMono-Regular", 100);
    // const char *hie = "hello!";
    // const char *hier = "oo";
    
    // addFontToList("DancingScript-Regular", 200);
    
    // ! // TODO: Clean this mess, maybe create a function to remember each input and reactions.
    
    const char *hell = "/mnt/DataDisk/ascendOS/templates/basic.yml";
    parse_yaml(hell);
    
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            // printf("%u\n", );
            if (event.type == SDL_EVENT_QUIT) running = false;
            if (1) {
                if(event.key.key == SDLK_SPACE) {

                    numberOfSpacePressed += 1;
                    // char fuck[50] = "/mnt/DataDisk/ascendOS/templates/basic.yml";
                    printf("Parsing yaml file with space press number: %u\n", numberOfSpacePressed);
                    render_managerDeleteScene();
                    parse_yaml(hell);
                    // SDL_Texture *secondText = generateTextTexture(renderer, hier, &SDLRedColor, 0, 0);
                    // SDL_Texture *firstText = generateTextTexture(renderer, hie, &SDLRedColor, 0, 0);
                    // printf("text success: %i\n", firstText && secondText);
                    // render_managerAddItemToDraw(2, 50, firstText, NULL, 100, 100, 239, 118);
                    // render_managerAddItemToDraw(2, 100, firstText, NULL, 100, 100, 239, 118);
                    // render_managerAddItemToDraw(2, 200, secondText, NULL, 200, 400, 114, 118);
                }
                if (event.key.key== SDLK_RETURN) {
                    state = (state == STATE_LOGIN) ? STATE_HOME : STATE_LOGIN;
                } else if (event.key.key == SDLK_ESCAPE) {
                    running = false;
                    break;
                } else if (event.key.key == SDLK_C) {
                    if (whatToMove == &FPSLocation) {
                        whatToMove = &backgroundColorBox;
                    } else {
                        whatToMove = &FPSLocation;
                    }
                } else if (event.key.key == SDLK_H) {
                    printf("Transition function has been removed since it is deprecated and do not work\n");
                } else if(event.key.key == SDLK_V) {
                    printf("Removed ability to disable VSync\n");
                } else if (event.key.key == SDLK_O) {
                    printf("Modifying circle precision has been removed\n");
                } else if (event.key.key == SDLK_L) {
                    printf("Modifying circle precision has been removed\n");
                }
                else if (event.key.key == SDLK_LEFT) {
                    whatToMove->x -= 1;
                } else if (event.key.key == SDLK_RIGHT) {
                    whatToMove->x += 1;
                } else if (event.key.key == SDLK_UP) {
                    whatToMove->y -= 1;
                } else if (event.key.key == SDLK_DOWN) {
                    whatToMove->y += 1;
                } else if (event.key.key == SDLK_G) {
                    whatToMove->w -= 1;
                } else if (event.key.key == SDLK_D) {
                    whatToMove->w += 1;
                } else if (event.key.key == SDLK_Z) {
                    whatToMove->h -= 1;
                } else if (event.key.key == SDLK_S) {
                    whatToMove->h += 1;
                }
            }
        };
        
        frameStartTime = SDL_GetTicks();


        



        // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear screen
        // SDL_RenderClear(renderer);
        
        // if (state == STATE_LOGIN) {
        //     SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for login
        // } else if (state == STATE_HOME) {
        //     SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green for home
        // } 
        // else if (inTransition) {
        //     printf("-");
        //     isStillInTransition = transitionBackgroundColorNextFrame(renderer, transitionPtr);
        //     if (!isStillInTransition) {
        //         inTransition = false;
        //         free(transitionPtr);
        //     }
        // }
        
        // SDL_RenderFillRect(renderer, &backgroundColorBox);

        // // This is the RGB triangle
        // SDL_Surface *tempSurface = SDL_CreateRGBSurface(0, 100, 100, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
        // SDL_Texture *tempTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
        // SDL_Vertex point1 = {{10, 10}, {255, 0, 0, 255}, {0, 0}};
        // SDL_Vertex point2 = {{100, 0}, {0, 255, 0, 255}, {0, 0}};
        // SDL_Vertex point3 = {{100, 100}, {0, 0, 255, 255}, {0, 0}};
        // SDL_Vertex point4 = {{200, 150}, {255, 255, 255, 255}, {0, 0}};
        // SDL_Vertex tempVertex[] = { point1, point2, point3, point1, point4, point2 };
        // SDL_RenderGeometry(renderer, NULL, tempVertex, 3, NULL, 0);
        
        // SDL_FreeSurface(tempSurface);
        // SDL_DestroyTexture(tempTexture);

        // SDL_FColor tempColor = {255, 255, 255, 255};
        // drawCircleAtCoord(renderer, 100, 100, 100, 100, tempColor);
        // drawCircleAtCoord(renderer, 300, 300, 50, 100, tempColor);
        
        
        // SDL_Rect tempRect = {100, 100, 200, 200};
        // selectFontFromList("Roboto-Regular", 100);
        // renderTextAtCoord(renderer, "Hello world!", 100, 100, &SDLGreenColor, true, &tempRect);
        // selectFontFromList("BebasNeue-Regular", 50);
        // renderTextAtCoord(renderer, "Hello world!", 100, 100, &SDLGreenColor, false, NULL);
        // selectFontFromList("DancingScript-Regular", 200);
        // renderTextAtCoord(renderer, "world!", 500, 100, &SDLBlueColor, false, NULL);
        // selectFontFromList("CascadiaMono-Regular", 100);
        
        

        // char ccd[500];
        // if (getcwd(ccd, sizeof(ccd)) == NULL) {
        //     perror("getcwd() error");
        // } else {
        //     char imagePath[550];
        //     sprintf(imagePath, "%s/static/img/orange.jpg", ccd);
        //     renderImage(renderer, imagePath, 100, 100);
        // }
        
        // drawSquareAtCoord(renderer, 50, 50, 50, SDLGreenColor);
        // drawRectAtCoord(renderer, 100, 100, 150, 30, SDLBlueColor);
        // drawCircleAtCoord(renderer, 0, 0, 50, 100, SDLBlueColor);

        // printf("Hi\n");
        // selectFontFromList("Roboto-Regular", 10);
        // SDL_Texture *fpsTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, 200, 200);
        // SDL_SetRenderTarget(renderer, fpsTexture);
        // printf("Hi2\n");
        // renderTextAtCoord(renderer, currentFPS, 0, 0, &SDLBlueColor, false, NULL);
        // printf("Hi3\n");
        // SDL_SetRenderTarget(renderer, NULL);
        // printf("Hi4\n");
        
        // unsigned int idToRemove = render_managerAddItemToDraw(2, 10, fpsTexture, NULL, 0, 0, 200, 200);
        // printf("Hi5\n");
        // printf("%u\n", idToRemove);
        
        render_managerDrawScene();
        // signed int renderResult = render_managerRemoveItem(2, idToRemove);
        // printf("%d\n", renderResult);
 
        SDL_RenderPresent(renderer);
        frameTime = SDL_GetTicks() - frameStartTime;
        if (frameTime > 0) {
            fps = 1000.0f / frameTime;
            sprintf(currentFPS, "%u", (int)round(fps));
        }

   
    }

    // ? Clear (mostly) everything of the system
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    utilsQuit();
    SDL_Quit();
    return 0;
}
