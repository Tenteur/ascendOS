/**
 * @file interface_handler.c
 * @brief This file manages all the interface and how they are being rendered.
 */

#include <yaml.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "../include/images.h"
#include "../include/text.h"
#include "../include/shapes.h"
#include "../include/event_handler.h"
#include "../include/render_manager.h"
#include "../include/interface_handler.h"

char currentWorkingDirectory[250];
char *defaultFont = "Roboto-Regular";
SDL_Renderer *interfaceRenderer;

int interface_handlerInit(SDL_Renderer *renderer) {
    if (getcwd(currentWorkingDirectory, sizeof(currentWorkingDirectory)) == NULL) {
        perror("getcwd error");
        return 1;
    }
    printf("interface_handler Initialized !\n");
    interfaceRenderer = renderer;
    return 0;
}


int parse_yaml(const char *filename) {
    printf("file path: %s\n", filename);
    FILE *file = fopen(filename, "rb");
    yaml_parser_t parser;
    yaml_event_t event;
    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, file);
    // unsigned int numberOfMappings = 0;
    // unsigned int numberOfSequence = 0;

    while (1) {

        // switch(event.type){
        //     case YAML_SCALAR_EVENT:
        //         // if (strcmp((char *)event.data.scalar.value, "type") == 0) {
        //         //     yaml_parser_parse(&parser, &event);
        //         //     if (strcmp((char *)event.data.scalar.value, "rect") == 0) {
        //         //         parse_rect(parser, event);
        //         //     } else if (strcmp((char *)event.data.scalar.value, "text") == 0) {
        //         //         parse_text(parser, event);
        //         //     }
        //         // }
        //         printf("Scalar (%s)\n", event.data.scalar.value);
        //         break;
        //     case YAML_MAPPING_START_EVENT:
        //         printf("Start of a mapping\n");
        //         break;
        //     case YAML_NO_EVENT:
        //         printf("There is no events");
        //         break;
        //     case YAML_STREAM_END_EVENT:
        //         printf("stream end event \n");
        //         break;
        //     case YAML_STREAM_START_EVENT:
        //         printf("stream start event \n");
        //         break;
        //     case YAML_DOCUMENT_START_EVENT:
        //         printf("document start event \n");
        //         break;
        //     case YAML_DOCUMENT_END_EVENT:
        //         printf("document end event \n");
        //         break;
        //     case YAML_ALIAS_EVENT:
        //         printf("alias event \n");
        //         break;
        //     case YAML_SEQUENCE_START_EVENT:
        //         printf("sequence start event \n");
        //         break;
        //     case YAML_SEQUENCE_END_EVENT:
        //         printf("sequence end event \n");
        //         break;
        //     case YAML_MAPPING_END_EVENT:
        //         printf("mapping end event \n");
        //         break;
        //     default:
        //         break;
        // }
        yaml_parser_parse(&parser, &event);

        if (event.type == YAML_SCALAR_EVENT && strcmp((const char*)event.data.scalar.value, "root") == 0) {
            break;
        }


    };
    parse_properties(parser, event);
    yaml_event_delete(&event);

    printf("finished it\n");
    // yaml_parser_delete(&parser);
    fclose(file);
    return 0;
}

// TODO: Switch to strtol.
inline unsigned int conv(const yaml_char_t *value) {
    // return strtol()
    // printf("%i", (int)value);
    // return atoi((char *)value);
    char *endPtr;
    long result = strtol(value, &endPtr, 10);
    return result;
}

