/**
 * @file images.h
 * @brief This file contains everything needed to render and modify images
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/**
 * @brief Initialize the variables and all modules needed for using images
 */
signed int imagesInit();

/**
 * @brief Used to load an image as a texture
 * @param renderer The SDL renderer
 * @param imagePath The absolute path to an image (jpg or png)
 * @return 0 if success -1 if failure
 */
SDL_Texture *loadImageAsTexture(SDL_Renderer *renderer, const char *imagePath);
/**
 * @brief Used to load an image as a surface
 * @param renderer The SDL renderer
 * @param imagePath The absolute path to an image (jpg or png)
 * @return 0 if success -1 if failure
 */
SDL_Surface *loadImageAsSurface(const char *imagePath);
/**
 * @brief Render a given image on the screen
 * @param renderer The SDL renderer
 * @param imagePath The absolute path of an image (jpg or png)
 * @param x the x coordinate in pixel
 * @param y the y coordinate in pixel
 */
signed int renderImage(SDL_Renderer *renderer, const char *imagePath, const int x, const int y);