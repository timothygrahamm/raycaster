#include "line.hpp"

Line::Line(sf::Vector2f _p1, sf::Vector2f _p2){
    this->p1 = _p1;
    this->p2 = _p2;
    this->line[0] = this->p1;
    this->line[1] = this->p2;
}

float Line::get_length(){
    int y_distance = (p2.y - p1.y);
    int x_distance = (p2.x - p1.x);
    int line_length = pow(y_distance, 2) + pow(x_distance,2);
    if (line_length < 0){
        line_length = INT32_MAX;
    }
    return sqrt(line_length);
}