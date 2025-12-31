#ifndef SNAKE_H
#define SNAKE_H

#include <SDL2/SDL.h>

struct s_snake_part
{
    struct s_snake_part* next;
    SDL_Point position;
    char data;
};
typedef struct s_snake_part t_snake_part;

struct s_snake
{
    int alive;
    t_snake_part* first;
    t_snake_part* tail;
    float speed;
    int timer;
    int size;
    int moved;
    int direction;
    int forbiden_direction;
};
typedef struct s_snake t_snake;

t_snake* new_snake();

void free_snake(t_snake* snake);

t_snake_part* new_snake_part(SDL_Point position);

void free_snake_part(t_snake_part* part);

void add_snake_part(t_snake* snake, t_snake_part* part);

int snake_length(t_snake* snake);

void move_snake_right(t_snake* snake, int x_map);
void move_snake_left(t_snake* snake, int x_map);
void move_snake_up(t_snake* snake, int y_map);
void move_snake_down(t_snake* snake, int y_map);
void follow_snake(t_snake_part* elt, SDL_Point position);
#endif