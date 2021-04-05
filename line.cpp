#include "line.hpp"

Line::Line(sf::Vector2f _p1, sf::Vector2f _p2){
    this->p1 = _p1;
    this->p2 = _p2;
    this->line[0] = this->p1;
    this->line[1] = this->p2;
}