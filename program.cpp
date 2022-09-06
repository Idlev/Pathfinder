#include "program.h"

program::program()
{

    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow("Pathfinder",
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
                    SCREEN_WIDTH,SCREEN_HEIGHT,
                    SDL_WINDOW_OPENGL);

    renderer = SDL_CreateRenderer(window,-1,0);
    scale_current = 1;

    //Set initial values
    is_running = true;
    algo_chosen = 1;
    mouse_click = false;
    mouse_x = 0;
    mouse_y = 0;

    //Background
    surface_bg = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0);
    SDL_FillRect(surface_bg,NULL,SDL_MapRGB(surface_bg->format,220,230,255));
    texture_bg = SDL_CreateTextureFromSurface(renderer,surface_bg);

    rect_bg.x = 0;
    rect_bg.y = 0;
    rect_bg.w = SCREEN_WIDTH;
    rect_bg.h = SCREEN_HEIGHT;

    //GRID
    grid_size = 30;
    line_width = 20;
    line_length = 600;
    grid_x = 225;
    grid_y = 25;

    //BLOCK
    block_size = 19;

    //Create BLCOK textures

    block_unfilled_surface = SDL_CreateRGBSurface(0,block_size,block_size,32,0,0,0,0);
    SDL_FillRect(block_unfilled_surface,NULL,SDL_MapRGB(block_unfilled_surface->format,220,208,255));
    block_unfilled_texture = SDL_CreateTextureFromSurface(renderer,block_unfilled_surface);

    block_filled_surface = SDL_CreateRGBSurface(0,block_size,block_size,32,0,0,0,0);
    SDL_FillRect(block_filled_surface,NULL,SDL_MapRGB(block_unfilled_surface->format,45,44,47));
    block_filled_texture = SDL_CreateTextureFromSurface(renderer,block_filled_surface);

    block_start_surface = SDL_CreateRGBSurface(0,block_size,block_size,32,0,0,0,0);
    SDL_FillRect(block_start_surface,NULL,SDL_MapRGB(block_start_surface->format,45,100,245));
    block_start_texture = SDL_CreateTextureFromSurface(renderer,block_start_surface);

    block_target_surface = SDL_CreateRGBSurface(0,block_size,block_size,32,0,0,0,0);
    SDL_FillRect(block_target_surface,NULL,SDL_MapRGB(block_target_surface->format,222,23,56));
    block_target_texture = SDL_CreateTextureFromSurface(renderer,block_target_surface);

    block_visited_surface = SDL_CreateRGBSurface(0,block_size,block_size,32,0,0,0,0);
    SDL_FillRect(block_visited_surface,NULL,SDL_MapRGB(block_visited_surface->format,144,238,144));
    block_visited_texture = SDL_CreateTextureFromSurface(renderer,block_visited_surface);

    block_solution_surface = SDL_CreateRGBSurface(0,block_size,block_size,32,0,0,0,0);
    SDL_FillRect(block_solution_surface,NULL,SDL_MapRGB(block_solution_surface->format,252,127,3));
    block_solution_texture = SDL_CreateTextureFromSurface(renderer,block_solution_surface);

    //PROGRAM BUTTONS
    //Set program rects and create texture from loaded images

    //Random
    button_random_rect.x = 60;
    button_random_rect.y = 230;
    button_random_rect.w = 100;
    button_random_rect.h = 50;

    button_random_surface = load_image("imgs/random_button3.png");
    button_random_texture = SDL_CreateTextureFromSurface(renderer,button_random_surface);

    //Reset
    button_reset_rect.x = 60;
    button_reset_rect.y = 170;
    button_reset_rect.w = 100;
    button_reset_rect.h = 50;

    button_reset_surface = load_image("imgs/reset_button3.png");
    button_reset_texture = SDL_CreateTextureFromSurface(renderer,button_reset_surface);

    //Start
    button_start_rect.x = 60;
    button_start_rect.y = 110;
    button_start_rect.w = 100;
    button_start_rect.h = 50;

    button_start_surface = load_image("imgs/start_button3.png");
    button_start_texture = SDL_CreateTextureFromSurface(renderer,button_start_surface);


    //Algo1
    button_algo1_surface = load_image("imgs/algo1_button.png");
    button_algo1_texture = SDL_CreateTextureFromSurface(renderer,button_algo1_surface);

    button_algo1_rect.x = 60;
    button_algo1_rect.y = 390;
    button_algo1_rect.w = 100;
    button_algo1_rect.h = 50;

    //Algo 2
    button_algo2_surface = load_image("imgs/algo2_button.png");
    button_algo2_texture = SDL_CreateTextureFromSurface(renderer,button_algo2_surface);

    button_algo2_rect.x = 60;
    button_algo2_rect.y = 450;
    button_algo2_rect.w = 100;
    button_algo2_rect.h = 50;

    //Chosen algo
    chosen_surface = load_image("imgs/chosen.png");
    chosen_texture = SDL_CreateTextureFromSurface(renderer,chosen_surface);

    //Start and Target block values
    start_set = true;
    target_set = true;

}

