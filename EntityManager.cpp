#include "EntityManager.h"

#include <iostream>
#include <stdlib.h>

EntityManager::EntityManager(Game *game, ArenaState *arena_state, Downgrade* dg)
{
	this->arena_state = arena_state;
	this->game = game;
	downgrade = dg;
}

EntityManager::~EntityManager() {}

void EntityManager::update_entities()
{
	sf::Vector2f player_pos = find_entities_by_type(PLAYER)[0]->get_pos();
	Entity* player = find_entities_by_type(PLAYER)[0];

	for (int i = 0; i < entities.size(); ++i)
	{
		// let all entities know where the player is
		entities[i]->set_player_pos(player_pos);
		entities[i]->tick();
		
		// check if player is attacking
		if (player->is_attacking())
		{
			arena_state->draw_attack_sprite();
			// check if player hits any entity
			bool hit = find_entities_by_type(PLAYER_ATTACK_HITBOX)[0]->check_entity_collision(entities[i]);
			if (hit)
			{
				entities[i]->hit(player->get_attack_damage(), player->get_direction());
			}
		}

		// check if enemy is attacking player
		if (entities[i]->get_entity_type() == GRUNT)
		{
			if (entities[i]->check_entity_collision(player))
			{
				if(player->get_invincibility() <= 0)
					player->hit(entities[i]->get_attack_damage(), entities[i]->get_direction());
			}
		}

		// check if anyone is dead
		if (entities[i]->get_hp() <= 0)
		{
			if (entities[i]->get_entity_type() == PLAYER)
			{
				arena_state->end_game();
			}
			else
			{
				if (entities[i]->get_entity_type() == GRUNT)
				{
					arena_state->enemy_killed();
				}

				delete_entity(entities[i]->get_entity_id());
			}

		}
	}

	player->set_attacking(false);
}

void EntityManager::create_entity(int entity_type)
{
	switch (entity_type)
	{
	case PLAYER:
		entities.push_back(new Player(global_id++, downgrade));
		find_entity_by_id(global_id - 1)->set_entity_type(PLAYER);
		find_entity_by_id(global_id - 1)->set_game(game);
		find_entity_by_id(global_id - 1)->set_arena_state(arena_state);
		find_entity_by_id(global_id - 1)->set_entity_downgrade(downgrade);

		entities.push_back(new PlayerAttackHitbox(global_id++));
		find_entity_by_id(global_id - 1)->set_entity_type(PLAYER_ATTACK_HITBOX);
		find_entity_by_id(global_id - 2)->set_attack_hitbox(entities[global_id - 1]);
		find_entity_by_id(global_id - 1)->set_entity_downgrade(downgrade);
		break;
	case GRUNT:
		// randomly spawn grunt not on player
		int spawn_x, spawn_y;
		do
		{
			spawn_x = (rand() % (int)(arena_state->get_arena_size().x - 51))
				+ arena_state->get_arena_position().x;
			spawn_y = (rand() % (int)(arena_state->get_arena_size().y - 51))
				+ arena_state->get_arena_position().y - 25 * downgrade->enemy_size_mod;
		} while (find_entities_by_type(PLAYER)[0]->get_pos().x
			+ find_entities_by_type(PLAYER)[0]->get_size().x < spawn_x
			&& find_entities_by_type(PLAYER)[0]->get_pos().x > spawn_x + 60
			&& find_entities_by_type(PLAYER)[0]->get_pos().y
			+ find_entities_by_type(PLAYER)[0]->get_size().y < spawn_y
			&& find_entities_by_type(PLAYER)[0]->get_pos().y > spawn_x + 60);

		entities.push_back(new Grunt(global_id++, spawn_x, spawn_y, downgrade));
		find_entity_by_id(global_id - 1)->set_entity_type(GRUNT);
		find_entity_by_id(global_id - 1)->set_game(game);
		find_entity_by_id(global_id - 1)->set_arena_state(arena_state);
		find_entity_by_id(global_id - 1)->set_entity_downgrade(downgrade);
		break;
	default:
		std::cout << "Invalid entity type" << std::endl;
		break;
	}
}

Entity * EntityManager::find_entity_by_id(int entity_id)
{
	for (auto it : entities)
	{
		if (it->get_entity_id() == entity_id)
		{
			return it;
		}
	}
	return nullptr;
}

std::vector<Entity*> EntityManager::find_entities_by_type(int entity_type)
{
	std::vector<Entity*> ebt;
	for (auto it : entities)
	{
		if (it->get_entity_type() == entity_type)
		{
			ebt.push_back(it);
		}
	}

	return ebt;
}

void EntityManager::delete_entity(int entity_id)
{
	Entity* e = find_entity_by_id(entity_id);
	entities.erase(std::remove(entities.begin(), entities.end(), e), entities.end());
	delete e;
}

void EntityManager::handle_event(sf::Event event)
{
	for (int i = 0; i < entities.size(); ++i)
	{
		entities[i]->handle_event(event);
	}
}

std::vector<Entity*> EntityManager::get_all_entities()
{
	return entities;
}
