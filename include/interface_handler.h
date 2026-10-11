/**
 * @file interface_handler.h
 * @brief This interface_handler (as his name suggest) manages all the interface and how they are being rendered.
 * @note Will need a lot more work to include everything, still a draft.
 * This code aims to translate a .yaml file to actual rendering by using images, text and all available means.
 */
#pragma once

#include <SDL3/SDL.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <yaml.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
// #include "../include/cJSON.h"

enum itemType {
    GROUP,
    TEXT,
    RECTANGLE,
    IMAGE,
    VIDEO
};

/**
 * @brief Init the variables and functions needed for the code to work.
 */
int interface_handlerInit(SDL_Renderer *renderer);
/**
 * @brief Closes the all the files and variables, use this to cleanly exit the program.
 */
int interface_handlerQuit();

/**
 * @brief Will load a scene from a file contained in the templates directory
 * @param sceneName The name of the file where the scene is to be loader (as a .jsonUI)
 */
int interface_handlerLoadScene(const char *filename);

int parse_yaml(const char *filename);
int parse_properties(yaml_parser_t parser, yaml_event_t event);
int parse_events(yaml_parser_t parser, yaml_event_t event);

unsigned int addItemRect(int colorR, int colorG, int colorB, int colorA, int sizeX, int sizeY, int posX, int posY);
unsigned int addItemText(int colorR, int colorG, int colorB, int colorA, int sizeX, int sizeY, int posX, int posY, char *textContent, char *fontFamily, int fontSize);
unsigned int convertYamlCharToUInt(const yaml_char_t *value);

int interface_handlerGenerateScene();
