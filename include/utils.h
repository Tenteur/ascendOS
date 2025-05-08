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

/**
 * @brief Used for storing fonts
 */
typedef struct fontListStruct {
    char fontName[20];
    int fontSize;
    TTF_Font *fontPtr;
    struct fontListStruct *next;
} fontListStruct;

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

/**
 * @brief Draw circle on screen
 * Draw a cicle at given coordinate and given radius.
 * @param x x coordinate in pixels
 * @param y y coordinate in pixels
 * @param radius radius as a float (number of pixels i think)
 * @param precision number of segments to draw as int
 * @return 0 if success, -1 if failure
 */
signed int drawCircleAtCoord(SDL_Renderer *renderer, const int x, const int y, const float radius, const int precision, const SDL_Color color);

/// @brief Initialize the background transition. /!\ SHOULD NOT BE USED, glitchy and not versatile. Will be rewrote.
/// @bug Will be rewritten-repurposed, not really working 
transitionProperties *transitionBackgroundColorInit(SDL_Color *startingColor, SDL_Color *endingColor, const int transitionNumberFrames);
/// @brief Render the background transition frame. /!\ SHOULD NOT BE USED, glitchy and not versatile. Will be rewrote.
/// @bug Will be rewritten-repurposed, not really working 
bool transitionBackgroundColorNextFrame(SDL_Renderer *renderer, transitionProperties *transitionPropertiesItem);

// Render a text at x,y coord with a SDL_Color specified and size
/**
 * @brief Draw text on screen
 * Draw a given text on screen with given coordinates
 * @param text the text that should be rendered
 * @param x x coordinates in pixels
 * @param y y coordinates in pixels
 * @param textColor color in which the text should be
 * @param useCustomSize should use the customSize ?
 * @return 0 if success, -1 if failure
 */
signed int renderTextAtCoord(SDL_Renderer *renderer, const char *text, const int x, const int y, SDL_Color *textColor, bool useCustomSize, SDL_Rect *customSizeRect);
// Generate text texture and return it as a pointer
SDL_Texture *generateTextTexture(SDL_Renderer *renderer, const char *text, const SDL_Color *textColor);
// Return a pointer of a font based on the name and size asked
/**
 * @brief Add a font to be selected
 * Add a font to the list of selectible fonts
 * @param fontName the name of the font that should be added
 * @param fontSize the size of the font that should be added
 * @return 0 if success, -1 if failure
 */
signed int addFontToList(char *fontName, int fontSize);
/**
 * @brief Set the selected font
 * Set the selected font from the list of added fonts
 * @param fontName the name of the font that should be selected
 * @param fontSize the size of the font that should be selected
 * @return 0 if success, -1 if failure
 */
signed int selectFontFromList(char *fontName, int fontSize);
signed int removeFontFromList(char fontName, int fontSize);