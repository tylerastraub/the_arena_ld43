#include "ArenaState.h"
#include "Player.h"
#include "EntityManager.h"
#include "Downgrade.h"

#include <iostream>
#include <vector>

ArenaState::ArenaState(std::vector<Wave*> wave, std::vector<Downgrade*> dg, int w, int php) : arena_size(sf::Vector2f(500, 500)),
	arena_wall_size(5, 20), waves(wave), downgrades(dg), current_wave_num(w), player_starting_hp(php) {}

ArenaState::~ArenaState() {}

void ArenaState::init()
{
	current_wave = waves[current_wave_num - 1];
	current_downgrade = new Downgrade();
	for (int i = 0; i < current_wave_num; ++i)
	{
		current_downgrade->update_downgrade(downgrades[i]);
	}

	entity_manager = new EntityManager(game, this, current_downgrade);
	create_walls_and_arena();
	
	entity_manager->create_entity(PLAYER);
	entity_manager->find_entities_by_type(PLAYER)[0]->set_hp(player_starting_hp);

	for (int i = 0; i < current_wave->get_max_enemies_on_screen() 
		&& i < current_wave->get_total_enemies(); ++i)
	{
		++num_of_enemies_on_screen;
		entity_manager->create_entity(GRUNT);
	}

	num_of_enemies = current_wave->get_total_enemies();

	if (!main_font.loadFromFile("fonts/manaspc.ttf"))
	{
		std::cout << "ERROR: could not load font manaspc.ttf" << std::endl;
	}
	main_font.loadFromFile("fonts/manaspc.ttf");

	if (!attack_texture.loadFromFile("res/attack.png"))
	{
		std::cout << "ERROR: could not load texture attack.png" << std::endl;
	}
	attack_texture.loadFromFile("res/attack.png");
}

void ArenaState::handle_event(sf::Event event)
{
	entity_manager->handle_event(event);
}

void ArenaState::tick(sf::Time elapsed)
{
	tick_count -= elapsed.asSeconds();
	if (tick_count <= 0)
	{
		tick_count = tick_rate;
		// if we are not paused, we can update
		if (!pause)
		{
			entity_manager->update_entities();
		}
	}
}

void ArenaState::render(sf::RenderWindow& window)
{
	window.clear(sf::Color(40, 40, 40));

	// text
	sf::Text wave_text, num_enemies_text, downgrades_text, hp_text, 
		game_over_text, restart_text, win_text;
	wave_text.setFont(main_font);
	wave_text.setPosition(sf::Vector2f(10, 5));
	wave_text.setString("wave: " + std::to_string(current_wave->get_wave_number()));

	num_enemies_text.setFont(main_font);
	num_enemies_text.setPosition(sf::Vector2f(10, 39));
	num_enemies_text.setString("enemies left: " + std::to_string(num_of_enemies));

	downgrades_text.setFont(main_font);
	downgrades_text.setPosition(10, window.getSize().y - 24);
	downgrades_text.setString("downgrades: ");
	downgrades_text.setCharacterSize(15);
	for (auto it : current_downgrade->downgrade_descriptions)
	{
		downgrades_text.setString(downgrades_text.getString() + it + ",");
	}
	downgrades_text.setString(downgrades_text.getString().substring(0, downgrades_text.getString().getSize() - 1));

	hp_text.setString("hp: " + std::to_string(entity_manager->find_entities_by_type(PLAYER)[0]->get_hp()));
	hp_text.setFont(main_font);
	hp_text.setPosition(window.getSize().x - hp_text.getLocalBounds().width - 10, 5);
	hp_text.setFillColor(sf::Color(255, 0, 0));

	game_over_text.setFont(main_font);
	game_over_text.setString("game over");
	game_over_text.setCharacterSize(70);
	sf::FloatRect rect = game_over_text.getLocalBounds();
	game_over_text.setOrigin(rect.left + rect.width / 2.0f,
		rect.top + rect.height / 2.0f);
	game_over_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

	win_text.setFont(main_font);
	win_text.setString("you won");
	win_text.setCharacterSize(70);
	rect = win_text.getLocalBounds();
	win_text.setOrigin(rect.left + rect.width / 2.0f,
		rect.top + rect.height / 2.0f);
	win_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f));

	restart_text.setFont(main_font);
	restart_text.setString("press r to restart");
	rect = restart_text.getLocalBounds();
	restart_text.setOrigin(rect.left + rect.width / 2.0f,
		rect.top + rect.height / 2.0f);
	restart_text.setPosition(sf::Vector2f(window.getSize().x / 2.0f, game_over_text.getPosition().y + 75));

	// textures
	sf::Sprite attack_sprite;
	attack_sprite.setTexture(attack_texture);
	attack_sprite.setPosition(entity_manager->find_entities_by_type(PLAYER_ATTACK_HITBOX)[0]->get_pos());
	switch (entity_manager->find_entities_by_type(PLAYER)[0]->get_direction())
	{
	case NORTH:
		attack_sprite.setRotation(270.f);
		attack_sprite.setPosition(sf::Vector2f(attack_sprite.getPosition().x,
			attack_sprite.getPosition().y + 40));
		break;
	case EAST:
		attack_sprite.setRotation(0.f);
		break;
	case SOUTH:
		attack_sprite.setRotation(90.f);
		attack_sprite.setPosition(sf::Vector2f(attack_sprite.getPosition().x + 40,
			attack_sprite.getPosition().y));
		break;
	case WEST:
		attack_sprite.setRotation(180.f);
		attack_sprite.setPosition(sf::Vector2f(attack_sprite.getPosition().x + 40,
			attack_sprite.getPosition().y + 40));
		break;
	default:
		break;
	}

	// draw floor and background walls
	window.draw(arena_floor);
	window.draw(walls["Top Border"]);
	window.draw(walls["Top Wall"]);
	window.draw(walls["Left Wall"]);
	window.draw(walls["Right Wall"]);

	// draw all entities
	int id = 0;
	std::vector<Entity*> e = entity_manager->get_all_entities();
	for (auto it : e)
	{
		window.draw(it->get_collision_rect());
	}

	window.draw(entity_manager->find_entities_by_type(PLAYER)[0]->get_attack_hitbox()->get_collision_rect());

	if (display_attack_counter < 2)
	{
		window.draw(attack_sprite);
		++display_attack_counter;
	}	

	// draw foreground wall
	window.draw(walls["Bottom Border"]);
	window.draw(walls["Bottom Wall"]);

	// // draw ui
	window.draw(wave_text);
	window.draw(num_enemies_text);
	window.draw(downgrades_text);
	window.draw(hp_text);
	if (game_over)
	{
		window.draw(restart_text);
		window.draw(game_over_text);
		pause = true;
	}

	if (game_won)
	{
		window.draw(win_text);
		window.draw(restart_text);
		pause = true;
	}
}

