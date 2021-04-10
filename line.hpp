#ifndef LINE_H 
#define LINE_H

#include "SFML/Graphics.hpp"
#include <cmath>

class Line : public sf::Drawable

{
    public:

        Line(){
            
        }

        Line(sf::Vector2f _p1, sf::Vector2f _p2);

        float get_length();

        sf::Vector2f p1;

        sf::Vector2f p2;

        sf::Vertex line[2];
    
    private:
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
        {
            target.draw(this->line, 2, sf::Lines, states);   
        }

};

#endif