int parse_properties(yaml_parser_t parser, yaml_event_t event) {
    int posX = 0, posY = 0, sizeX = 0, sizeY = 0, colorR = 0, colorG = 0, colorB = 0, colorA = 0, itemID = 0, itemLayer = 0;
    char *fontFamily, *text, *path;
    int fontSize = 0;
    int itemType = -1;
    int idDownEvent = -1;
    int eventType = -1;
    int done = 0;

    while (!done) {
        // if (event.type == YAML_MAPPING_END_EVENT) break;
        printf("In loop, itemType: %i\n", itemType);
        yaml_parser_parse(&parser, &event);
        switch (event.type) {
            case YAML_DOCUMENT_END_EVENT:
                done = 1;
                break;
            case YAML_NO_EVENT:
                done = 1;
                printf("No more events, breaking.\n");
                break;
            case YAML_MAPPING_END_EVENT:
                switch (itemType) {
                    case RECTANGLE:
                        addItemRect(colorR, colorG, colorB, colorA, sizeX, sizeY, posX, posY);
                        break;
                    case TEXT:
                        addItemText(colorR, colorG, colorB, colorA, sizeX, sizeY, posX, posY, text, fontFamily, fontSize);
                        break;
                    default:
                        done = 1;
                        break;
                }
                break;
            case YAML_SCALAR_EVENT:
                if (strcmp((char*)event.data.scalar.value, "type") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (strcmp((char*)event.data.scalar.value, "group") == 0) itemType = GROUP;
                    else if (strcmp((char*)event.data.scalar.value, "text") == 0) itemType = TEXT;
                    else if (strcmp((char*)event.data.scalar.value, "rect") == 0) itemType = RECTANGLE;
                    else if (strcmp((char*)event.data.scalar.value, "image") == 0) itemType = IMAGE;
                    else if (strcmp((char*)event.data.scalar.value, "video") == 0) itemType = VIDEO;

                } else if (strcmp((char*)event.data.scalar.value, "position") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type != YAML_SEQUENCE_START_EVENT) {
                        return -1;
                    }
                    yaml_parser_parse(&parser, &event);
                    posX = conv(event.data.scalar.value);
                    yaml_parser_parse(&parser, &event);
                    posY = conv(event.data.scalar.value);
                    yaml_parser_parse(&parser, &event); // Needed, else it SegFault

                } else if (strcmp((char *) event.data.scalar.value, "fontSize") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type != YAML_SCALAR_EVENT) return -1;
                    fontSize = conv(event.data.scalar.value);
                    // yaml_parser_parse(&parser, &event); // Needed, else it SegFault

                } else if (strcmp((char *) event.data.scalar.value, "color") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type != YAML_SEQUENCE_START_EVENT) return -1;
                    yaml_parser_parse(&parser, &event);
                    colorR = conv(event.data.scalar.value);
                    yaml_parser_parse(&parser, &event);
                    colorG = conv(event.data.scalar.value);
                    yaml_parser_parse(&parser, &event);
                    colorB = conv(event.data.scalar.value);
                    yaml_parser_parse(&parser, &event); // Needed, else it SegFault
                    colorA = conv(event.data.scalar.value);
                    yaml_parser_parse(&parser, &event); // Needed, else it SegFault

                } else if (strcmp((char*)event.data.scalar.value, "fontFamily") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type != YAML_SCALAR_EVENT) return -1;
                    fontFamily = (char*)event.data.scalar.value;
                    // yaml_parser_parse(&parser, &event); // Needed, else it SegFault

                } else if (strcmp((char *) event.data.scalar.value, "textContent") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type != YAML_SCALAR_EVENT) return -1;
                    text = (char*)event.data.scalar.value;
                    // yaml_parser_parse(&parser, &event); // Needed, else it SegFault

                } else if (strcmp((char *) event.data.scalar.value, "size") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type == YAML_SEQUENCE_START_EVENT) {
                        yaml_parser_parse(&parser, &event);
                        sizeX = conv(event.data.scalar.value);
                        yaml_parser_parse(&parser, &event);
                        sizeY = conv(event.data.scalar.value);
                    } else if (event.type == YAML_SCALAR_EVENT) {
                        sizeX = 0;
                        sizeY = 0;
                    }
                } else if (strcmp((char *) event.data.scalar.value, "event") == 0) {
                    yaml_parser_parse(&parser, &event);
                    if (event.type == YAML_SEQUENCE_START_EVENT) {
                        parse_events(parser, event);

                    }
                }
                break;

            default:
                break;
        }
    }
    return 0;
}

