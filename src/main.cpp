#include "InputParser.h"
#include "Game.h"

int main(int argc, char* argv[])
{
    InputParser input(argc, argv);

    // version argument
    if (input.cmdOptionExists("-v") || input.cmdOptionExists("--version"))
        std::cout << "Version 1.0" << std::endl;

    
    Game game;
    if (!game.init())
    {
        return EXIT_FAILURE;
    }

    return game.run();
}