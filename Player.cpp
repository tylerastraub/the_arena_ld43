#include "Player.h"

#include <iostream>

Player::Player(int id, Downgrade* dg)
{
	pos = sf::Vector2f(400, 400);
	size = sf::Vector2f(25, 50);
	player_size = size;
	last_pos = pos;
	collision_box.setPosition(pos);
	collision_box.setSize(size);
	collision_box.setFillColor(sf::Color(255, 0, 255));
	set_entity_id(id);

	move_speed = 6.0 * dg->movement_mod;
	acceleration = 2.5 * dg->friction_mod;
	attack_damage = 8 * dg->attack_mod;
	knockback_distance = 15.0;
	knockback_modifier = 1.0;
	invincibility_frames = 60;

	direction = EAST;

	hp = 100;
}

Player::~Player()
{
}

void Player::tick()
{
	move();
	update_direction();
	if (invincibility_frames > 0)
	{
		collision_box.setFillColor(sf::Color(255, 255, 255));
		--invincibility_frames;
	}
	else
	{
		collision_box.setFillColor(sf::Color(255, 0, 255));
	}
		
}

void Player::handle_event(sf::Event event)
{
	// handle key input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if(!left)
		{
			direction = WEST;
		}
		left = true;
	}
	else
	{
		left = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (!right)
		{
			direction = EAST;
		}
		right = true;
	}
	else
	{
		right = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!up)
		{
			direction = NORTH;
		}
		up = true;
	}
	else
	{
		up = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (!down)
		{
			direction = SOUTH;
		}
		down = true;
	}
	else
	{
		down = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		attack();
		attacked = true;
	}
	else
	{
		attacked = false;
	}
}

void Player::update_direction()
{
	switch (direction)
	{
		case NORTH:
			player_attack_hitbox->set_size(sf::Vector2f(player_attack_hitbox->get_default_size().y,
				player_attack_hitbox->get_default_size().x));
			player_attack_hitbox->set_pos(sf::Vector2f(pos.x + size.x / 2.0 
				- player_attack_hitbox->get_size().x / 2.0, pos.y - player_attack_hitbox->get_size().y));
			break;
		case EAST:
			player_attack_hitbox->set_size(player_attack_hitbox->get_default_size());
			player_attack_hitbox->set_pos(sf::Vector2f(pos.x + size.x, pos.y + size.y / 2.0 
				- player_attack_hitbox->get_size().y / 2.0));
			break;
		case SOUTH:
			player_attack_hitbox->set_size(sf::Vector2f(player_attack_hitbox->get_default_size().y,
				player_attack_hitbox->get_default_size().x));
			player_attack_hitbox->set_pos(sf::Vector2f(pos.x + size.x / 2.0 
				- player_attack_hitbox->get_size().x / 2.0, pos.y + size.y));
			break;
		case WEST:
			player_attack_hitbox->set_size(player_attack_hitbox->get_default_size());
			player_attack_hitbox->set_pos(sf::Vector2f(pos.x - player_attack_hitbox->get_size().x,
				pos.y + size.y / 2.0 - player_attack_hitbox->get_size().y / 2.0));
			break;
		default:
			break;
	}
}

void Player::hit(int damage, int knockback_direction)
{
	invincibility_frames = 60;
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

void Player::attack()
{
	if (!attacked)
	{
		attacking = true;
	}
}


