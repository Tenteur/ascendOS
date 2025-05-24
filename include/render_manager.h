/**
 * @file render_manager.h
 * @brief This render_manager contains everything needed to manager the renderer
 * @note Need a lot of work, will not be operationnal for long time.
 * This script aim to simplify the rendering and keeping/destroying textures by managing them automatically.
 * Once this script is complete, should disallow the use of raw SDL2 rendering.
 */

#include <SDL2/SDL.h>
#include <stdbool.h>



/**
 * @brief This data structure stores all the information needed for rendering.
 * @param sceneID is where the item is drawn 
 */
typedef struct itemDataNode {
    int sceneID;
    signed int layer;
    int itemID;
    SDL_Texture *texturePtr;
    SDL_Surface *surfacePtr;
    int xCoord;
    int yCoord;
    int width;
    int height;
    struct itemDataNode *next;
} itemDataNode;

/**
 * @brief Initialize all the needed variables and scripts for the render_manager.
 * @return 0 if success, -1 if failure.
 */
signed int render_managerInit(SDL_Renderer *renderer);

/**
 * @brief Draws complete or partial scene
 * @param sceneID 1 for background, 2 for middle, 3 for foreground. Draws all upper scene (1 draws 1, 2 and 3). If 0, draws everything.
 * @note Will probably be modified since there is no real use case for drawing scenes independently.
 * @return 0 if success, -1 if failure.
 */
signed int render_managerDrawScene();

/**
 * @brief Add a texture to draw to screen.
 * Add a texture to draw and return the id of this item relative to scene
 * @note One of texturePtr and surfacePtr should be NULL, the one that is not NULL is the one that will be drawn.
 * @param sceneID The ID of the scene where the texture is drawn (0 background, 1 middle, 2 foreground).
 * @param layer The layer to draw the texture, lower layer gets drawn first (0->1->2) if two texute have the same layer but overlap, the first to be drawn is based on the ID the texute gets.
 * @param texturePtr The pointer to the texture to add, can be NULL (check note).
 * @param surfacePtr The pointer to the surface to add, can be NULL (check note).
 * @param x The x coordinate where the texture gets drawn relative to screen (absolute so).
 * @param y The y coordinate where the texture gets drawn relative to screen (absolute so).
 * @param width The width of the item.
 * @param height The height of the item.
 * @return itemID > 0 if success, 0 if failure.
 */
int render_managerAddItemToDraw(int sceneID, signed int layer, SDL_Texture *texturePtr, SDL_Surface *surfacePtr, const int x, const int y, const int width, const int height);
/**
 * @brief Add a surface to draw to screen.
 * Add a texture to draw and return the id of this item relative to scene
 * @note The surface gets converted to a texture before being drawn.
 * @param sceneID The ID of the scene where the texture is drawn (1 background, 2 middle, 3 foreground).
 * @param layer The layer to draw the surface, lower layer gets drawn first (0->1->2) if two surface have the same layer but overlap, the first to be drawn is based on the ID the texute gets.
 * @param surfacePtr The pointer to the surface to add.
 * @param x The x coordinate where the surface gets drawn relative to screen (absolute so).
 * @param y The y coordinate where the surface gets drawn relative to screen (absolute so).
 * @return itemID > 0 if success, 0 if failure.
 */
// int render_managerAddSurfaceToDraw(int sceneID, signed int layer, SDL_Surface *surfacePtr, const int x, const int y);

/**
 * @brief Get the itemID from the data structure.
 * Get the itemID from a SDL_Texture of SDL_Surface and compare them with what's stored.
 * @param texturePtr is used to compare it to stored textures pointers, if NULL, search for surfacePtr.
 * @param surfacePtr is used to compare it to stored surfaces pointers, if NULL, search for texturePtr.
 * @param sceneID search inside one scene, if -1, search across all scenes.
 * @note if both texturePtr and surfacePtr are NULL, return an error.
 * @return itemID > 0 if success, 0 if failure.
 */
int render_managerGetItemID(SDL_Texture *texturePtr, SDL_Surface *surfacePtr);

/**
 * @brief Deletes an item from being rendered.
 * Delete the item (free for surfaces and destroy for textures) from memory and data structure. Won't be rendered.
 * @param itemID the ID of the item to delete.
 * @param sceneID the ID of the scene where the item is located.
 * @param afterRender is the item deleted after the render (true) or before (false).
 * @return 0 if success, -1 if failure.
 */
signed int render_managerRemoveItem(int itemID, int sceneID, bool afterRender);

/**
 * @brief Update an item's position
 * Change the item's X and Y coordinate
 * @param itemID the ID of the item
 * @param sceneID the ID of the scene where the item is located
 * @param x the new X coord
 * @param y the new Y coord
 * @return 0 if success, -1 if failure.
 */
signed int render_managerUpdateItemCoordinate(int itemID, int sceneID, int x, int y);
