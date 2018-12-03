#pragma once

#include "Entity.h"
#include "PlayerAttackHitbox.h"

class Player : public Entity
{
public:
	Player(int id, Downgrade* dg);
	~Player();

	virtual void tick() override;
	virtual void handle_event(sf::Event event) override;
	virtual void attack() override;
	virtual void update_direction() override;
	virtual void hit(int damage, int knockback_direction) override;
};

