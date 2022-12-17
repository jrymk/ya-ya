#ifndef YAYA_MODEL_H
#define YAYA_MODEL_H

#include "graphics.h"
#include <vector>

#define TILEMAP_COORD(x, y) sf::Vector2f(384 * x + 128 * y, 90.5 - 90.5 * x + 271.5 * y)

const std::vector<Graphics::Quad> modelEntity = {

};

const Graphics::Quad modelSelector[4] = {
        Graphics::Quad(
                ZDEPTH_GROUND_OVERLAY,
                UIVec(0, 0), sf::Vector2f(1045, 8064),
                UIVec(0.25, 0), sf::Vector2f(1045 + 128, 8064),
                UIVec(0.25, 0.25), sf::Vector2f(1045 + 128, 8064 + 128),
                UIVec(0, 0.25), sf::Vector2f(1045, 8064 + 128)
        ),
        Graphics::Quad(
                ZDEPTH_GROUND_OVERLAY,
                UIVec(0, 0), sf::Vector2f(1045, 8064),
                UIVec(-0.25, 0), sf::Vector2f(1045 + 128, 8064),
                UIVec(-0.25, 0.25), sf::Vector2f(1045 + 128, 8064 + 128),
                UIVec(0, 0.25), sf::Vector2f(1045, 8064 + 128)
        ),
        Graphics::Quad(
                ZDEPTH_GROUND_OVERLAY,
                UIVec(0, 0), sf::Vector2f(1045, 8064),
                UIVec(0.25, 0), sf::Vector2f(1045 + 128, 8064),
                UIVec(0.25, -0.25), sf::Vector2f(1045 + 128, 8064 + 128),
                UIVec(0, -0.25), sf::Vector2f(1045, 8064 + 128)
        ),
        Graphics::Quad(
                ZDEPTH_GROUND_OVERLAY,
                UIVec(0, 0), sf::Vector2f(1045, 8064),
                UIVec(-0.25, 0), sf::Vector2f(1045 + 128, 8064),
                UIVec(-0.25, -0.25), sf::Vector2f(1045 + 128, 8064 + 128),
                UIVec(0, -0.25), sf::Vector2f(1045, 8064 + 128)
        )
};

const std::vector<Graphics::Quad> modelPlayer = {
        Graphics::Quad(
                ZDEPTH_GROUND_OVERLAY,
                UIVec(-0.5, 0.5), TILEMAP_COORD(2, 1),
                UIVec(0.5, 0.5), TILEMAP_COORD(3, 1),
                UIVec(0.5, -0.5), TILEMAP_COORD(3, 2),
                UIVec(-0.5, -0.5), TILEMAP_COORD(2, 2)
        ),
        Graphics::Quad(
                ZDEPTH_ABOVE_GROUND,
                UIVec(-0.5, 1.5), TILEMAP_COORD(0, 2),
                UIVec(0.5, 1.5), TILEMAP_COORD(1, 2),
                UIVec(0.5, -0.5), TILEMAP_COORD(1, 4),
                UIVec(-0.5, -0.5), TILEMAP_COORD(0, 4)
        )
};

const Graphics::Quad modelGrownDuck =
        Graphics::Quad(ZDEPTH_ABOVE_GROUND,
                       UIVec(-0.5, 1.5), TILEMAP_COORD(25, 10),
                       UIVec(0.5, 1.5), TILEMAP_COORD(30, 10),
                       UIVec(0.5, -0.5), TILEMAP_COORD(30, 18),
                       UIVec(-0.5, -0.5), TILEMAP_COORD(25, 18));

const std::vector<Graphics::Quad> modelTruck = {
        Graphics::Quad(
                ZDEPTH_ABOVE_GROUND,
                UIVec(-3.5, 7), TILEMAP_COORD(4, 6),
                UIVec(4.5, 7), TILEMAP_COORD(12, 6),
                UIVec(4.5, -2), TILEMAP_COORD(12, 15),
                UIVec(-3.5, -2), TILEMAP_COORD(4, 15)
        )
};

