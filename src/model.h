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

const std::vector<Graphics::Quad> modelEgg[3] = {
        {
                Graphics::Quad(
                        0.85,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-3, 9),
                        UIVec(0.5, 0.5), TILEMAP_COORD(-2, 9),
                        UIVec(0.5, -0.5), TILEMAP_COORD(-2, 10),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-3, 10)
                )
        },
        {
                Graphics::Quad(
                        0.85,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-2, 9),
                        UIVec(0.5, 0.5), TILEMAP_COORD(-1, 9),
                        UIVec(0.5, -0.5), TILEMAP_COORD(-1, 10),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-2, 10)
                )
        },
        {
                Graphics::Quad(
                        0.85,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-1, 9),
                        UIVec(0.5, 0.5), TILEMAP_COORD(0, 9),
                        UIVec(0.5, -0.5), TILEMAP_COORD(0, 10),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-1, 10)
                )
        }
};

const std::vector<Graphics::Quad> modelNPC = {
        // Why not use texture ... ?
};

const std::vector<Graphics::Quad> modelGrass[9] = {
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-5, 15),
                        UIVec(1., 1.), TILEMAP_COORD(-4, 15),
                        UIVec(1., 0.), TILEMAP_COORD(-4, 16),
                        UIVec(0., 0.), TILEMAP_COORD(-5, 16)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-4, 15),
                        UIVec(1., 1.), TILEMAP_COORD(-3, 15),
                        UIVec(1., 0.), TILEMAP_COORD(-3, 16),
                        UIVec(0., 0.), TILEMAP_COORD(-4, 16)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-3, 15),
                        UIVec(1., 1.), TILEMAP_COORD(-2, 15),
                        UIVec(1., 0.), TILEMAP_COORD(-2, 16),
                        UIVec(0., 0.), TILEMAP_COORD(-3, 16)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-5, 16),
                        UIVec(1., 1.), TILEMAP_COORD(-4, 16),
                        UIVec(1., 0.), TILEMAP_COORD(-4, 17),
                        UIVec(0., 0.), TILEMAP_COORD(-5, 17)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-4, 16),
                        UIVec(1., 1.), TILEMAP_COORD(-3, 16),
                        UIVec(1., 0.), TILEMAP_COORD(-3, 17),
                        UIVec(0., 0.), TILEMAP_COORD(-4, 17)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-3, 16),
                        UIVec(1., 1.), TILEMAP_COORD(-2, 16),
                        UIVec(1., 0.), TILEMAP_COORD(-2, 17),
                        UIVec(0., 0.), TILEMAP_COORD(-3, 17)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-5, 17),
                        UIVec(1., 1.), TILEMAP_COORD(-4, 17),
                        UIVec(1., 0.), TILEMAP_COORD(-4, 18),
                        UIVec(0., 0.), TILEMAP_COORD(-5, 18)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-4, 17),
                        UIVec(1., 1.), TILEMAP_COORD(-3, 17),
                        UIVec(1., 0.), TILEMAP_COORD(-3, 18),
                        UIVec(0., 0.), TILEMAP_COORD(-4, 18)
                )
        },
        {
                Graphics::Quad(
                        0.0,
                        UIVec(0., 1.), TILEMAP_COORD(-3, 17),
                        UIVec(1., 1.), TILEMAP_COORD(-2, 17),
                        UIVec(1., 0.), TILEMAP_COORD(-2, 18),
                        UIVec(0., 0.), TILEMAP_COORD(-3, 18)
                )
        }
};

const std::vector<Graphics::Quad> modelDirt = {
        Graphics::Quad(
                0.0,
                UIVec(0., 1.), TILEMAP_COORD(2, 3),
                UIVec(1., 1.), TILEMAP_COORD(3, 3),
                UIVec(1., 0.), TILEMAP_COORD(3, 4),
                UIVec(0., 0.), TILEMAP_COORD(2, 4)
        )
};

const std::vector<Graphics::Quad> modelStone = {
        Graphics::Quad(
                0.0,
                UIVec(0., 2.), TILEMAP_COORD(3, 2),
                UIVec(1., 2.), TILEMAP_COORD(4, 2),
                UIVec(1., 0.), TILEMAP_COORD(4, 4),
                UIVec(0., 0.), TILEMAP_COORD(3, 4)
        )
};


#endif //YAYA_MODEL_H