int parse_events(yaml_parser_t parser, yaml_event_t event) {
    int eventType = -1, isDownEvent = -1, x1 = -1, x2 = -1, y1 = -1, y2 = -1, itemID = -1, itemLayer = -1, fonctionPtr = -1;
    int alreadyParsedPrev = 0;
    while (true) {
        switch(event.type) {
            case YAML_SCALAR_EVENT:
                printf("Scalar (%s)\n", event.data.scalar.value);
                break;
            case YAML_MAPPING_START_EVENT:
                printf("Start of a mapping\n");
                break;
            case YAML_NO_EVENT:
                printf("There is no more events\n");
                _exit(10);
                break;
            case YAML_STREAM_END_EVENT:
                printf("stream end event \n");
                break;
            case YAML_STREAM_START_EVENT:
                printf("stream start event \n");
                break;
            case YAML_DOCUMENT_START_EVENT:
                printf("document start event \n");
                break;
            case YAML_DOCUMENT_END_EVENT:
                printf("document end event \n");
                break;
            case YAML_ALIAS_EVENT:
                printf("alias event \n");
                break;
            case YAML_SEQUENCE_START_EVENT:
                printf("sequence start event \n");
                break;
            case YAML_SEQUENCE_END_EVENT:
                printf("sequence end event \n");
                break;
            case YAML_MAPPING_END_EVENT:
                printf("mapping end event \n");
                break;
            default:
                break;
        }
        switch (event.type) {
            case YAML_SCALAR_EVENT:
                if (strcmp((char*)event.data.scalar.value, "eventType") == 0) {
                    yaml_parser_parse(&parser, &event);                                         // TODO: Optimize this part, this is horrendous >:[
                    if (strcmp((char*)event.data.scalar.value, "onHover") == 0) {eventType = HOVER; isDownEvent = 2;}
                    else if (strcmp((char*)event.data.scalar.value, "onLeftClick") == 0) {eventType = LEFTCLICK; isDownEvent = 2;}
                    else if (strcmp((char*)event.data.scalar.value, "onMiddleClick") == 0) {eventType = MIDDLECLICK; isDownEvent = 2;}
                    else if (strcmp((char*)event.data.scalar.value, "onRightClick") == 0) {eventType = RIGHTCLICK; isDownEvent = 2;}
                    else if (strcmp((char*)event.data.scalar.value, "onTyping") == 0) {eventType = TYPING; isDownEvent = 2;}
                    else if (strcmp((char*)event.data.scalar.value, "onManualTrigger") == 0) {eventType = MANUALTRIGGER; isDownEvent = 2;}

                    else if (strcmp((char*)event.data.scalar.value, "onHoverUp") == 0) {eventType = HOVER; isDownEvent = 0;}
                    else if (strcmp((char*)event.data.scalar.value, "onLeftClickUp") == 0) {eventType = LEFTCLICK; isDownEvent = 0;}
                    else if (strcmp((char*)event.data.scalar.value, "onMiddleClickUp") == 0) {eventType = MIDDLECLICK; isDownEvent = 0;}
                    else if (strcmp((char*)event.data.scalar.value, "onRightClickUp") == 0) {eventType = RIGHTCLICK; isDownEvent = 0;}
                    else if (strcmp((char*)event.data.scalar.value, "onTypingUp") == 0) {eventType = TYPING; isDownEvent = 0;}
                    else if (strcmp((char*)event.data.scalar.value, "onManualTriggerUp") == 0) {eventType = MANUALTRIGGER; isDownEvent = 0;}

                    else if (strcmp((char*)event.data.scalar.value, "onHoverDown") == 0) {eventType = HOVER; isDownEvent = 1;}
                    else if (strcmp((char*)event.data.scalar.value, "onLeftClickDown") == 0) {eventType = LEFTCLICK; isDownEvent = 1;}
                    else if (strcmp((char*)event.data.scalar.value, "onMiddleClickDown") == 0) {eventType = MIDDLECLICK; isDownEvent = 1;}
                    else if (strcmp((char*)event.data.scalar.value, "onRightClickDown") == 0) {eventType = RIGHTCLICK; isDownEvent = 1;}
                    else if (strcmp((char*)event.data.scalar.value, "onTypingDown") == 0) {eventType = TYPING; isDownEvent = 1;}
                    else if (strcmp((char*)event.data.scalar.value, "onManualTriggerDown") == 0) {eventType = MANUALTRIGGER; isDownEvent = 1;}

                }
                // check what the action for the event is (for now this is in the function, will move this code to its own function).
                else if (strcmp((char*)event.data.scalar.value, "action") == 0) {
                    yaml_parser_parse(&parser, &event);
                    // char strbr[strlen((char *) event.data.scalar.value)];
                    // strcpy(((char *) event.data.scalar.value), strbr);
                    // char strbr[] = "debugMsg,Hello world!";
                    const char *separator = ",";
                    char *string = malloc(strlen((char*)event.data.scalar.value) + 1);
                    strcpy(string, (char*)event.data.scalar.value);
                    char *strToken = strtok(string, separator);
                    while (strToken != nullptr) {
                        // The event is 'debugMsg'
                        if (strcmp(strToken, "debugMsg") == 0) {
                            strToken = strtok(nullptr, separator);
                            size_t strTokenLen = strlen(strToken);
                            struct extraData *extraData = malloc(sizeof(struct extraData));
                            extraData->data = malloc(strTokenLen + 1);
                            extraData->dataLength = strTokenLen;
                            strcpy((char*)extraData->data, strToken);
                            // free(extraData->data); //WHY FREE'D ????
                            // free(extraData);       //WHY FREE'D ????
                        } else {
                            strToken = strtok(nullptr, separator);
                        }

                    }
                    free(string);
                }
                break;
            case YAML_SEQUENCE_END_EVENT:
                return 1;
            default:
                break;
        }
        if (alreadyParsedPrev == 0) {
            yaml_parser_parse(&parser, &event);
        } else {
            alreadyParsedPrev = 0;
        }
    }
    return 0;
}

