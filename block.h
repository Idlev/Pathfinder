#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>
#include <vector>

#include "SDL2/SDL.h"


class block
{
    public:
        block(int x,int y,int i, int j,int blk_size,
              SDL_Texture *tex1,SDL_Texture *tex2, SDL_Texture *tex3,
              SDL_Texture *tex4,SDL_Texture *tex5, SDL_Texture *tex6);
        ~block();

        void show_block(SDL_Renderer *renderer);
        void toggle_block();
        bool is_clicked(int mouse_x,int mouse_y);
        void set_type(int num);
        void set_active(bool val);
        bool get_active();
        void set_visited(bool val);
        bool get_visited();
        void set_solution(bool val);
        int get_type();

        void set_visited_2(bool val);
        bool get_visited_2();


        //path
        std::vector<int> get_path();
        std::vector<int> get_path2();
        void add_direction(std::vector<int> p,int dir);
        void add_direction2(std::vector<int> p,int dir);
        void set_path(std::vector<int> p);
        void clear_path();

        int get_x();
        int get_y();

    private:

        SDL_Texture *block_unfilled;
        SDL_Texture *block_filled;
        SDL_Texture *block_start;
        SDL_Texture *block_target;
        SDL_Texture *block_visited;
        SDL_Texture *block_solution;
        SDL_Rect block_rect;

        int x_coord;
        int y_coord;

        int block_size;
        int type;
        bool active;
        bool visited;
        bool visited_2;
        bool solution;

        std::vector<int> path;
        std::vector<int> path2;
};

#endif // BLOCK_H
