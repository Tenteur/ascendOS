/**
 * @file shapes.h
 * @brief shapes file contains everything needed to draw shapes and modify them
 * @note Is not fully implemented, lots of things missing
 */

#include <SDL2/SDL.h>

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
