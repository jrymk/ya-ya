#include "gamecontroller.h"
#include "eggcarton.h"
#include "egg.h"
#include "player.h"
#include "truck.h"
#include "duck.h"

GameController::GameController(Game* game): game(game) {}

void GameController::setGame(Game* game) {
    this->game = game;
}

void GameController::handleAction(GameController::GameActions action) {
    switch (action) {
        case BTN_START_NEW_GAME:
            startOfDayTp = Timer::getNow() + DAY_START_CUTSCENE_LENGTH;
            gameState = DAY_START_SCENE;
            cash = 0;
            dayCount = 1;
            startDaySequence();
            break;
        case BTN_START_LOAD_GAME:
            startOfDayTp.loadTimer();
            gameState = GAMEPLAY;  // TBD
    }
}

void GameController::update() {
    switch (gameState) {
        case DAY_START_SCENE:

            if (startOfDayTp.elapsed() >= 0.)
                gameState = GAMEPLAY;
            break;
        case GAMEPLAY:


            if (startOfDayTp.elapsed() >= DAY_LENGTH) {
                gameState = DAY_END_SCENE;
                endDaySequence();
            }
            break;
        case DAY_END_SCENE:
            if (startOfDayTp.elapsed() >= DAY_LENGTH + DAY_END_SCENE) {
                dayCount++;
                startOfDayTp = Timer::getNow() + DAY_START_CUTSCENE_LENGTH;
                gameState = DAY_START_SCENE;
                startDaySequence();
            }
            break;
    }
}

void GameController::loadToSaveFile() {
    game->load();
}

void GameController::loadTestWorld() {
    std::shared_ptr<Player> player(new Player(game));

    player->id = "player$player";
    player->opacity = 1.;
    auto &player_e = game->insertEntity(player);
    game->setPlayer(player);
//
//    for (int i = 0; i < 100; i++) {
//        std::shared_ptr<Egg> egg(new Egg(game));
//        egg->id = game->newId(EGG);
//        egg->opacity = 1.;
//        egg->position.x = -4.5 + i % 10;
//        egg->position.y = -4.5 + i / 10;
//        egg->genderIsMale = i & 0b1;
//        egg->fertilized = true;
//        game->insertEntity(egg);
//    }

    {
        std::shared_ptr<EggCarton> eggcarton(new EggCarton(game));
        eggcarton->id = game->newId(EGG_CARTON);
        eggcarton->opacity = 1.;
        eggcarton->position.x = 1.;
        eggcarton->position.y = 0.;
        auto cptr = game->insertEntity(eggcarton);

        for (int i = 0; i < 10; i++) {
            std::shared_ptr<Egg> egg(new Egg(game));
            egg->id = game->newId(EGG);
            egg->opacity = 1.;
            egg->genderIsMale = i & 0b1;
            egg->fertilized = true;
            auto eptr = game->insertEntity(egg);

            {
                Action a(eptr, Timer::getNow(), ENTITY_OWN_BY); // need the container to register "own_by" or else it will not be set up correctly
                a.argEntity[0] = cptr;
                a.argInt[0] = i;
                game->pushAction(a);
            }
        }
    }
    std::shared_ptr<Truck> truck(new Truck(game));
    truck->id = game->newId(TRUCK);
    truck->opacity = 1.;
    truck->collisionPushable = false;
    truck->underlyingPos.x = 6.5;
    truck->underlyingPos.y = 0;
    auto tptr = game->insertEntity(truck);
    game->setTruck(truck);

    {
        std::shared_ptr<EggCarton> eggcarton(new EggCarton(game));
        eggcarton->id = game->newId(EGG_CARTON);
        eggcarton->opacity = 1.;
        eggcarton->underlyingPos.x = 1.;
        eggcarton->underlyingPos.y = 1.;
        auto cptr = game->insertEntity(eggcarton);

        for (int i = 0; i < 10; i++) {
            std::shared_ptr<Egg> egg(new Egg(game));
            egg->id = game->newId(EGG);
            egg->opacity = 1.;
            egg->genderIsMale = i & 0b1;
            egg->fertilized = true;
            auto eptr = game->insertEntity(egg);

            {
                Action a(eptr, Timer::getNow() + 1.5 + .2 * i, ENTITY_OWN_BY);
                a.argEntity[0] = cptr;
                a.argInt[0] = i;
                game->pushAction(a);
            }
        }
    }
}

void GameController::saveToSaveFile() {
    startOfDayTp.saveTimer();
    game->save();
}

void GameController::startDaySequence() {


}

void GameController::endDaySequence() {
    double goodsAvailableForSale = 0.;
    debug << game->truck->id << ", " << game->truck->inventory.size() << "\n";
    for (int slot = Truck::InventorySlots::TRUCK_A0; slot <= Truck::InventorySlots::TRUCK_D3; slot++) {
        std::shared_ptr<Entity> e = game->truck->inventory[slot];
        if (e == nullptr)
            continue;
        switch (e->type) {
            case DUCK: {
                std::shared_ptr<Duck> duck = std::dynamic_pointer_cast<Duck>(e);
                if (!duck)
                    break;
                switch (duck->growStage) {
                    case Duck::DUCKLING:
                        goodsAvailableForSale += 50. + 10. * getRand();
                        break;
                    case Duck::CHILD:
                        goodsAvailableForSale += 90. + 30. * getRand();
                        break;
                    case Duck::GROWN:
                        goodsAvailableForSale += 800. + 200. * getRand();
                        break;
                }
                Action a(Timer::getNow(), GLOBAL_DESTROY);
                a.argEntity[0] = e;
                game->pushAction(a);
                break;
            }
            case EGG_CARTON:
                for (int eslot = EggCarton::InventorySlots::EGG_0; eslot <= EggCarton::InventorySlots::EGG_9; eslot++) {
                    if (e->inventory[eslot] != nullptr && e->inventory[eslot]->type == EGG) {
                        std::shared_ptr<Egg> egg = std::dynamic_pointer_cast<Egg>(e->inventory[eslot]);
                        if (!egg)
                            break;
                        if (egg->fertilized)
                            goodsAvailableForSale += 0.; // can you sell fertilized eggs?
                        else
                            goodsAvailableForSale += 10. + 2. * getRand(); // can you sell fertilized eggs?

                        Action a(Timer::getNow(), GLOBAL_DESTROY);
                        a.argEntity[0] = e->inventory[eslot];
                        game->pushAction(a);
                    }
                }
                break;
        }
    }
    debug << "$" << goodsAvailableForSale << "\n";
    cash += goodsAvailableForSale;
}