program::~program()
{
    SDL_FreeSurface(surface_bg);
    SDL_FreeSurface(button_random_surface);
    SDL_FreeSurface(button_start_surface);
    SDL_FreeSurface(button_reset_surface);
    SDL_FreeSurface(button_algo1_surface);
    SDL_FreeSurface(button_algo2_surface);
    SDL_FreeSurface(chosen_surface);

    SDL_DestroyTexture(texture_bg);
    SDL_DestroyTexture(button_random_texture);
    SDL_DestroyTexture(button_reset_texture);
    SDL_DestroyTexture(button_start_texture);
    SDL_DestroyTexture(button_algo1_texture);
    SDL_DestroyTexture(button_algo2_texture);
    SDL_DestroyTexture(chosen_texture);
}

//Function for loading an image
SDL_Surface *program::load_image(const char *filename){

    SDL_Surface *tmp = IMG_Load(filename);

    if(tmp == NULL)
        std::cout << "Error loading image" << std::endl;

    return tmp;
}

//Regulate constant frame rate
void program::regulate_fps(Uint32 start){
    if(1000 / FRAMES_PER_SECOND > (SDL_GetTicks() - start))
        SDL_Delay(1000 / FRAMES_PER_SECOND - (SDL_GetTicks() -start));
}

//Check if a button is clicked
bool program::check_button_click(int x,int y,SDL_Rect *rect){

    bool ans = false;

    if( ( x >= rect->x and x <= rect->x + rect->w) &&
    ( y >= rect->y and y <= rect->y + rect->h) )
        ans = true;

    return ans;
}

//Event handling function
void program::handle_events(){

    SDL_Event event;

    while(SDL_PollEvent(&event)){

        switch(event.type){

            case SDL_QUIT:
                is_running = false;
                break;


            case SDL_KEYDOWN:
                switch(event.key.keysym.sym){

                    case SDLK_1:
                        scale_current = 1;
                        SDL_SetWindowSize(window,SCREEN_WIDTH*scale_current,SCREEN_HEIGHT*scale_current);
                        SDL_RenderSetScale(renderer,scale_current,scale_current);
                        break;

                    case SDLK_2:
                        scale_current = 2;
                        SDL_SetWindowSize(window,SCREEN_WIDTH*scale_current,SCREEN_HEIGHT*scale_current);
                        SDL_RenderSetScale(renderer,scale_current,scale_current);
                        break;

                    case SDLK_3:
                        scale_current = 3;
                        SDL_SetWindowSize(window,SCREEN_WIDTH*scale_current,SCREEN_HEIGHT*scale_current);
                        SDL_RenderSetScale(renderer,scale_current,scale_current);
                        break;
                }

                break;

            case SDL_MOUSEBUTTONDOWN:
                switch(event.button.button){

                    case SDL_BUTTON_LEFT:
                        mouse_click = true;
                        std::cout << "Mouse coords x,y: " << mouse_x << "," << mouse_y << std::endl;
                        break;
                }

                break;

            case SDL_MOUSEMOTION:
                mouse_x = event.motion.x/scale_current;
                mouse_y = event.motion.y/scale_current;

        }

    }
}

