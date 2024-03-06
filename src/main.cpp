#include "InputParser.h"
#include "Game.h"

int main(int argc, char* argv[])
{
    InputParser input(argc, argv);

    std::string mapPath = "assets/map/map.txt";  // default map path

    if (input.cmdOptionExists("-h") || input.cmdOptionExists("--version"))
        std::cout << "Version 1.0" << std::endl;

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

    Game game;
    if (!game.init(mapPath))
    {
        return EXIT_FAILURE;
    }

    return game.run();
}