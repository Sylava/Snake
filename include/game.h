#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "snake.h"

struct s_fruits
{
    SDL_Point position;
    int eaten;
};
typedef struct s_fruits t_fruits;

struct s_game
{
    SDL_Renderer* renderer;
    t_snake** players;
    int player_nb;
    t_fruits** fruits;
};
typedef struct s_game t_game;
#endif