//Create blocks
void program::make_blocks(){

    //Create block objects
    for(int i=0; i<grid_size; i++){
        for(int j=0; j<grid_size; j++){

            block_vec.push_back(new block(1+grid_x+line_width*i,1+grid_y+line_width*j,i,j,
                                block_size,block_unfilled_texture,block_filled_texture,
                                block_start_texture,block_target_texture,block_visited_texture,block_solution_texture));

        }
    }

    //Set Start and Target block, Initially
    //Start at pos 4,14
    //Target at    25,14
    for(int i=0; i<block_vec.size(); i++){
        if(block_vec[i]->get_x() == 4 && block_vec[i]->get_y() == 14)
            block_vec[i]->set_type(0);
        else if(block_vec[i]->get_x() == 25 && block_vec[i]->get_y() == 14)
            block_vec[i]->set_type(1);
    }
}

//Draw function for blocks
void program::show_blocks(){

    for(int i=0; i<block_vec.size(); i++){
        block_vec[i]->show_block(renderer);
    }
}

//Prints content of given STL vector
void program::print_vector(std::vector<int> vec){

    for(int i=0; i<vec.size(); i++){
        std::cout << vec[i];
    }

    std::cout << std::endl;
}

//DRAW RECTANGULAR GRID, STARTING FROM X,Y, NUMBER OF LINES, SPACE BETWEEN LINES
void program::draw_grid(int x,int y,int num,int space,int len){

    SDL_SetRenderDrawColor(renderer,45,44,47,255);

    for(int i=0; i<num; i++){

        //Horizonal
        SDL_RenderDrawLine(renderer,x,y+space*i,x+len,y+space*i);
        //Vertical
        SDL_RenderDrawLine(renderer,x+space*i,y,x+space*i,y+len);

    }
}

//Reset to initial state
void program::reset(){

    for(int i=0; i<block_vec.size(); i++){
        if(block_vec[i]->get_x() == 4 && block_vec[i]->get_y() == 14)
            block_vec[i]->set_type(0);
        else if(block_vec[i]->get_x() == 25 && block_vec[i]->get_y() == 14)
            block_vec[i]->set_type(1);
        else{
            block_vec[i]->set_type(2);
        }

        block_vec[i]->set_active(0);
        block_vec[i]->set_visited(0);
        block_vec[i]->set_visited_2(0);
        block_vec[i]->set_solution(0);
        block_vec[i]->clear_path();
    }

    start_set = true;
    target_set = true;

}

//If start pressed, clear previous solutions
void program::clear_start(){

    for(int i=0; i<block_vec.size(); i++){
        block_vec[i]->set_visited(0);
        block_vec[i]->set_visited_2(0);
        block_vec[i]->set_solution(0);
        block_vec[i]->clear_path();
    }
}

//Randomize maze
void program::randomize(){

    int tmp = 0;
    reset();

    for(int i=0; i<block_vec.size(); i++){
        if(block_vec[i]->get_type() == 2){
            tmp = rand()%2;

            if(tmp == 1){
                block_vec[i]->set_active(1);
            }
        }
    }
}

