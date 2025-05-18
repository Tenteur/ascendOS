/**
 * @file render_manager.h
 * @brief This render_manager contains everything needed to manager the renderer
 * @note Need a lot of work, will not be operationnal for long time.
 * This script aim to simplify the rendering and keeping/destroying textures by managing them automatically.
 * Once this script is complete, should disallow the use of raw SDL2 rendering.
 */

#include <SDL2/SDL.h>

/**
 * @brief Initialize all the needed variables and scripts for the render_manager
 * @return 0 if success, -1 if failure
 */
signed int render_managerInit();

/**
 * @brief Draws complete or partial scene
 * @param sceneID 1 for background, 2 for middle, 3 for foreground. Draws all upper scene (1 draws 1, 2 and 3). If 0, draws everything.
 * @note Will probably be modified since there is no real use case for drawing scenes independently.
 * @return 0 if success, -1 if failure.
 */
signed int render_managerDrawScene(int sceneID);

/**
 * @brief Add a texture to draw to screen.
 * Add a texture to draw and return the id of this texture relative to scene
 * @param sceneID The ID of the scene where the texture is drawn (1 background, 2 middle, 3 foreground).
 * @param layer The layer to draw the texture, lower layer gets drawn first (0->1->2) if two texute have the same layer but overlap, the first to be drawn is based on the ID the texute gets.
 * @param texturePtr The pointer to the texture to add.
 * @param x The x coordinate where the texture gets drawn relative to screen (absolute so).
 * @param y The y coordinate where the texture gets drawn relative to screen (absolute so).
 * @return ID of the texture that got added, -1 if error occured, ID > 0 if success
 */
signed int render_managerAddTextureToDraw(int sceneID, signed int layer, SDL_Texture *texturePtr, const int x, const int y);
/**
 * @brief Add a surface to draw to screen.
 * Add a texture to draw and return the id of this texture relative to scene
 * @note The surface gets converted to a texture before being drawn.
 * @param sceneID The ID of the scene where the texture is drawn (1 background, 2 middle, 3 foreground).
 * @param layer The layer to draw the surface, lower layer gets drawn first (0->1->2) if two surface have the same layer but overlap, the first to be drawn is based on the ID the texute gets.
 * @param surfacePtr The pointer to the surface to add.
 * @param x The x coordinate where the surface gets drawn relative to screen (absolute so).
 * @param y The y coordinate where the surface gets drawn relative to screen (absolute so).
 * @return ID of the surface that got added, -1 if error occured, ID > 0 if success
 */
signed int render_managerAddSurfaceToDraw(int sceneID, signed int layer, SDL_Surface *surfacePtr, const int x, const int y);
