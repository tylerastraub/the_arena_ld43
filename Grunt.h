#pragma once

#include "Entity.h"

class Grunt : public Entity
{
public:
	Grunt(int id, float x, float y, Downgrade* dg);
	~Grunt();

	virtual void tick() override;
	virtual void handle_event(sf::Event event) override;
	virtual void update_direction() override;
	virtual void attack() override;
	virtual void hit(int damage, int knockback_direction) override;
};