//Create visualization of solution path
void program::set_solution(){

    std::vector<int> directions = solution_block->get_path();

    int x_curr = 0;
    int y_curr = 0;
    int tmp = 0;

    for(int i=0; i<block_vec.size(); i++){

        if(block_vec[i]->get_type() == 0){
            x_curr = block_vec[i]->get_x();
            y_curr = block_vec[i]->get_y();
            std::cout << "START x,y: " << x_curr << "," << y_curr << std::endl;
        }
    }

    for(int i=0; i<directions.size(); i++){
        //std::cout << directions[i] << " ";

        tmp = directions[i];

        for(int i=0; i<block_vec.size(); i++){

            if(tmp == 0){ // UP
                if(block_vec[i]->get_x() == x_curr && block_vec[i]->get_y() == y_curr-1){
                   // std::cout << "Goint UP, Now at: " << x_curr << "," << y_curr << std::endl;
                    block_vec[i]->set_solution(1);
                    y_curr--;
                    break;
                }
            }else if(tmp == 2){ // DOWN
                if(block_vec[i]->get_x() == x_curr && block_vec[i]->get_y() == y_curr+1){
                    //std::cout << "Going DOWN, Now at: " << x_curr << "," << y_curr << std::endl;
                    block_vec[i]->set_solution(1);
                    y_curr++;
                    break;
                }
            }else if(tmp == 1){ // RIGHT
                if(block_vec[i]->get_x() == x_curr+1 && block_vec[i]->get_y() == y_curr){
                   // std::cout << "Going RIGHT, Now at: " << x_curr << "," << y_curr << std::endl;
                    block_vec[i]->set_solution(1);
                    x_curr++;
                    break;
                }
            }else{ // LEFT
              //  std::cout << "Going LEFT, Now at: " << x_curr << "," << y_curr << std::endl;
                if(block_vec[i]->get_x() == x_curr-1 && block_vec[i]->get_y() == y_curr){
                    block_vec[i]->set_solution(1);
                    x_curr--;
                    break;
                }
            }
        }
    }

    std::cout << std::endl;

}


void program::program_logic(){

    if(mouse_click == true){

        //BUTTONS
        if(check_button_click(mouse_x,mouse_y,&button_reset_rect)){
            std::cout << "Reset Button Pressed" << std::endl;
            reset();
        }else if(check_button_click(mouse_x,mouse_y,&button_start_rect)){
            clear_start();
            std::cout << "Start Button Pressed" << std::endl;
            if(algo_chosen == 1){
                algorithm_lees();
            }else if(algo_chosen == 2){
                algorithm_improved();
            }
            set_solution();
        }else if(check_button_click(mouse_x,mouse_y,&button_random_rect)){
            std::cout << "Randomize Pressed" << std::endl;
            randomize();
        }else if(check_button_click(mouse_x,mouse_y,&button_algo1_rect)){
            std::cout << "Algo1 Pressed" << std::endl;
            algo_chosen = 1;
        }else if(check_button_click(mouse_x,mouse_y,&button_algo2_rect)){
            std::cout << "Algo2 Pressed" << std::endl;
            algo_chosen = 2;
        }


        //IF WE CLICK ON A BLOCK, Set start, target, active conditions
        for(int i=0; i<block_vec.size(); i++){
            if(block_vec[i]->is_clicked(mouse_x,mouse_y) == true){

                if(start_set == false && block_vec[i]->get_type() != 1){
                    block_vec[i]->set_type(0);
                    block_vec[i]->set_active(0);
                    start_set = true;
                }else if(target_set == false && block_vec[i]->get_type() != 0){
                    block_vec[i]->set_type(1);
                    block_vec[i]->set_active(0);
                    target_set = true;
                }else if(start_set == true && block_vec[i]->get_type() == 0){
                    block_vec[i]->set_type(2);
                    block_vec[i]->set_active(0);
                    start_set = false;
                }else if(target_set == true && block_vec[i]->get_type() == 1){
                    block_vec[i]->set_type(2);
                    block_vec[i]->set_active(0);
                    target_set = false;
                }else if(block_vec[i]->get_type() == 2){
                    block_vec[i]->toggle_block();
                }
            }
        }
    }

    mouse_click = false;
}

