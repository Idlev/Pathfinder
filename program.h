#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include "block.h"

class program
{
    public:
        program();
        ~program();

        void start();

    private:

        //Main loop
        bool is_running;

        //IMAGE LOAD
        SDL_Surface *load_image(const char *filename);

        //PATHFINDING ALGORTIHMS
        void algorithm_lees();
        void algorithm_improved();
        int algo_chosen;

        //SCREEN
        SDL_Window *window;
        SDL_Renderer *renderer;
        const int SCREEN_WIDTH = 850;
        const int SCREEN_HEIGHT = 650;
        int scale_current;

        //BUTTONS
        SDL_Surface *button_start_surface;
        SDL_Texture *button_start_texture;
        SDL_Surface *button_reset_surface;
        SDL_Texture *button_reset_texture;
        SDL_Surface *button_random_surface;
        SDL_Texture *button_random_texture;

        //Algo selection
        SDL_Surface *button_algo1_surface;
        SDL_Texture *button_algo1_texture;
        SDL_Surface *button_algo2_surface;
        SDL_Texture *button_algo2_texture;

        SDL_Surface *chosen_surface;
        SDL_Texture *chosen_texture;

        //Rects
        SDL_Rect button_start_rect;
        SDL_Rect button_reset_rect;
        SDL_Rect button_random_rect;
        SDL_Rect button_algo1_rect;
        SDL_Rect button_algo2_rect;

        //BLOCKS
        std::vector<block*> block_vec;
        block *solution_block;
        void make_blocks();
        void make_blocks2();
        void show_blocks();
        int block_size;

        SDL_Surface *block_unfilled_surface;
        SDL_Texture *block_unfilled_texture;
        SDL_Surface *block_filled_surface;
        SDL_Texture *block_filled_texture;
        SDL_Surface *block_start_surface;
        SDL_Texture *block_start_texture;
        SDL_Surface *block_target_surface;
        SDL_Texture *block_target_texture;
        SDL_Surface *block_visited_surface;
        SDL_Texture *block_visited_texture;
        SDL_Surface *block_solution_surface;
        SDL_Texture *block_solution_texture;

        //LOGIC
        void program_logic();
        bool start_set;
        bool target_set;
        void reset();
        void clear_start();
        void draw();
        void set_solution();
        void randomize();

        //BACKGROUNG
        SDL_Surface *surface_bg;
        SDL_Texture *texture_bg;
        SDL_Rect rect_bg;

        //GRID
        void draw_grid(int x,int y,int num,int space,int len);
        int grid_size;
        int line_width;
        int line_length;
        int grid_x;
        int grid_y;

        //FPS
        const int FRAMES_PER_SECOND = 60;
        void regulate_fps(Uint32 start);

        //EVENTS
        void handle_events();
        bool mouse_click;
        int mouse_x;
        int mouse_y;

        bool check_button_click(int x,int y, SDL_Rect *rect);

        void print_vector(std::vector<int> vec);
};

#endif // PROGRAM_H
