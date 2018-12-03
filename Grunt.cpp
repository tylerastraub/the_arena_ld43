#include "Grunt.h"

#include <iostream>

Grunt::Grunt(int id, float x, float y, Downgrade* dg)
{
	size = sf::Vector2f(25 * dg->enemy_size_mod, 25 * dg->enemy_size_mod);
	pos = sf::Vector2f(x, y);
	collision_box.setFillColor(sf::Color(255, 0, 0));
	collision_box.setPosition(pos);
	collision_box.setSize(size);
	set_entity_id(id);

	// movement settings
	move_speed = 0.8 * dg->enemy_speed_mod;
	acceleration = move_speed / 4.f * dg->friction_mod;
	knockback_distance = 30.0;

	hp = 20 * dg->enemy_health_mod;
	attack_damage = 10 * dg->enemy_damage_mod;
}

Grunt::~Grunt() {}

void Grunt::tick()
{
	// TODO: make less jittery movement
	// move towards player
	if (!is_hit)
	{
		if (pos.x < player_pos.x)
		{
			right = true;
		}
		else
		{
			right = false;
		}

		if (pos.x > player_pos.x)
		{
			left = true;
		}
		else
		{
			left = false;
		}

		if (pos.y < player_pos.y + 12)
		{
			down = true;
		}
		else
		{
			down = false;
		}

		if (pos.y > player_pos.y + 12)
		{
			up = true;
		}
		else
		{
			up = false;
		}
	}
	
	update_direction();
	move();
}

void Grunt::handle_event(sf::Event event)
{

}

void Grunt::update_direction() 
{
	if (abs(pos.y - player_pos.y + 12) > abs(pos.x - player_pos.x))
	{
		if (up)
		{
			direction = NORTH;
		}
		else
		{
			direction = SOUTH;
		}
	}
	else
	{
		if (left)
		{
			direction = WEST;
		}
		else
		{
			direction = EAST;
		}
	}
}

void Grunt::attack()
{
}

void Grunt::hit(int damage, int knockback_direction)
{
	is_hit = true;
	hp -= damage;

	switch (knockback_direction)
	{
	case NORTH:
		dy_hit = knockback_distance * knockback_modifier * -1;
		break;
	case EAST:
		dx_hit = knockback_distance * knockback_modifier;
		break;
	case SOUTH:
		dy_hit = knockback_distance * knockback_modifier;
		break;
	case WEST:
		dx_hit = knockback_distance * knockback_modifier * -1;
		break;
	default:
		break;
	}
}
