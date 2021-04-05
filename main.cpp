#include <SFML/Graphics.hpp>
#include <vector>

#include "world.hpp"

std::vector<sf::Shape> shapes;

int main()
{

    std::unique_ptr<sf::RenderWindow> window(new sf::RenderWindow(sf::VideoMode(1024, 768), "My window"));
        
    std::unique_ptr<World> world (new World(&*window));
    
    world->run();

    return 0;
}