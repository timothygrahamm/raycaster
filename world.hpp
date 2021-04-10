#ifndef WORLD_H 
#define WORLD_H

#include "SFML/Graphics.hpp"
#include "line.hpp"
#include <iostream>
#include <map>

#include <math.h>

#define PI 3.14159265

class World

{
    public:

        const int tile_interval = 64;
        const int fov_resolution = 64;

        const int fp_x = 550;
        const int fp_y = 0;
        const int world_map [8][8] = 

        {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 1, 0, 0, 1},
            {1, 0, 1, 0, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
        };

        World(sf::RenderWindow * _window);

        void run();

        std::vector<sf::RectangleShape> rectangles;

        std::vector<sf::RectangleShape> walls;

    protected:
    
        void draw();

        void logic();

        sf::Vector2f get_horizontal_intercept(float ray_angle);

        sf::Vector2f get_vertical_intercept(float ray_angle);

        sf::Vector2f get_closest_intercept(float ray_angle);

        sf::Color get_color(float ray_angle);

        std::vector<float> get_ray_angles(float player_angle);

        double get_line_length(sf::Vector2f p1, sf::Vector2f p2);

        void inputs();

        void add_shape();

        bool running = false;
        sf::RenderWindow * window;

        float player_angle = 3 * PI/2 + 0.3;
        sf::CircleShape player_shape;
        sf::CircleShape vision_shape;

        std::vector<Line> ray_intercepts;
        Line vision_line;

};

#endif