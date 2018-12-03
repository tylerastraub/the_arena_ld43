#include "PlayerAttackHitbox.h"

PlayerAttackHitbox::PlayerAttackHitbox(int id)
{
	set_entity_id(id);
	collision_box.setPosition(pos);
	collision_box.setSize(size);
}

PlayerAttackHitbox::~PlayerAttackHitbox() {}

void PlayerAttackHitbox::tick()
{
	/* // used for debugging purposes
	collision_box.setFillColor(sf::Color(255, 255, 255));
	collision_box.setPosition(pos);
	collision_box.setSize(size);
	*/
	
}

void PlayerAttackHitbox::handle_event(sf::Event event) {}

void PlayerAttackHitbox::update_direction() {}

void PlayerAttackHitbox::attack() {}

void PlayerAttackHitbox::hit(int damage, int knockback_direction) {}


