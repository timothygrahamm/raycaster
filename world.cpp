#include "world.hpp"

World::World(sf::RenderWindow * window){
    this->window = window;
    this->player_shape = sf::CircleShape(2);
    this->player_shape.setPosition(300,300);
    this->player_shape.setFillColor(sf::Color::Cyan);

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
    this->ray_intercepts.clear();
    int current_x = this->fp_x;
    this->walls.clear();

    for( const auto ray_angle : this->get_ray_angles(this->player_angle) ) {
        this->ray_intercepts.push_back(Line(this->player_shape.getPosition(), get_closest_intercept(ray_angle)));
        
        Line ray = Line(this->player_shape.getPosition(), get_closest_intercept(ray_angle));
        float ray_length = ray.get_length();
        float angle_difference = this->player_angle - ray_angle; if (angle_difference < 0) angle_difference+= 2*PI; if (angle_difference > 2*PI) angle_difference -= 2*PI;
        ray_length = ray_length * cos(angle_difference);
        ray_length /= 24;

        sf::RectangleShape wall = sf::RectangleShape(sf::Vector2f(8, 700/(ray_length)));

        wall.setFillColor(this->get_color(ray_angle));

        wall.setPosition(sf::Vector2f(current_x, 450 - 350/(ray_length)));

        
        this->walls.push_back(wall);
        current_x +=8;
    }
    //this->ray_intercepts.push_back(Line(this->player_shape.getPosition(), get_closest_intercept(player_angle)));
}

std::vector<float> World::get_ray_angles(float player_angle){
    float min_angle = (player_angle - PI/6);
    if (min_angle < 0){
        min_angle = 2*PI + min_angle;
    }
    float current_angle = min_angle;

    std::vector<float> ray_angles = std::vector<float>();

    ray_angles.push_back(current_angle);

    for (int i = 1; i < this->fov_resolution; i++){
        current_angle += PI/3 / this->fov_resolution;
        if (current_angle > 2*PI){
            current_angle = current_angle - 2*PI;
        }
        ray_angles.push_back(current_angle);
    }

    return ray_angles;
}

double World::get_line_length(sf::Vector2f p1, sf::Vector2f p2){
    int y_distance = (p2.y - p1.y);
    int x_distance = (p2.x - p1.x);
    int line_length = pow(y_distance, 2) + pow(x_distance,2);
    if (line_length < 0){
        return INT32_MAX;
    }
    return line_length;
}

sf::Vector2f World::get_closest_intercept(float ray_angle){
    sf::Vector2f h_intercept = this->get_horizontal_intercept(ray_angle);
    sf::Vector2f v_intercept = this->get_vertical_intercept(ray_angle);

    double h_intercept_distance = this->get_line_length(this->player_shape.getPosition(), h_intercept);
    double v_intercept_distance = this->get_line_length(this->player_shape.getPosition(), v_intercept);

    if (h_intercept_distance >= v_intercept_distance){
        return v_intercept;
    }
    else{
        return h_intercept;
    }  
}

//TODO: make this not so stupid, dont need to recalculate the intercepts just to get the color
sf::Color World::get_color(float ray_angle){
    sf::Vector2f h_intercept = this->get_horizontal_intercept(ray_angle);
    sf::Vector2f v_intercept = this->get_vertical_intercept(ray_angle);

    double h_intercept_distance = this->get_line_length(this->player_shape.getPosition(), h_intercept);
    double v_intercept_distance = this->get_line_length(this->player_shape.getPosition(), v_intercept);

    if (h_intercept_distance >= v_intercept_distance){
        return sf::Color(0,0,250);
    }
    else{
        return sf::Color(0,0,150);
    }  
}

sf::Vector2f World::get_horizontal_intercept(float ray_angle){
    const float inverse_tan = cos(ray_angle) / sin(ray_angle);
    int vision_steps = INT32_MAX;
    sf::Vector2f h_intercept = sf::Vector2f(INT32_MAX, INT32_MAX);
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
    int vision_steps = INT32_MAX;
    sf::Vector2f v_intercept = sf::Vector2f(INT32_MAX, INT32_MAX);
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

    sf::RectangleShape roof = sf::RectangleShape(sf::Vector2f(1600-fp_x-28, 350));
    roof.setPosition(sf::Vector2f(fp_x,100));
    roof.setFillColor(sf::Color(100,100,100,255));

    sf::RectangleShape floor = sf::RectangleShape(sf::Vector2f(1600-fp_x-28, 350));
    floor.setPosition(sf::Vector2f(fp_x,450));
    floor.setFillColor(sf::Color(50,50,50,255));
    this->window->draw(roof);
    this->window->draw(floor);

    for( const auto rectangle : this->rectangles ) {
        this->window->draw(rectangle);
    }

    for( const auto ray : this->ray_intercepts ) {
        this->window->draw(ray);

    }

    for( const auto wall : this->walls ) {
        this->window->draw(wall);

    }

    this->window->draw(player_shape);
    sf::Vector2f vision_direction = sf::Vector2f(
        player_shape.getPosition().x + cos(this->player_angle) * 50,
        player_shape.getPosition().y + sin(this->player_angle) * 50
    );

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