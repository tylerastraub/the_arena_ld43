#pragma once

#include "Entity.h"

class PlayerAttackHitbox : public Entity
{
public:
	PlayerAttackHitbox(int id);
	~PlayerAttackHitbox();

	virtual void tick() override;
	virtual void handle_event(sf::Event event) override;
	virtual void update_direction() override;
	virtual void attack() override;
	virtual void hit(int damage, int knockback_direction) override;
};

