#include "snake.h"
#include <stdlib.h>
#include <stdio.h>

t_snake* new_snake()
{
    t_snake* snake = malloc(sizeof(t_snake));
    snake->alive = 1;
    snake->first = NULL;
    snake->tail = NULL;
    snake->size = 0;
    snake->speed = 0;
    snake->timer = 0;
    snake->moved = 0;
    snake->direction = 1;
    snake->forbiden_direction = 3;
    return snake;
}

void free_snake(t_snake* snake)
{
    t_snake_part* elt = snake->first;
    t_snake_part* elt2 = NULL;
    while(elt)
    {
        elt2 = elt->next;
        free_snake_part(elt);
        elt = elt2;
    }

    free(snake);
}

t_snake_part* new_snake_part(SDL_Point position)
{
    t_snake_part* part = malloc(sizeof(t_snake_part));
    part->next = NULL;
    part->position = position;
    return part;
}

void free_snake_part(t_snake_part* part)
{
    free(part);
}

void add_snake_part(t_snake* snake, t_snake_part* part)
{
    if(snake->first == NULL)
        snake->first = part;
    else
        snake->tail->next = part;
    snake->tail = part;
    snake->size++;
    
}

int snake_length(t_snake* snake)
{
    int length = 0;
    t_snake_part* elt = snake->first;
    while(elt)
    {
        elt = elt->next;
        ++length;
    }
    return length;
}

void move_snake_right(t_snake* snake, int x_map)
{
    if(!snake->first)
        return;

    if(snake->first->next)
        follow_snake(snake->first->next, snake->first->position);
    
    snake->first->position.x++;
    if(snake->first->position.x >= x_map)
        snake->first->position.x = 0;
}

void move_snake_left(t_snake* snake, int x_map)
{
    if(!snake->first)
        return;

    if(snake->first->next)
        follow_snake(snake->first->next, snake->first->position);
    
    snake->first->position.x--;
    if(snake->first->position.x < 0)
        snake->first->position.x = x_map - 1;
}

void move_snake_up(t_snake* snake, int y_map)
{
    if(!snake->first)
        return;

    if(snake->first->next)
        follow_snake(snake->first->next, snake->first->position);
    
    snake->first->position.y--;
    if(snake->first->position.y < 0)
        snake->first->position.y = y_map - 1;
}

void move_snake_down(t_snake* snake, int y_map)
{
    if(!snake->first) return;

    if(snake->first->next)
        follow_snake(snake->first->next, snake->first->position);
    
    snake->first->position.y++;
    if(snake->first->position.y >= y_map)
        snake->first->position.y = 0;
}

void follow_snake(t_snake_part* elt, SDL_Point position)
{
    if(elt->next)
        follow_snake(elt->next, elt->position);
    elt->position = position;
}