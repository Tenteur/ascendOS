/**
 * @file interface_handler.h
 * @brief This interface_handler (as his name suggest) manages all the interface and how they are being rendered.
 * @note Will need a lot more work to include everything, still a draft.
 * This code aims to translate a .jsonUI file to actual rendering by using images, text and all available means.
 */

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