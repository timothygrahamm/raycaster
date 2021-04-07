#include "world.hpp"

World::World(sf::RenderWindow * window){
    this->window = window;
    this->player_shape = sf::CircleShape(2);
    this->player_shape.setPosition(300,300);
    this->player_shape.setFillColor(sf::Color::Cyan);

    this->vision_shape = sf::CircleShape(3);
    this->vision_shape.setPosition(1000,760);
    this->vision_shape.setFillColor(sf::Color::Red);

    int width = sizeof this->world_map[0] / sizeof(int);
    int height = sizeof this->world_map / sizeof this->world_map[0];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            if (this->world_map[i][j]==1){
                sf::RectangleShape tile = sf::RectangleShape(sf::Vector2f(tile_interval-1, tile_interval-1));
                tile.setPosition(sf::Vector2f(j*tile_interval, i*tile_interval));
                tile.setFillColor(sf::Color::Green);
                this->rectangles.push_back(tile);
            }
        }
    }
}

void World::run(){
    this->running = true;
    while(this->running){
        this->inputs();
        this->logic();
        this->draw();
    }
}

void World::logic(){
    
    sf::Vector2f h_intercept = this->get_horizontal_intercept(this->player_angle);
    sf::Vector2f v_intercept = this->get_vertical_intercept(this->player_angle);

    float h_intercept_distance = abs(this->player_shape.getPosition().y - h_intercept.y)/abs(this->player_shape.getPosition().x - h_intercept.x);
    float v_intercept_distance = abs(this->player_shape.getPosition().y - v_intercept.y)/abs(this->player_shape.getPosition().x - h_intercept.x);

    if (h_intercept_distance >= v_intercept_distance){
        this->vision_shape.setPosition(v_intercept);
    }
    else{
        this->vision_shape.setPosition(h_intercept);
    }  
    

}

sf::Vector2f World::get_horizontal_intercept(float ray_angle){
    const float inverse_tan = cos(ray_angle) / sin(ray_angle);
    int vision_steps = INT8_MAX;
    sf::Vector2f h_intercept = sf::Vector2f(INT8_MAX, INT8_MAX);
    if (sin(ray_angle)!=0){
        
        int x_offset;
        //if looking down
        if (sin(ray_angle)>0){
            int y_offset = ((int) this->player_shape.getPosition().y >> 6 << 6) + 64;
            int y_change = y_offset - player_shape.getPosition().y;

            float x_change = y_change * inverse_tan;
            x_offset = player_shape.getPosition().x + x_change;

            int x_grid = std::min(abs(int(floor(x_offset / this->tile_interval))), 8);
            int y_grid = y_offset / this->tile_interval - 1;

            vision_steps = 0;
            while(vision_steps < 8){
                if (this->world_map[y_grid][x_grid]==1 || this->world_map[y_grid+1][x_grid]==1){
                    h_intercept = sf::Vector2f(x_offset,y_offset);
                    break;
                }
                else{
                    y_offset += this->tile_interval;
                    y_change = this->tile_interval;
                    x_change = y_change * inverse_tan;
                    x_offset += x_change;

                    x_grid = std::min(abs(int(floor(x_offset / this->tile_interval))), 8);
                    y_grid = y_offset / this->tile_interval - 1;
                }
                vision_steps++;  
            }      
        }
        //if looking up
        if (sin(ray_angle)<0){
            int y_offset = (int) this->player_shape.getPosition().y >> 6 << 6;
            int y_change = y_offset - player_shape.getPosition().y;

            float x_change = y_change * inverse_tan;
            x_offset = player_shape.getPosition().x + x_change;

            int x_grid = std::min(abs(int(floor(x_offset / this->tile_interval))), 8);
            int y_grid = y_offset / this->tile_interval - 1;

            vision_steps = 0;
            while(vision_steps < 8){
                if (this->world_map[y_grid][x_grid]==1 || this->world_map[y_grid+1][x_grid]==1){
                    h_intercept = sf::Vector2f(x_offset,y_offset);
                    break;
                }
                else{
                    y_offset -= this->tile_interval;
                    y_change = -this->tile_interval;
                    x_change = y_change * inverse_tan;
                    x_offset += x_change;

                    x_grid = std::min(abs(int(floor(x_offset / this->tile_interval))), 8);
                    y_grid = y_offset / this->tile_interval - 1;
                }
                vision_steps++;
            }
            
        }
    }

    return h_intercept;
}

