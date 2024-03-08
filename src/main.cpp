#include "InputParser.h"
#include "Game.h"

int main(int argc, char* argv[])
{
    Game game;
    InputParser input(argc, argv);

    std::string mapPath = "assets/map/map.txt";  // default map path

    if (input.cmdOptionExists("-v") || input.cmdOptionExists("--version")) 
    {
        std::cout << "Version 1.0" << std::endl;
        return EXIT_SUCCESS;
    }
    if (input.cmdOptionExists("-s") || input.cmdOptionExists("--skip-intro"))
        game.skipIntro();
    if (input.cmdOptionExists("-m") || input.cmdOptionExists("--map"))
    {
        mapPath = input.getCmdOption("-m");
        if (mapPath.empty())
        {
            mapPath = input.getCmdOption("--map");
            if (mapPath.empty())
            {
                std::cerr << "Invalid map path" << std::endl;
                return EXIT_FAILURE;
            }
        }
    }
    
    std::cout << "Map: " << mapPath << std::endl;

    if (!game.init(mapPath))
    {
        return EXIT_FAILURE;
    }

    return game.run();
}