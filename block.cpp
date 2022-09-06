#include "block.h"

block::block(int x,int y,int i, int j,int blk_size,
             SDL_Texture *tex1,SDL_Texture *tex2, SDL_Texture *tex3,
             SDL_Texture *tex4,SDL_Texture *tex5, SDL_Texture *tex6)
{
    //0 for start, 1 for target, 2 for block
    type = 2;

    block_size = blk_size;
    active = 0;
    visited = 0;
    visited_2 = 0;
    solution = 0;

    x_coord = i;
    y_coord = j;

    block_rect.x = x;
    block_rect.y = y;
    block_rect.w = block_size;
    block_rect.h = block_size;

    block_unfilled = tex1;
    block_filled = tex2;
    block_start = tex3;
    block_target = tex4;
    block_visited = tex5;
    block_solution = tex6;
}

block::~block()
{
    //dtor
}

int block::get_x(){

    return x_coord;
}

int block::get_y(){

    return y_coord;
}

void block::set_type(int num){

    if(num == 0)
        type = 0;
    else if(num == 1){
        type = 1;
    }else if(num == 2){
        type = 2;
    }else{
        std::cout << "Incorrect type" << std::endl;
    }

}

void block::toggle_block(){

    if(active == false)
        active = true;
    else
        active = false;
}

void block::set_active(bool val){

    active = val;
}

bool block::get_active(){

    return active;
}

void block::set_visited(bool val){

    visited = val;
}

bool block::get_visited(){

    return visited;
}

void block::set_visited_2(bool val){
    visited_2 = val;
}

bool block::get_visited_2(){

    return visited_2;
}

int block::get_type(){

    return type;
}

void block::set_solution(bool val){

    solution = val;
}

bool block::is_clicked(int x,int y){

    bool ans = false;

    if( ( x >= block_rect.x and x <= block_rect.x + block_rect.w) &&
        ( y >= block_rect.y and y <= block_rect.y + block_rect.h) ){

        std::cout << "Block pos (x,y): " << x_coord << "," << y_coord << std::endl;
        ans = true;
    }

    return ans;
}

std::vector<int> block::get_path(){

    return path;
}

std::vector<int> block::get_path2(){

    return path2;
}

void block::set_path(std::vector<int> p){
    path = p;
}

//DIRECTIONS
//UP,RIGHT,DOWN,LEFT 0,1,2,3
void block::add_direction(std::vector<int> p,int dir){

    path = p;
    path.push_back(dir);
}

void block::add_direction2(std::vector<int> p,int dir){

    path2 = p;

    if(dir == 0){
        path2.push_back(2);
    }else if(dir == 1){
        path2.push_back(3);
    }else if(dir == 2){
        path2.push_back(0);
    }else if(dir == 3){
        path2.push_back(1);
    }
}

void block::clear_path(){

    path.clear();
    path2.clear();
}

void block::show_block(SDL_Renderer *renderer){

    if(type == 0){
        SDL_RenderCopy(renderer,block_start,NULL,&block_rect);
    }else if(type == 1){
        SDL_RenderCopy(renderer,block_target,NULL,&block_rect);
    }else if(type == 2){

        if(active == 1)
            SDL_RenderCopy(renderer,block_filled,NULL,&block_rect);
        else{

            if(visited == 0 and visited_2 == 0)
                SDL_RenderCopy(renderer,block_unfilled,NULL,&block_rect);
            else{

                if(solution == 1){
                    SDL_RenderCopy(renderer,block_solution,NULL,&block_rect);
                }else{
                    SDL_RenderCopy(renderer,block_visited,NULL,&block_rect);
                }
            }
        }
    }
}