unsigned int addItemRect(int colorR, int colorG, int colorB, int colorA, int sizeX, int sizeY, int posX, int posY) {
    SDL_Surface *rectSurface = SDL_CreateSurface(sizeX, sizeY, SDL_PIXELFORMAT_RGBA8888);
    if (rectSurface == NULL) return 0;
    const SDL_PixelFormatDetails *formatDetails = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA8888);
    const Uint32 rectColor = SDL_MapRGBA(formatDetails, NULL, (Uint8)colorR, (Uint8)colorG, (Uint8)colorB, (Uint8)colorA);
    SDL_FillSurfaceRect(rectSurface, NULL, rectColor);
    SDL_Texture *rectTexture = SDL_CreateTextureFromSurface(interfaceRenderer, rectSurface);
    SDL_DestroySurface(rectSurface);
    if (rectTexture == NULL) return 0;
    unsigned int rectItemID = render_managerAddItemToDraw(1, 0, rectTexture, NULL, posX, posY, sizeX, sizeY);
    return rectItemID;
}

unsigned int addItemText(int colorR, int colorG, int colorB, int colorA, int sizeX, int sizeY, int posX, int posY, char *textContent, char *fontFamily, int fontSize) {
    SDL_FColor textColor = {colorR, colorG, colorB, colorA};
    if (selectFontFromList(fontFamily, fontSize) == -1) addFontToList(fontFamily, fontSize);
    SDL_Texture *textTexture = generateTextTexture(interfaceRenderer, textContent, &textColor, sizeX, sizeY);
    printf("text texture: %p", textTexture);
    // SDL_GetTextureSize()
    unsigned int textItemID = render_managerAddItemToDraw(1, 0, textTexture, nullptr, posX, posY, 300, 300);
    return textItemID;
}

int interface_handlerGenerateScene() {
    return 0;
}
