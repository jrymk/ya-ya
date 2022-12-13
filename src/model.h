#ifndef YAYA_MODEL_H
#define YAYA_MODEL_H

#include "graphics.h"
#include <vector>

#define TILEMAP_COORD(x, y) sf::Vector2f(384 * x + 128 * y, 90.5 - 90.5 * x + 271.5 * y)

const std::vector<Graphics::Quad> modelEntity = {

};

const std::vector<Graphics::Quad> modelPlayer = {
        Graphics::Quad(
                0.80,
                UIVec(-0.5, 0.5), TILEMAP_COORD(2, 1),
                UIVec(0.5, 0.5), TILEMAP_COORD(3, 1),
                UIVec(0.5, -0.5), TILEMAP_COORD(3, 2),
                UIVec(-0.5, -0.5), TILEMAP_COORD(2, 2)
        ),
        Graphics::Quad(
                0.91,
                UIVec(-0.5, 1.5), TILEMAP_COORD(0, 2),
                UIVec(0.5, 1.5), TILEMAP_COORD(1, 2),
//                UIVec(0.5, 0.5), TILEMAP_COORD(1, 3),
//                UIVec(-0.5, 0.5), TILEMAP_COORD(0, 3)
//        ),
//        Graphics::Quad(
//                0.91,
//                UIVec(-0.5, 0.5), TILEMAP_COORD(0, 3),
//                UIVec(0.5, 0.5), TILEMAP_COORD(1, 3),
                UIVec(0.5, -0.5), TILEMAP_COORD(1, 4),
                UIVec(-0.5, -0.5), TILEMAP_COORD(0, 4)
        )
};

const std::vector<Graphics::Quad> modelDuck = {
        Graphics::Quad(0.80,
                       UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 7, 90.5 + 90.5 * 1),
                       UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 7, 0 + 90.5 * 1),
                       UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 7, 271.5 + 90.5 * 1),
                       UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 7, 362 + 90.5 * 1),
                       0.0),
        Graphics::Quad(0.90,
                       UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 4, 90.5 + 90.5 * 2),
                       UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 4, 0 + 90.5 * 2),
                       UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 4, 271.5 + 90.5 * 2),
                       UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 4, 362 + 90.5 * 2)),
        Graphics::Quad(0.91,
                       UIVec(-0.5, 0.5), sf::Vector2f(0 + 128, 90.5 + 90.5 * 3),
                       UIVec(0.5, 0.5), sf::Vector2f(384 + 128, 0 + 90.5 * 3),
                       UIVec(0.5, -0.5), sf::Vector2f(512 + 128, 271.5 + 90.5 * 3),
                       UIVec(-0.5, -0.5), sf::Vector2f(128 + 128, 362 + 90.5 * 3))
};

const std::vector<Graphics::Quad> modelEgg = {
        Graphics::Quad(
                0.85,
                UIVec(-0.5, 0.5), sf::Vector2f(0 + 128 * 10, 90.5 + 90.5 * 0),
                UIVec(0.5, 0.5), sf::Vector2f(384 + 128 * 10, 0 + 90.5 * 0),
                UIVec(0.5, -0.5), sf::Vector2f(512 + 128 * 10, 271.5 + 90.5 * 0),
                UIVec(-0.5, -0.5), sf::Vector2f(128 + 128 * 10, 362 + 90.5 * 0)
        )
};

const std::vector<Graphics::Quad> modelTile = {
        Graphics::Quad(
                0.0,
                UIVec(0., 0.), sf::Vector2f(0 + 128 * 8, 90.5 + 90.5 * 4),
                UIVec(1., 0.), sf::Vector2f(384 + 128 * 8, 0 + 90.5 * 4),
                UIVec(1., 1.), sf::Vector2f(512 + 128 * 8, 271.5 + 90.5 * 4),
                UIVec(0., 1.), sf::Vector2f(128 + 128 * 8, 362 + 90.5 * 4)
        )
};


#endif //YAYA_MODEL_H
