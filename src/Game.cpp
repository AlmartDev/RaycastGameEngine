#include "Game.h"

#include "SDLRenderer.h"

#include <SDL2/SDL.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <optional>

#ifdef __EMSCRIPTEN__
extern "C" void emscriptenCallback(Game* game)
{
    game->runEmscriptenIteration();
}
#endif

Game::Game()
    : camera_(INITIAL_POSITION, INITIAL_DIRECTION, INITIAL_FIELD_OF_VIEW, map_)
    , raycaster_(camera_, map_)
    , renderer_(std::make_unique<SDLRenderer>())
{
}

Game::~Game()
{
    SDL_Quit();
}

bool Game::init(std::string mapPath)
{
    bool success = renderer_->init(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "RayCast Engine");
    if (!success)
    {
        return false;
    }

    std::optional<Map> map = Map::create(mapPath);
    if (!map.has_value())
    {
        return false;
    }
    map_ = map.value();

    if (!raycaster_.init(*renderer_))
    {
        std::cerr << "Error initializing raycaster engine." << std::endl;
        return false;
    }

    camera_.rotationSpeed(CURSOR_TURN_SPEED);

    return true;
}

void Game::loadMap(std::string mapPath)
{
    std::optional<Map> map = Map::create(mapPath);
    map_ = map.value();
}

int Game::run()
{
    running_ = true;
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop_arg(reinterpret_cast<em_arg_callback_func>(emscriptenCallback), this, 0, true);
#else
    using namespace std::chrono;
    using namespace std::chrono_literals;

    constexpr nanoseconds timeStep(16ms);

    high_resolution_clock::time_point previousTime = high_resolution_clock::now();
    nanoseconds lag(0ns);

    while (running_)
    {
        high_resolution_clock::time_point currentTime = high_resolution_clock::now();
        auto elapsedTime = currentTime - previousTime;
        previousTime = currentTime;
        lag += elapsedTime;

        event();
        while (lag >= timeStep)
        {
            update();
            lag -= timeStep;
        }

        render();
    }
#endif

    return EXIT_SUCCESS;
}

#ifdef __EMSCRIPTEN__
void Game::runEmscriptenIteration()
{
    event();
    update();
    render();
}
#endif

void Game::event()
{
    // TODO: Decouple SDL event handling
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);

    eventState_.moveForward = keystate[SDL_SCANCODE_UP] || keystate[SDL_SCANCODE_W];
    eventState_.moveBackward = keystate[SDL_SCANCODE_DOWN] || keystate[SDL_SCANCODE_S];
    eventState_.turnLeft = keystate[SDL_SCANCODE_LEFT];
    eventState_.turnRight = keystate[SDL_SCANCODE_RIGHT];
    eventState_.strafeLeft = keystate[SDL_SCANCODE_A];
    eventState_.strafeRight = keystate[SDL_SCANCODE_D];
    eventState_.running = keystate[SDL_SCANCODE_LSHIFT];

    eventState_.shoot = keystate[SDL_SCANCODE_SPACE];

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                running_ = false;
                break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                    case SDLK_1:    // testing
                        loadMap("assets/map/map.txt");
                        break;
                    case SDLK_2:
                        loadMap("assets/map/map1.txt");
                        break;
                    case SDLK_3:
                        loadMap("assets/map/map2.txt");
                        break;
                }
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                    case SDLK_g:
                        skipIntro();    
                        break;
                    case SDLK_ESCAPE:
                        running_ = false;
                        break;
                    // HACK: Safe for single button toggles, can be more pretty though
                    case SDLK_m:
                        raycaster_.toggleMapDraw();
                        break;
                    case SDLK_n:
                        raycaster_.toggleNightMode();
                        break;
                    case SDLK_i:
                        raycaster_.toggleInverseColor();
                        break;
                }
        }
    }
}

void Game::update()
{
    camera_.movementSpeed(eventState_.running ? RUN_MOVEMENT_SPEED : BASE_MOVEMENT_SPEED);

    if (eventState_.moveForward)
    {
        camera_.move(Camera::DIRECTION_FORWARD);
    }
    if (eventState_.moveBackward)
    {
        camera_.move(Camera::DIRECTION_BACKWARD);
    }
    if (eventState_.turnLeft)
    {
        camera_.turn(Camera::DIRECTION_LEFT);
    }
    if (eventState_.turnRight)
    {
        camera_.turn(Camera::DIRECTION_RIGHT);
    }
    if (eventState_.strafeLeft)
    {
        camera_.strafe(Camera::DIRECTION_LEFT);
    }
    if (eventState_.strafeRight)
    {
        camera_.strafe(Camera::DIRECTION_RIGHT);
    }

    // New stuff!
    if (eventState_.shoot) {
        raycaster_.shoot();
    }
}

void Game::render()
{
    renderer_->clearScreen();
    raycaster_.drawEverything(*renderer_);
    renderer_->refreshScreen();
}

