#pragma once

#include "Camera.h"
#include "Map.h"
#include "Texture.h"

#include <array>
#include <iostream>
#include <optional>
#include <vector>

class IRenderer;

class RayCaster
{
public:
    RayCaster(Camera& camera, Map& map);

    bool showGame = false;

    bool init(IRenderer& renderer);
    void drawEverything(IRenderer& renderer);
    void toggleMapDraw();
    void toggleNightMode();

    void shoot() { _shoot = true; }
private:
    enum class WallSide
    {
        VERTICAL,
        HORIZONTAL
    };

    static constexpr size_t MAP_SQUARE_SIZE = 32;
    static constexpr uint32_t DARKEN_MASK = 8355711;

    static constexpr uint32_t rgbToUint32(const uint8_t r, const uint8_t g, const uint8_t b);
    static uint32_t shadeTexelByDistance(const uint32_t texelToShade, const float distance);

    void drawTop();
    void drawBottom();
    void drawWalls();
    void drawMap();
    void drawMapSquares();
    void drawMapPlayer();
    void drawMapDebugLines(const Vector2<float>& mapPlayerPosition);

    void drawUI();

    bool _shoot = false;
    unsigned int _shootTime = 0;

    std::pair<Vector2<int>, Vector2<float>> calculateInitialStep(
        const Vector2<int>& mapSquarePosition,
        const Vector2<float>& rayDirection,
        const Vector2<float>& rayStepDistance);

    std::pair<WallSide, size_t> performDDA(
        const Vector2<int>& stepDirection,
        const Vector2<float>& rayStepDistance,
        Vector2<int>& mapSquarePositionInOut,
        Vector2<float>& sideDistanceInOut) const;

    float calculateWallDistance(
        const WallSide side,
        const Vector2<int>& mapSquarePosition,
        const Vector2<int>& stepDirection,
        const Vector2<float>& rayDirection);

    /**
     * Calculate lowest and highest pixel to fill in current column.
     */
    [[nodiscard]] std::pair<int, int> calculateDrawLocations(const int wallColumnHeight) const;

    /**
     * Performs affine texture mapping
     */
    void drawTexturedColumn(
        const size_t x,
        const size_t mapSquareIndex,
        const WallSide side,
        const float wallDistance,
        const int wallColumnHeight,
        Vector2<float> ray,
        const int drawStart,
        const int drawEnd);

    Texture* mapIndexToWallTexture(const size_t index);
    void plotPixel(const uint16_t x, const uint16_t y, const uint32_t pixel);

    Camera& camera_;
    Map& map_;
    std::optional<Texture> topTexture_; 

    std::optional<Texture> alphabetTexture; // Think it as a pixel font 7x12
    std::vector<int> calculateCharsIndex(const char* text);
    void drawText(const char* text, int posx, int posy, bool transparent);

    void drawImage(Texture& texture, int posx, int posy, bool transparent);

    int hp = 100;   // here for now
    int ammo = 35;  // here for now

    std::vector<std::pair<char, int>> charIndices;

    std::optional<Texture> topTextureNight_;
    std::optional<Texture> bottomTexture_;
    std::array<std::optional<Texture>, 4> wallTextures_;
    std::vector<uint32_t> drawBuffer_;
    uint16_t screenWidth_, screenHeight_;

    // UI textures
    std::optional<Texture> logoTexture_;
    std::optional<Texture> gunTexture_;
    std::optional<Texture> gunFlareTexture_;

    bool overviewMapOn_{false};
    bool drawDarkness_{false};
    float cameraLineDistance_;
    float planeLeftDistance_;
    float planeRightDistance_;
};