const std::vector<Graphics::Quad> modelEgg[3] = {
        {
                Graphics::Quad(
                        ZDEPTH_ABOVE_GROUND,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-3, 9),
                        UIVec(0.5, 0.5), TILEMAP_COORD(-2, 9),
                        UIVec(0.5, -0.5), TILEMAP_COORD(-2, 10),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-3, 10)
                )
        },
        {
                Graphics::Quad(
                        ZDEPTH_ABOVE_GROUND,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-2, 9),
                        UIVec(0.5, 0.5), TILEMAP_COORD(-1, 9),
                        UIVec(0.5, -0.5), TILEMAP_COORD(-1, 10),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-2, 10)
                )
        },
        {
                Graphics::Quad(
                        ZDEPTH_ABOVE_GROUND,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-1, 9),
                        UIVec(0.5, 0.5), TILEMAP_COORD(0, 9),
                        UIVec(0.5, -0.5), TILEMAP_COORD(0, 10),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-1, 10)
                )
        }
};

const std::vector<Graphics::Quad> modelEggCarton = {
        {
                Graphics::Quad(
                        ZDEPTH_ABOVE_GROUND - ZDEPTH_LAYER * 2,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-3, 10),
                        UIVec(0.5, 0.5), TILEMAP_COORD(-2, 10),
                        UIVec(0.5, -0.5), TILEMAP_COORD(-2, 11),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-3, 11)
                ),
                Graphics::Quad(
                        ZDEPTH_ABOVE_GROUND,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-2, 10),
                        UIVec(0.5, 0.5), TILEMAP_COORD(-1, 10),
                        UIVec(0.5, -0.5), TILEMAP_COORD(-1, 11),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-2, 11)
                ),
                Graphics::Quad(
                        ZDEPTH_ABOVE_GROUND + ZDEPTH_LAYER * 2,
                        UIVec(-0.5, 0.5), TILEMAP_COORD(-1, 10),
                        UIVec(0.5, 0.5), TILEMAP_COORD(0, 10),
                        UIVec(0.5, -0.5), TILEMAP_COORD(0, 11),
                        UIVec(-0.5, -0.5), TILEMAP_COORD(-1, 11)
                )
        }
};

const std::vector<Graphics::Quad> modelNPC = {
        // Why not use texture ... ?
};

const Graphics::Quad modelGrass = {
        Graphics::Quad(
                ZDEPTH_GROUND,
                UIVec(0., 1.), TILEMAP_COORD(1, 20),
                UIVec(1., 1.), TILEMAP_COORD(11, 20),
                UIVec(1., 0.), TILEMAP_COORD(11, 30),
                UIVec(0., 0.), TILEMAP_COORD(1, 30)
        )
};

const std::vector<Graphics::Quad> modelDirt = {
        Graphics::Quad(
                ZDEPTH_GROUND,
                UIVec(0., 1.), TILEMAP_COORD(2, 3),
                UIVec(1., 1.), TILEMAP_COORD(3, 3),
                UIVec(1., 0.), TILEMAP_COORD(3, 4),
                UIVec(0., 0.), TILEMAP_COORD(2, 4)
        )
};

const std::vector<Graphics::Quad> modelMoai = {
        Graphics::Quad(
                ZDEPTH_ABOVE_GROUND,
                UIVec(0., 2.), TILEMAP_COORD(3, 2),
                UIVec(2., 2.), TILEMAP_COORD(5, 2),
                UIVec(2., 0.), TILEMAP_COORD(5, 4),
                UIVec(0., 0.), TILEMAP_COORD(3, 4)
        )
};


/// IMAGES
const Graphics::Image imageAltKey(sf::Vector2f(0, 7932), sf::Vector2f(260, 260), 24. / 256.); // actual size 256*256
const Graphics::Image imageMouseLeftBtn(sf::Vector2f(260, 7932), sf::Vector2f(260, 260), 24. / 256.);
const Graphics::Image imageMouseRightBtn(sf::Vector2f(520, 7932), sf::Vector2f(260, 260), 24. / 256.);
const Graphics::Image imageMouseBase(sf::Vector2f(780, 7932), sf::Vector2f(260, 260), 24. / 256.);

#endif //YAYA_MODEL_H
