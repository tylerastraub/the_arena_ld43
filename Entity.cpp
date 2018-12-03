#include "Entity.h"
#include "PlayerAttackHitbox.h"

#include <iostream>

Entity::Entity() {}


Entity::~Entity() {}

void Entity::set_entity_type(int et)
{
	entity_type = et;
}

void Entity::set_entity_id(int id)
{
	entity_id = id;
}

void Entity::set_entity_downgrade(Downgrade* dg)
{
	downgrade = dg;
}

void Entity::set_game(Game * game)
{
	this->game = game;
}

void Entity::set_arena_state(ArenaState * as)
{
	arena_state = as;
}

void Entity::set_player_pos(sf::Vector2f pp)
{
	player_pos = pp;
}

void Entity::set_attack_hitbox(Entity * hitbox)
{
	player_attack_hitbox = hitbox;
	player_attack_hitbox->set_default_size(sf::Vector2f(40, 40));
	update_direction(); // called here to set attack hitbox in right location
}

void Entity::set_collision_box(sf::RectangleShape collision_box)
{
	this->collision_box = collision_box;
}

void Entity::set_size(sf::Vector2f size)
{
	this->size = size;
}

void Entity::set_pos(sf::Vector2f pos)
{
	this->pos = pos;
}

void Entity::set_default_size(sf::Vector2f default_size)
{
	this->default_size = default_size;
}

sf::Vector2f Entity::get_default_size()
{
	return default_size;
}

void Entity::set_attacking(bool a)
{
	attacking = a;
}

void Entity::set_hp(int hp)
{
	this->hp = hp;
}

void Entity::move()
{
	if (!is_hit)
	{
		if (left)
		{
			if (dx > -1 * move_speed)
			{
				dx -= acceleration;
			}
			else
			{
				dx = -1 * move_speed;
			}
		}

		if (right)
		{
			if (dx < move_speed)
			{
				dx += acceleration;
			}
			else
			{
				dx = move_speed;
			}
		}

		if (up)
		{
			if (dy > -1 * move_speed)
			{
				dy -= acceleration;
			}
			else
			{
				dy = -1 * move_speed;
			}
		}

		if (down)
		{
			if (dy < move_speed)
			{
				dy += acceleration;
			}
			else
			{
				dy = move_speed;
			}
		}

		// move 1 pixel at a time, check for collisions at each step
		for (int i = 0; i < abs(dx); ++i)
		{
			if (dx < 0)
			{
				--pos.x;
			}
			else
			{
				++pos.x;
			}
			check_map_collision();
			last_pos = pos;
		}

		for (int i = 0; i < abs(dy); ++i)
		{
			if (dy < 0)
			{
				--pos.y;
			}
			else
			{
				++pos.y;
			}

			check_map_collision();
			last_pos = pos;
		}

		// add friction
		if ((!left && !right) || (left && right))
		{
			if (abs(dx) - acceleration < 0)
			{
				dx = 0;
			}
			else if (dx < 0)
			{
				dx += acceleration;
			}
			else if (dx > 0)
			{
				dx -= acceleration;
			}
		}

		if ((!up && !down) || (up && down))
		{
			if (abs(dy) - acceleration < 0)
			{
				dy = 0;
			}
			else if (dy < 0)
			{
				dy += acceleration;
			}
			else if (dy > 0)
			{
				dy -= acceleration;
			}
		}
	}
	else
	{
		// we are hit
		for (int i = 0; i < abs(dx_hit); ++i)
		{
			if (dx_hit < 0)
			{
				--pos.x;
			}
			else
			{
				++pos.x;
			}
			check_map_collision();
			last_pos = pos;
		}

		for (int i = 0; i < abs(dy_hit); ++i)
		{
			if (dy_hit < 0)
			{
				--pos.y;
			}
			else
			{
				++pos.y;
			}
		}

		if (abs(dx_hit) - knockback_distance * knockback_modifier / hit_time < 0)
		{
			dx_hit = 0;
		}
		else if (dx_hit < 0)
		{
			dx_hit += knockback_distance * knockback_modifier / hit_time;
		}
		else
		{
			dx_hit -= knockback_distance * knockback_modifier / hit_time;
		}
		
		if (abs(dy_hit) - knockback_distance * knockback_modifier / hit_time < 0)
		{
			dy_hit = 0;
		}
		else if (dy_hit < 0)
		{
			dy_hit += knockback_distance * knockback_modifier / hit_time;
		}
		else
		{
			dy_hit -= knockback_distance * knockback_modifier / hit_time;
		}

		check_map_collision();
		last_pos = pos;

		if (abs(dx_hit) <= 0 && abs(dy_hit) <= 0)
		{
			is_hit = false;
		}
	}

	collision_box.setPosition(pos);
}

void Entity::check_map_collision()
{
	// use else if statement because player can't collide with left and right sides at same time
	if (pos.x < arena_state->get_arena_position().x)
	{
		pos.x = last_pos.x;
	}
	else if (pos.x + size.x > arena_state->get_arena_position().x + arena_state->get_arena_size().x)
	{
		pos.x = last_pos.x;
	}

	// use else if statement because player can't collide with top and bottom sides at same time
	if (pos.y < arena_state->get_arena_position().y - size.y)
	{
		pos.y = last_pos.y;
	}
	else if (pos.y + size.y > arena_state->get_arena_position().y + arena_state->get_arena_size().y)
	{
		pos.y = last_pos.y;
	}
}

bool Entity::check_entity_collision(Entity* e)
{
	if ((e->get_pos().x < pos.x + size.x &&
		e->get_pos().x + e->get_size().x > pos.x &&
		e->get_pos().y < pos.y + size.y &&
		e->get_pos().y + e->get_size().y > pos.y))
	{
		return true;
	}

	return false;
}

bool Entity::is_attacking()
{
	return attacking;
}

int Entity::get_entity_id()
{
	return entity_id;
}

int Entity::get_entity_type()
{
	return entity_type;
}

sf::Vector2f Entity::get_pos()
{
	return pos;
}

sf::Vector2f Entity::get_size()
{
	return size;
}

sf::RectangleShape Entity::get_collision_rect()
{
	return collision_box;
}

Entity * Entity::get_attack_hitbox()
{
	return player_attack_hitbox;
}

int Entity::get_direction()
{
	return direction;
}

int Entity::get_attack_damage()
{
	return attack_damage;
}

int Entity::get_hp()
{
	return hp;
}

int Entity::get_invincibility()
{
	return invincibility_frames;
}