//show things on screen
void program::draw(){

    //Background
    SDL_RenderCopy(renderer, texture_bg, NULL, &rect_bg);

    //Grid
    draw_grid(grid_x,grid_y,grid_size+1,line_width,line_length);

    //BUTTONS
    //reset
    SDL_RenderCopy(renderer,button_reset_texture, NULL, &button_reset_rect);

    //Start
    SDL_RenderCopy(renderer,button_start_texture, NULL, &button_start_rect);

    //Random
    SDL_RenderCopy(renderer,button_random_texture, NULL, &button_random_rect);

    //Algos
    if(algo_chosen ==  1){
        SDL_RenderCopy(renderer,chosen_texture, NULL, &button_algo1_rect);
    }else if(algo_chosen == 2){
        SDL_RenderCopy(renderer,chosen_texture, NULL, &button_algo2_rect);
    }

    SDL_RenderCopy(renderer,button_algo1_texture, NULL, &button_algo1_rect);
    SDL_RenderCopy(renderer,button_algo2_texture, NULL, &button_algo2_rect);

    SDL_RenderCopy(renderer,button_algo2_texture, NULL, &button_algo2_rect);

    //Blocks
    show_blocks();

}

//LEES ALGORITHM, BFS-TYPE SHORTEST PATH SEARCH
void program::algorithm_lees(){

    if(target_set == 0 || start_set == 0){
        return;
    }

    std::queue<block*> my_queue;

    block* start_block;
    block* target_block;

    //Find starting block and target, 0-type, 1-type
    for(int i=0; i<block_vec.size(); i++){
        if(block_vec[i]->get_type() == 0){
            start_block = block_vec[i];
        }else if(block_vec[i]->get_type() == 1)
            target_block = block_vec[i];
    }

    start_block->set_visited(1);
    my_queue.push(start_block);

    while(!my_queue.empty()){

        block *block1 = my_queue.front();
        my_queue.pop();

        //WHEN TARGET IS FOUND, RETURN
        if(block1->get_type() == 1){
            solution_block = block1;
            return;
        }

        //CHECK POSSIBLE MOVEMENTS
        for(int i=0; i<block_vec.size(); i++){

            //CHECK IF BLOCK IS VALID AND NOT VISITED BEFORE
            //BLOCK IS ABOVE CURRENT
            if(block_vec[i]->get_x() == block1->get_x() && block_vec[i]->get_y()+1 == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),0); //0 FOR UP DIR
                    my_queue.push(block_vec[i]);
                }
            //BLOCK IS BELOW CURRENT
            }else if(block_vec[i]->get_x() == block1->get_x() && block_vec[i]->get_y()-1 == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),2); //2 FOR DOWN DIR
                    my_queue.push(block_vec[i]);
                }
            //BLOCK IS RIGHT OF CURRENT
            }else if(block_vec[i]->get_x()-1 == block1->get_x() && block_vec[i]->get_y() == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),1); //1 FOR RIGHT DIR
                    my_queue.push(block_vec[i]);
                }
            //BLOCK IS LEFT OF CURRENT
            }else if(block_vec[i]->get_x()+1 == block1->get_x() && block_vec[i]->get_y() == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),3); //3 FOR LEFT DIR
                    my_queue.push(block_vec[i]);
                }
            }
        }

        draw();
        SDL_RenderPresent(renderer);
        SDL_Delay(25);

    }
}

