#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <conio.h>
#include "snake.h"
#include "game.h"
#include <SDL2/SDL.h>

int SQUARE_SIZE = 16;
int SQUARE_MARGIN = 1;
int MAP_SIZE_X = 34;
int MAP_SIZE_Y = 20;
int INITIAL_SPEED = 200;
float SPEED_FACTOR = 0.96;
SDL_Keycode KEYS[2][4] = {{SDLK_z, SDLK_q, SDLK_s, SDLK_d}, {SDLK_UP, SDLK_LEFT, SDLK_DOWN, SDLK_RIGHT}};
//direction_x, direction_y, direction, R, G, B
int SNAKES_DATA[2][6] = {{-1, 0, 3, 20, 20, 245}, {1, 0, 1, 20, 245, 20}};
SDL_Point SNAKES_POSITION[2] = {{4, 5}, {30, 15}};

int get_forbiden_direction(int direction)
{
    if(direction == 0)
        return 2;
    if(direction == 1)
        return 3;
    if(direction == 2)
        return 0;
    return 1;
}

int get_valid_key(t_snake** players, int player_nb)
{
    SDL_Event event;
    SDL_Keycode pressed_key;

    while(SDL_PollEvent(&event))
    {
        if (event.type == SDL_KEYDOWN)
        {
            pressed_key = event.key.keysym.sym;
            if(pressed_key == 'r')
                return 1;
            for(int i = 0; i < player_nb; ++i)
            {
                for(int j = 0; j < 4; ++j)
                {
                    if(KEYS[i][j] == pressed_key)
                    {
                        if(j != players[i]->forbiden_direction)
                        {
                            players[i]->direction = j;
                            i = player_nb;
                            break;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

void fruits_spawn(t_game* game)
{
    char tab[MAP_SIZE_Y][MAP_SIZE_X];
    SDL_Point possible_pos[MAP_SIZE_X * MAP_SIZE_Y];
    int count = 0;
    for(int i = 0; i < MAP_SIZE_Y; ++i)
    {
        for(int j = 0; j < MAP_SIZE_X; ++j)
        {
            tab[i][j] = '.';
        }
    }
    for(int i = 0; i < game->player_nb; ++i)
    {
        t_snake_part* elt = game->players[i]->first;
        while(elt)
        {
            tab[elt->position.y][elt->position.x] = 'o';
            elt = elt->next;
        }
        if(game->fruits[i]->eaten)
            tab[game->fruits[i]->position.y][game->fruits[i]->position.x] = 'x';
    }
    for(int i = 0; i < MAP_SIZE_Y; ++i)
    {
        for(int j = 0; j < MAP_SIZE_X; ++j)
        {
            if(tab[i][j] == '.')
                possible_pos[count++] = (SDL_Point){j, i};
        }
    }
    for(int i = 0; i < game->player_nb; ++i)
    if(game->fruits[i]->eaten)
    {
        {
            int rand_index = rand() % count;
            game->fruits[i]->position = possible_pos[rand_index];
            game->fruits[i]->eaten = 0;
        }
    }
}

t_snake* create_player(int index)
{
    SDL_Point position = SNAKES_POSITION[index];
    t_snake* snake = new_snake();
    snake->direction = SNAKES_DATA[index][2];
    snake->speed = INITIAL_SPEED;
    snake->timer = INITIAL_SPEED;
    for(int i = 0; i < 3; ++i)
    {
        add_snake_part(snake, new_snake_part(position));
        position.x += SNAKES_DATA[index][0];
        position.y += SNAKES_DATA[index][1];
    }
    return snake;
}

void clear_screen(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
    SDL_RenderClear(renderer);
}

void show_snake(t_snake* player, SDL_Renderer* renderer, SDL_Color color)
{
    int square2 = SQUARE_SIZE - SQUARE_MARGIN * 2;
    t_snake_part* elt = player->first;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    while(elt)
    {
        SDL_Rect part = {(elt->position.x) * SQUARE_SIZE + SQUARE_MARGIN, (elt->position.y) * SQUARE_SIZE + SQUARE_MARGIN, square2, square2};
        SDL_RenderFillRect(renderer, &part);
        elt = elt->next;
    }
}

void show_fruits(t_game* game)
{
    int fruit_margin = SQUARE_MARGIN + 2;
    int square2 = SQUARE_SIZE - fruit_margin * 2;
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    for(int i = 0; i < game->player_nb; ++i)
    {
        SDL_Rect fruit_rec = {(game->fruits[i]->position.x) * SQUARE_SIZE + fruit_margin, (game->fruits[i]->position.y) * SQUARE_SIZE + fruit_margin, square2, square2};
        SDL_RenderFillRect(game->renderer, &fruit_rec);
    }
}

t_snake** create_players(int player_nb)
{
    t_snake** players = malloc(sizeof(t_snake) * player_nb);
    for(int i = 0; i < player_nb; ++i)
    {
        players[i] = create_player(i);
    }
    return players;
}

void move_snake(t_snake* player)
{
    if(player->direction == 0)
        move_snake_up(player, MAP_SIZE_Y);
    else if(player->direction == 1)
        move_snake_left(player, MAP_SIZE_X);
    else if(player->direction == 2)
        move_snake_down(player, MAP_SIZE_Y);
    else if(player->direction == 3)
        move_snake_right(player, MAP_SIZE_X);
}

int eat_fruits(t_game* game)
{
    for(int i = 0; i < game->player_nb; ++i)
    {
        if(game->players[i]->moved)
        {
            for(int y = 0; y < game->player_nb; ++y)
            {
                if(game->players[i]->first->position.x == game->fruits[y]->position.x && game->players[i]->first->position.y == game->fruits[y]->position.y)
                {
                    add_snake_part(game->players[i], new_snake_part(game->fruits[y]->position));
                    game->players[i]->speed *= SPEED_FACTOR;
                    game->fruits[y]->eaten = 1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

int players_check(t_game* game)
{
    int count = 0;
    t_snake_part* elt;
    for(int i = 0; i < game->player_nb; ++i)
    {
        if(game->players[i]->moved == 0)
        {
            ++count;
            continue;
        }

        t_snake_part* head = game->players[i]->first;
        for(int j = 0; j < game->player_nb; ++j)
        {
            if(i == j)
                elt = game->players[j]->first->next;
            else
                elt = game->players[j]->first;
            while(elt)
            {
                if(head->position.x == elt->position.x && head->position.y == elt->position.y)
                {
                    game->players[i]->alive = 0;
                    j = game->player_nb;
                    break;
                }
                elt = elt->next;
            }
        }
        if(game->players[i]->alive)
            ++count;
    }
    if(game->player_nb >= 2)
        return count > 1;
    return count == 1;
}

t_fruits** create_fruits(int player_nb)
{
    t_fruits** fruits = malloc(sizeof(t_fruits) * player_nb);
    for(int i = 0; i < player_nb; ++i)
    {
        fruits[i] = malloc(sizeof(t_fruits));
        fruits[i]->position.x = 0;
        fruits[i]->position.y = 0;
        fruits[i]->eaten = 1;
    }
    return fruits;
}

int tick_game(t_game* game, float delta_time)
{
    if(get_valid_key(game->players, game->player_nb))
        return 0;
    for(int i = 0; i < game->player_nb; ++i)
    {
        t_snake* snake = game->players[i];
        snake->moved = 0;
        snake->timer -= delta_time;
        if(snake->timer <= 0)
        {
            snake->moved = 1;
            move_snake(snake);
            snake->forbiden_direction = get_forbiden_direction(snake->direction);
            snake->timer += snake->speed;
        }
        SDL_Color color = {SNAKES_DATA[i][4], SNAKES_DATA[i][5], SNAKES_DATA[i][6], 255};
        show_snake(snake, game->renderer, color);
    }
    int r = players_check(game);
    if(eat_fruits(game))
        fruits_spawn(game);
    show_fruits(game);
    return r;
}

void run_game(SDL_Renderer* renderer, int player_nb)
{
    int run = 1;
    t_snake** players = create_players(player_nb);
    t_fruits** fruits = create_fruits(player_nb);
    t_game game = {renderer, players, player_nb, fruits};
    fruits_spawn(&game);
    Uint32 lastTick = SDL_GetTicks();
    while(run)
    {
        // calcul durée dernière frame
        Uint32 currentTick = SDL_GetTicks();
        float delta_time = (currentTick - lastTick) / 1000.0f; // en secondes
        lastTick = currentTick;
        // effacer l'écran
        clear_screen(renderer);
        // tick game
        run = tick_game(&game, delta_time);
        // affichage
        SDL_RenderPresent(renderer);
    }

    for(int i = 0; i < player_nb; ++i)
    {
        free_snake(players[i]);
        free(game.fruits[i]);
    }
}

int main(int argc, char* argv[])
{
    int player_nb = argc > 1 ? atoi(argv[1]) : 1;
    int resolution_x = MAP_SIZE_X * SQUARE_SIZE;
    int resolution_y = MAP_SIZE_Y * SQUARE_SIZE;
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int error = SDL_CreateWindowAndRenderer(resolution_x, resolution_y, SDL_WINDOW_SHOWN, &window, &renderer);
    if(error)
    {
        SDL_Quit();
        return EXIT_SUCCESS;
    }
    run_game(renderer, player_nb);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}