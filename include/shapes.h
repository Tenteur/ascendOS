
/**
 * @file shapes.h
 * @brief shapes file contains everything needed to draw shapes and modify them
 * @note Is not fully implemented, lots of things missing
 */

#include <SDL2/SDL.h>

/**
 * @brief Draw circle on screen
 * Draw a cicle at given coordinate and given radius. The origin of the circle is top left
 * @param x x coordinate in pixels
 * @param y y coordinate in pixels
 * @param radius radius as a float (number of pixels i think)
 * @param precision number of segments to draw as int
 * @return 0 if success, -1 if failure
 */
signed int drawCircleAtCoord(SDL_Renderer *renderer, int x, int y, const float radius, const int precision, const SDL_Color color);
/**
 * @brief Draw a square on screen
 * Draw a square with a given color, coordinates and size. The origin of the square is top left
 * @param x x coordinates in pixels
 * @param y y coordinates in pixels
 * @param size the size of one side (which will be the size of the four side)
 * @param color the color of the square
 * @return 0 if success, -1 if failure
 */
signed int drawSquareAtCoord(SDL_Renderer *renderer, const int x, const int y, const int size, const SDL_Color color);
/**
 * @brief Draw a rectangle on screen
 * Draw a rectangle with a given color, coordinates and size. The origin of the rectange is top left
 * @param x x coordinates in pixels
 * @param y y coordinates in pixels
 * @param xsize the size of one the horizontal side
 * @param ysize the size of one the vertical side
 * @param color the color of the square
 * @return 0 if success, -1 if failure
 */
signed int drawRectAtCoord(SDL_Renderer *renderer, const int x, const int y, const int xsize, const int ysize, const SDL_Color color);
