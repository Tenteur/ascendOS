/**
 * @file interface_handler.h
 * @brief This interface_handler (as his name suggest) manages all the interface and how they are being rendered.
 * @note Will need a lot more work to include everything, still a draft.
 * This code aims to translate a .jsonUI file to actual rendering by using images, text and all available means.
 */

#include <SDL2/SDL.h>

/**
 * @brief Init the variables and functions needed for the code to work.
 */
int interface_handlerInit();
/**
 * @brief Closes the all the files and variables, use this to cleanly exit the program.
 */
int interface_handlerQuit();

/**
 * @brief Will load a scene from a file contained in the templates directory
 * @param sceneName The name of the file where the scene is to be loader (as a .jsonUI)
 */
int interface_handlerLoadScene(char sceneName[15]);

/**
 * @brief Fuse two textures and destroys them
 * Create a new texture and output it to the textures' pointer (outputTexture)
 * The second texture may be drawn over the first one
 * @param renderer The SDL renderer.
 * @param outputTexture The pointer of the final texture (need to be as big as the biggest of the two textures) and should already exist
 * @param firstInputTexture The pointer of the first texture to fuse
 * @param secondInputTexture The pointer of the second texture to fuse
 * @param firstInputCoord The coordinate of the first texture relative to the output texture
 * @param secondInputCoord The coordinate of the second texture relative to the output texture
 * @param firstInputSRect The texture coordinate of the first texture relative to the output texture (can be defaulted to NULL)
 * @param secondInputSRect The texture coordinate of the second texture relative to the output texture (can be defaulted to NULL)
 */
signed int render_managerFuseTextures(SDL_Renderer *renderer, SDL_Texture *outputTexture, SDL_Texture *firstInputTexture, SDL_Texture *secondInputTexture, SDL_Rect *firstInputCoord, SDL_Rect *secondInputCoord, SDL_Rect *firstInputSRect, SDL_Rect *secondInputSRect);