sf::Vector2f World::get_vertical_intercept(float ray_angle){
    const float tan = sin(ray_angle) / cos(ray_angle);
    int vision_steps = INT8_MAX;
    sf::Vector2f v_intercept = sf::Vector2f(INT8_MAX, INT8_MAX);
    if (cos(ray_angle)!=0){
        
        int y_offset;
        //if looking right
        if (cos(ray_angle)>0){
            int x_offset = ((int) this->player_shape.getPosition().x >> 6 << 6) + 64;
            int x_change = x_offset - player_shape.getPosition().x;

            float y_change = x_change * tan;
            y_offset = player_shape.getPosition().y + y_change;

            int y_grid = std::min(abs(int(floor(y_offset / this->tile_interval))), 8);
            int x_grid = x_offset / this->tile_interval - 1;

            vision_steps = 0;
            while(vision_steps < 8){
                if (this->world_map[y_grid][x_grid]==1 || this->world_map[y_grid][x_grid+1]==1){
                    v_intercept = sf::Vector2f(x_offset,y_offset);
                    break;
                }
                else{
                    x_offset += this->tile_interval;
                    x_change = this->tile_interval;
                    y_change = x_change * tan;
                    y_offset += y_change;

                    y_grid = std::min(abs(int(floor(y_offset / this->tile_interval))), 8);
                    x_grid = x_offset / this->tile_interval - 1;
                }
                vision_steps++;  
            }      
        }
        //if looking left
        if (cos(ray_angle)<0){
            int x_offset = (int) this->player_shape.getPosition().x >> 6 << 6;
            int x_change = x_offset - player_shape.getPosition().x;

            float y_change = x_change * tan;
            y_offset = player_shape.getPosition().y + y_change;

            int y_grid = std::min(abs(int(floor(y_offset / this->tile_interval))), 8);
            int x_grid = x_offset / this->tile_interval - 1;

            vision_steps = 0;
            while(vision_steps < 8){
                if (this->world_map[y_grid][x_grid]==1 || this->world_map[y_grid][x_grid+1]==1){
                    v_intercept = sf::Vector2f(x_offset,y_offset);
                    break;
                }
                else{
                    x_offset -= this->tile_interval;
                    x_change = -this->tile_interval;
                    y_change = x_change * tan;
                    y_offset += y_change;

                    y_grid = std::min(abs(int(floor(y_offset / this->tile_interval))), 8);
                    x_grid = x_offset / this->tile_interval - 1;
                }
                vision_steps++;
            }
            
        }
    }

    return v_intercept;
}

void World::draw(){
    this->window->clear(sf::Color::Black);

    for( const auto rectangle : this->rectangles ) {
        this->window->draw(rectangle);
    }

    this->window->draw(player_shape);
    this->window->draw(vision_shape);
    sf::Vector2f vision_direction = sf::Vector2f(
        player_shape.getPosition().x + cos(this->player_angle) * 50,
        player_shape.getPosition().y + sin(this->player_angle) * 50
    );
    Line test = Line(player_shape.getPosition(), vision_direction);

    this->window->draw(test);
    this->window->display();
}

void World::inputs(){
    sf::Event event;
    while (this->window->pollEvent(event)){
        
        if (event.type == sf::Event::Closed)
            this->window->close();

        if (event.type == sf::Event::KeyPressed){
            if (event.key.code == sf::Keyboard::Up){
                this->player_shape.setPosition(this->player_shape.getPosition().x + cos(this->player_angle) * 10, this->player_shape.getPosition().y + sin(this->player_angle) * 10);
            }
            else if (event.key.code == sf::Keyboard::Down){
                this->player_shape.setPosition(this->player_shape.getPosition().x - cos(this->player_angle) * 10, this->player_shape.getPosition().y - sin(this->player_angle) * 10);
            }
            else if (event.key.code == sf::Keyboard::Left){
                this->player_angle -= 0.05;
                if (this->player_angle < 0){
                    this->player_angle = 2 * PI;
                }
            }
            else if (event.key.code == sf::Keyboard::Right){
                this->player_angle += 0.05;
                if (this->player_angle > 2 * PI){
                    this->player_angle = 0;
                }
            }
        }
            
    }
}