void ArenaState::create_walls_and_arena()
{
	sf::RectangleShape left_wall, top_wall, right_wall, bottom_wall, top_border, bottom_border;

	// arena floor
	arena_floor.setSize(arena_size);
	// center arena in middle of screen
	arena_floor.setOrigin(arena_floor.getSize().x / 2.0f,
		arena_floor.getSize().y / 2.0f);
	arena_floor.setPosition(game->get_window_size_as_float().x / 2.0f, 
		game->get_window_size_as_float().y / 2.0f);
	arena_floor.setFillColor(sf::Color(184, 111, 80));
	arena_position = sf::Vector2f(arena_floor.getPosition().x
		- arena_floor.getSize().x / 2.0f, arena_floor.getPosition().y
		- arena_floor.getSize().y / 2.0f);

	// left wall
	left_wall.setSize(sf::Vector2f(arena_wall_size.x, arena_size.y));
	left_wall.setPosition(sf::Vector2f(arena_position.x - arena_wall_size.x,
		arena_position.y - arena_wall_size.y));
	left_wall.setFillColor(sf::Color(38, 43, 68));
	walls["Left Wall"] = left_wall;

	// top wall
	top_wall.setSize(sf::Vector2f(arena_size.x + arena_wall_size.x * 2, arena_wall_size.y));
	top_wall.setPosition(sf::Vector2f(arena_position.x - arena_wall_size.x,
		arena_position.y - arena_wall_size.y));
	top_wall.setFillColor(sf::Color(58, 68, 102));
	walls["Top Wall"] = top_wall;

	// top border
	top_border.setSize(sf::Vector2f(arena_size.x + arena_wall_size.x * 2, arena_wall_size.x));
	top_border.setPosition(sf::Vector2f(arena_position.x - arena_wall_size.x,
		top_wall.getPosition().y - arena_wall_size.x));
	top_border.setFillColor(sf::Color(38, 43, 68));
	walls["Top Border"] = top_border;

	// right wall
	right_wall.setSize(sf::Vector2f(arena_wall_size.x, arena_size.y));
	right_wall.setPosition(sf::Vector2f(arena_position.x + arena_size.x,
		arena_position.y - arena_wall_size.y));
	right_wall.setFillColor(sf::Color(38, 43, 68));
	walls["Right Wall"] = right_wall;

	// bottom wall
	bottom_wall.setSize(sf::Vector2f(arena_size.x + arena_wall_size.x * 2, arena_wall_size.y));
	bottom_wall.setPosition(sf::Vector2f(arena_position.x - arena_wall_size.x,
		arena_position.y + arena_size.y - arena_wall_size.y));
	bottom_wall.setFillColor(sf::Color(58, 68, 102));
	walls["Bottom Wall"] = bottom_wall;

	// bottom border
	bottom_border.setSize(sf::Vector2f(arena_size.x + arena_wall_size.x * 2, arena_wall_size.x));
	bottom_border.setPosition(sf::Vector2f(arena_position.x - arena_wall_size.x,
		bottom_wall.getPosition().y - arena_wall_size.x));
	bottom_border.setFillColor(sf::Color(38, 43, 68));
	walls["Bottom Border"] = bottom_border;
}

void ArenaState::enemy_killed()
{
	--num_of_enemies;
	--num_of_enemies_on_screen;
	
	if (num_of_enemies == 0)
	{
		wave_complete();
	}

	if (num_of_enemies_on_screen < current_wave->get_max_enemies_on_screen()
		&& num_of_enemies_on_screen < num_of_enemies)
	{
		entity_manager->create_entity(GRUNT);
		++num_of_enemies_on_screen;
	}
}

void ArenaState::end_game()
{
	game_over = true;
}

void ArenaState::wave_complete()
{
	if (current_wave_num != waves.size())
	{
		game->change_state(new ArenaState(waves, downgrades, current_wave_num + 1,
			entity_manager->find_entities_by_type(PLAYER)[0]->get_hp()));
	}
	else
	{
		game_won = true;
	}
}

void ArenaState::draw_attack_sprite()
{
	display_attack_counter = 0;
}

sf::Vector2f ArenaState::get_arena_size()
{
	return arena_size;
}

sf::Vector2f ArenaState::get_arena_position()
{
	return arena_position;
}

sf::Vector2f ArenaState::get_arena_wall_size()
{
	return arena_wall_size;
}

