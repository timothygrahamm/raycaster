#include <SFML/Graphics.hpp>
#include <vector>

#include "world.hpp"

std::vector<sf::Shape> shapes;

int main()
{

    std::unique_ptr<sf::RenderWindow> window(new sf::RenderWindow(sf::VideoMode(1600, 900), "My window"));
        
    std::unique_ptr<World> world (new World(&*window));
    
    world->run();

    return 0;
}