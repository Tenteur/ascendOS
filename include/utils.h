#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

typedef enum {
    STATE_LOGIN,
    STATE_HOME
} AppState;

typedef struct {
    SDL_Color *currentSDLColor;
    double red;
    double green;
    double blue;
    double alpha;
    int remainingFrames;
} transitionProperties;

typedef struct {
    int numberOfVertices;
    SDL_Vertex vertices[];
} shapeProperties;

// Render a circle of solid color at x,y coord with a radius. The precision is the number of segments the circle have.
signed int drawCircleAtCoord(SDL_Renderer *renderer, int x, int y, float radius, const int precision, SDL_Color color);

// Initialize the background transition. /!\ SHOULD NOT BE USED, glitchy and not versatile. Will be rewrote.
transitionProperties *transitionBackgroundColorInit(SDL_Color *startingColor, SDL_Color *endingColor, int transitionNumberFrames);
// Render the background transition frame. /!\ SHOULD NOT BE USED, glitchy and not versatile. Will be rewrote.
bool transitionBackgroundColorNextFrame(SDL_Renderer *renderer, transitionProperties *transitionPropertiesItem);