/**
 * @file shapes.c
 * @brief shapes.c contains implementations of functions declared inside shapes.h
 */


#include <SDL2/SDL.h>
#include "../include/shapes.h"


signed int drawCircleAtCoord(SDL_Renderer *renderer, const int x, const int y, const float radius, const int precision, const SDL_Color color) {
    const float increment = 2.0f * 3.14 / precision;
    SDL_Vertex vertices[precision + 2];
    
    // Centre du cercle
    vertices[0].position.x = x;
    vertices[0].position.y = y;
    vertices[0].color = color;
    vertices[0].tex_coord.x = 0;
    vertices[0].tex_coord.y = 0;

    for (int i = 0; i <= precision; i++) {
        float angle = i * increment;
        vertices[i + 1].position.x = roundf(x + radius * cosf(angle));
        vertices[i + 1].position.y = roundf(y + radius * sinf(angle));
        vertices[i + 1].color = color;
        vertices[i + 1].tex_coord.x = 0;
        vertices[i + 1].tex_coord.y = 0;
    }

    // Chaque triangle est (centre, i, i+1)
    for (int i = 1; i <= precision; i++) {
        SDL_Vertex triangle[3] = { vertices[0], vertices[i], vertices[i + 1] };
        if (SDL_RenderGeometry(renderer, NULL, triangle, 3, NULL, 0) < 0) {
            return -1;
        }
    }
    return 0;
}