//IMPROVED BFS, SIMULATENEOUS SEARCH FROM START AND TARGET
void program::algorithm_improved(){

    if(target_set == 0 || start_set == 0){
        return;
    }

    std::queue<block*> my_queue;
    std::queue<block*> my_queue2;

    block* start_block;
    block* target_block;

    //Find starting block and target, 0-type, 1-type
    for(int i=0; i<block_vec.size(); i++){
        if(block_vec[i]->get_type() == 0){
            start_block = block_vec[i];
        }else if(block_vec[i]->get_type() == 1)
            target_block = block_vec[i];
    }

    start_block->set_visited(1);
    target_block->set_visited_2(1);
    my_queue.push(start_block);
    my_queue2.push(target_block);

    while(!my_queue.empty() and !my_queue2.empty()){

        block *block1 = my_queue.front();
        my_queue.pop();

        block *block2 = my_queue2.front();
        my_queue2.pop();

        //WHEN A BLOCK IS VISITED FROM BOTH DIRECTIONS, A SOLUTION PATH IS FOUND
        if( block1->get_visited() == 1 and block1->get_visited_2() == 1){

            std::vector<int> p1 = block1->get_path();
            std::vector<int> p2 = block1->get_path2();
            std::cout << "Path 1: ";
            print_vector(p1);

            std::cout << "Path 2: ";
            print_vector(p2);

            std::reverse(p2.begin(),p2.end());
            std::cout << "Path 2 reversed: ";
            print_vector(p2);

            p1.insert(std::end(p1),std::begin(p2),std::end(p2));
            std::cout << "Appended path: ";
            print_vector(p1);

            solution_block = block1;
            solution_block->set_path(p1);
            return;
        }

        //CHECK POSSIBLE MOVEMENTS FROM START
        for(int i=0; i<block_vec.size(); i++){

            //CHECK IF BLOCK IS VALID AND NOT VISITED BEFORE
            //BLOCK IS ABOVE CURRENT
            if(block_vec[i]->get_x() == block1->get_x() && block_vec[i]->get_y()+1 == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),0); //0 FOR UP DIR
                    my_queue.push(block_vec[i]);
                }
            //BLOCK IS BELOW CURRENT
            }else if(block_vec[i]->get_x() == block1->get_x() && block_vec[i]->get_y()-1 == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),2); //2 FOR DOWN DIR
                    my_queue.push(block_vec[i]);
                }
            //BLOCK IS RIGHT OF CURRENT
            }else if(block_vec[i]->get_x()-1 == block1->get_x() && block_vec[i]->get_y() == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),1); //1 FOR RIGHT DIR
                    my_queue.push(block_vec[i]);
                }
            //BLOCK IS LEFT OF CURRENT
            }else if(block_vec[i]->get_x()+1 == block1->get_x() && block_vec[i]->get_y() == block1->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited(1);
                    block_vec[i]->add_direction( block1->get_path(),3); //3 FOR LEFT DIR
                    my_queue.push(block_vec[i]);
                }
            }


            //CHECK IF BLOCK IS VALID AND NOT VISITED BEFORE
            //BLOCK IS ABOVE CURRENT
            if(block_vec[i]->get_x() == block2->get_x() && block_vec[i]->get_y()+1 == block2->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited_2() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited_2(1);
                    block_vec[i]->add_direction2( block2->get_path2(),0); //0 FOR UP DIR
                    my_queue2.push(block_vec[i]);
                }
            //BLOCK IS BELOW CURRENT
            }else if(block_vec[i]->get_x() == block2->get_x() && block_vec[i]->get_y()-1 == block2->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited_2() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited_2(1);
                    block_vec[i]->add_direction2( block2->get_path2(),2); //2 FOR DOWN DIR
                    my_queue2.push(block_vec[i]);
                }
            //BLOCK IS RIGHT OF CURRENT
            }else if(block_vec[i]->get_x()-1 == block2->get_x() && block_vec[i]->get_y() == block2->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited_2() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited_2(1);
                    block_vec[i]->add_direction2( block2->get_path2(),1); //1 FOR RIGHT DIR
                    my_queue2.push(block_vec[i]);
                }
            //BLOCK IS LEFT OF CURRENT
            }else if(block_vec[i]->get_x()+1 == block2->get_x() && block_vec[i]->get_y() == block2->get_y()){
                //BLOCK IS INACTIVE AND NOT_VISITED
                if(block_vec[i]->get_active() == 0 && block_vec[i]->get_visited_2() == 0){

                    //SET AS VISITED, UPDATE PATH
                    block_vec[i]->set_visited_2(1);
                    block_vec[i]->add_direction2( block2->get_path2(),3); //3 FOR LEFT DIR
                    my_queue2.push(block_vec[i]);
                }
            }

        }

        draw();
        SDL_RenderPresent(renderer);
        SDL_Delay(25);

    }
}

// MAIN PROGRAM
void program::start(){

    Uint32 start;
    srand(time(0));
    make_blocks();

    while(is_running){

        start = SDL_GetTicks();
        handle_events();
        program_logic();

        draw();
        SDL_RenderPresent(renderer);
        regulate_fps(start);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

}
