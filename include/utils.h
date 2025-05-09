/**
 * @file utils.h
 * @brief utils file contains mostly every function that can be used in main. 
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/**
 * @brief Should be used for the app to know if loged or not.
 * @note This doesn't really do anything or used for now.
 */
typedef enum AppState {
    STATE_LOGIN,
    STATE_HOME
} AppState;

/**
 * @brief Used for transition
 * @note Will be repurposed
 */
typedef struct transitionProperties {
    SDL_Color *currentSDLColor;
    double red;
    double green;
    double blue;
    double alpha;
    int remainingFrames;
} transitionProperties;



// Used to initialize and remove the utils variables
/**
 * @brief Initializes the utils
 * Define some important variables, etc.
 */
int utilsInit();
/**
 * @brief Quit the utils
 * Quit modules and free variables
 * @todo Write more a function to clean more the system
 */
int utilsQuit();


/// @brief Initialize the background transition. /!\ SHOULD NOT BE USED, glitchy and not versatile. Will be rewrote.
/// @bug Will be rewritten-repurposed, not really working 
transitionProperties *transitionBackgroundColorInit(SDL_Color *startingColor, SDL_Color *endingColor, const int transitionNumberFrames);
/// @brief Render the background transition frame. /!\ SHOULD NOT BE USED, glitchy and not versatile. Will be rewrote.
/// @bug Will be rewritten-repurposed, not really working 
bool transitionBackgroundColorNextFrame(SDL_Renderer *renderer, transitionProperties *transitionPropertiesItem);

