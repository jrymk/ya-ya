#pragma once
#ifndef _NPC_H_
#define _NPC_H_

#include "entity.h"
#include "game.h"

class NPC : public Entity {
public : 
	bool genderIsMale = true;
	bool fertilized = false;

	void runAction(Action& action, std::vector<Action>& followUpActions) override;

	void loadModel() override;

	Game* game;

	NPC(Game* game);

	void customUpdate() override;

	enum InventorySlots {
		LANSEHYAOJI,
	};

	void setInventoryProps() override;

	std::string getDescriptionStr() override;
};

#endif