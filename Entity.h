#pragma once

#include "ArenaState.h"
#include "Downgrade.h"

class ArenaState;
class PlayerAttackHitbox;

enum
{
	NORTH,
	EAST,
	SOUTH,
	WEST
};

class Entity
{
public:
	Entity();
	~Entity();

	void set_entity_type(int et);
	void set_entity_id(int id);
	void set_entity_downgrade(Downgrade* dg);
	void set_game(Game *game);
	void set_arena_state(ArenaState *as);
	void set_player_pos(sf::Vector2f pp);
	void set_attack_hitbox(Entity *hitbox);
	void set_collision_box(sf::RectangleShape collision_box);
	void set_size(sf::Vector2f size);
	void set_pos(sf::Vector2f pos);
	void set_default_size(sf::Vector2f default_size);
	sf::Vector2f get_default_size();
	void set_attacking(bool a);
	void set_hp(int hp);

	virtual void tick() = 0;
	virtual void handle_event(sf::Event event) = 0;
	virtual void update_direction() = 0;
	virtual void attack() = 0; // method for when entity attacks
	virtual void hit(int damage, int knockback_direction) = 0; // method to handle what to do when hit
	void move();
	void check_map_collision();
	bool check_entity_collision(Entity* e);
	bool is_attacking();

	int get_entity_id();
	int get_entity_type();
	sf::Vector2f get_pos();
	sf::Vector2f get_size();
	sf::RectangleShape get_collision_rect();
	Entity* get_attack_hitbox();
	int get_direction();
	int get_attack_damage();
	int get_hp();
	int get_invincibility();

protected:
	// movement
	sf::Vector2f pos, last_pos, size, player_pos, player_size; // last_pos used for collision
	float dx = 0; // delta x
	float dy = 0; // delta y
	float dx_hit = 0;
	float dy_hit = 0; // used for when entity is hit
	float move_speed, acceleration, knockback_distance; // move_speed is max speed
	bool left, right, up, down;
	int direction;

	// game stuff
	Game *game = nullptr;
	ArenaState *arena_state = nullptr;
	Downgrade* downgrade;

	// collision
	sf::RectangleShape collision_box;
	Entity *player_attack_hitbox; // only player uses this

	// only used by player_attack_hitbox
	sf::Vector2f default_size;

	// attributes
	int hp = 1; // should be overriden by each entity
	// TODO: add knockback when hurt (both player and enemy)
	bool is_hit = false;
	int invincibility_frames = 0; // number of frames entity is invincible after hurt
	float knockback_modifier = 1.0;
	float hit_time = 5.0; // determines how long someone is stunned when hit. lower val = less time stunned
	bool attacked = false; // used to space out attacks
	bool attacking = false; // used to tell when hitbox is out
	int attack_damage;

private:
	int entity_type;
	int entity_id;
};

