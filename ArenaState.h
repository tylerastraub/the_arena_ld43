#pragma once

#include "State.h"
#include "Wave.h"
#include "Downgrade.h"

#include <vector>

class Player;
class EntityManager;

class ArenaState : public State
{
public:
	ArenaState(std::vector<Wave*> wave, std::vector <Downgrade*> dg, int w, int php);
	~ArenaState();

	virtual void init() override;
	virtual void handle_event(sf::Event event) override;
	virtual void tick(sf::Time elapsed) override;
	virtual void render(sf::RenderWindow& window) override;

	void create_walls_and_arena();
	void enemy_killed();
	void end_game();
	void wave_complete();
	void draw_attack_sprite();

	sf::Vector2f get_arena_size();
	sf::Vector2f get_arena_position();
	sf::Vector2f get_arena_wall_size();

private:
	std::map<std::string, sf::RectangleShape> walls;
	sf::RectangleShape arena_floor;
	sf::Vector2f arena_size, arena_position, arena_wall_size;
	bool pause = false;
	EntityManager *entity_manager;
	int display_attack_counter = 2; // number of ticks to display attack

	sf::Font main_font;
	sf::Texture attack_texture;

	// gamemode stuff
	std::vector<Downgrade*> downgrades;
	std::vector<Wave*> waves;
	Wave* current_wave;
	Downgrade* current_downgrade;
	int current_wave_num;
	int num_of_enemies; // how many enemies left per wave
	int num_of_enemies_on_screen;
	int player_starting_hp;
	bool game_over = false;
	bool is_wave_complete = false;
	bool game_won = false;
};

