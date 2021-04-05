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
        const int world_map [8][8] = 

        {
            {1, 1, 1, 1, 1, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 0, 1, 1, 1, 0, 1},
            {1, 0, 1, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 1, 1, 1, 1, 1},
        };

        World(sf::RenderWindow * _window);

        void run();

        std::vector<sf::RectangleShape> rectangles;

    protected:
    
        void draw();

        void logic();

        void inputs();

        void add_shape();

        bool running = false;
        sf::RenderWindow * window;

        float player_angle = 3 * PI/2 + 0.3;
        sf::CircleShape player_shape;
        sf::CircleShape vision_shape;
        Line vision_line;

};

#endif