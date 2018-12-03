#pragma once

#include "Player.h"
#include "Grunt.h"
#include "PlayerAttackHitbox.h"
#include "ArenaState.h"
#include "Game.h"
#include "Entity.h"
#include "Downgrade.h"

class Game;
class ArenaState;

enum
{
	PLAYER,
	GRUNT,
	PLAYER_ATTACK_HITBOX
};

class EntityManager
{
public:
	EntityManager(Game *game, ArenaState *arena_state, Downgrade* dg);
	~EntityManager();

	void update_entities();
	void create_entity(int entity_type);
	Entity* find_entity_by_id(int entity_id);
	std::vector<Entity*> find_entities_by_type(int entity_type);
	void delete_entity(int entity_id);
	void handle_event(sf::Event event);
	std::vector<Entity*> get_all_entities();
	
	int global_id = 0;

protected:
	Game *game = nullptr;
	ArenaState *arena_state = nullptr;

private:
	std::vector<Entity*> entities;
	Downgrade* downgrade;
};

