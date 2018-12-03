#include "Game.h"

#include "State.h"
#include "Menu.h"
#include "ArenaState.h"
#include "EntityManager.h"

#include <iostream>

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(1024, 768), "The Arena", sf::Style::Close);
	window->setFramerateLimit(60);
	window->setVerticalSyncEnabled(true);

	std::vector<Downgrade*> downgrade_list;
	std::vector<Wave*> wave_list;

	// i < number of total waves
	for (int i = 0; i < 8; ++i)
	{
		downgrade_list.push_back(new Downgrade());
		wave_list.push_back(new Wave((i + 1) * 3, ceil((i + 1) * 1.5), i + 1));
	}

	// downgrades
	downgrade_list[0]->downgrade_descriptions.push_back("none");
	downgrade_list[1]->enemy_health_mod = 2;
	downgrade_list[1]->downgrade_descriptions.push_back(" enemy hp+");
	downgrade_list[2]->enemy_speed_mod = 1.5;
	downgrade_list[2]->downgrade_descriptions.push_back(" faster enemies");
	downgrade_list[3]->enemy_damage_mod = 2;
	downgrade_list[3]->downgrade_descriptions.push_back(" stronger enemies");
	downgrade_list[4]->movement_mod = 0.7;
	downgrade_list[4]->downgrade_descriptions.push_back(" player is slowed");
	downgrade_list[5]->attack_mod = 0.5;
	downgrade_list[5]->downgrade_descriptions.push_back(" player is weaker");
	downgrade_list[6]->enemy_size_mod = 2.0;
	downgrade_list[6]->downgrade_descriptions.push_back(" bigger enemies");
	downgrade_list[7]->friction_mod = 0.05;
	downgrade_list[7]->downgrade_descriptions.push_back(" ground is icy");

	sf::Clock clock;

	float current_time;
	int fps;
	float last_time = 0;
	float fps_display_delay = 0.5; // interval between each fps display

	change_state(new ArenaState(wave_list, downgrade_list, 1, 100));

	while (window->isOpen())
	{
		fps = 0;

		// state handling/loading
		if (next_state)
		{
			if (current_state)
			{
				delete current_state;
			}

			current_state = next_state;
			next_state = nullptr;
			current_state->set_game(this);
			current_state->init();
		}

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				exit();
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				change_state(new ArenaState(wave_list, downgrade_list, 1, 100));
			}

			if (current_state)
			{
				current_state->handle_event(event);
			}
		}

		// display FPS
		sf::Time time_elapsed = clock.restart(); // clock.restart() returns time passed as milliseconds
		current_time = time_elapsed.asSeconds();
		fps = 1.f / current_time - last_time;
		fps_display_delay -= current_time;
		last_time = current_time;
		if (fps_display_delay <= 0)
		{
			std::cout << fps << " FPS" << std::endl;
			fps_display_delay = 0.5;
		}

		// call tick method
		if (current_state)
		{
			current_state->tick(time_elapsed);
		}

		window->clear(sf::Color(40, 40, 40)); // this will be background color

		// call render method
		if (current_state)
		{
			current_state->render(*window);
		}

		window->display();

		if (exit_flag)
		{
			window->close();
		}
	}
}

Game::~Game()
{
	delete window;
	if (current_state)
	{
		delete current_state;
	}
	if (next_state)
	{
		delete next_state;
	}
}

void Game::change_state(State *state)
{
	if (next_state)
	{
		delete next_state;
	}

	next_state = state;
}

void Game::exit()
{
	exit_flag = true;
}

sf::Vector2f Game::get_window_size_as_float()
{
	return sf::Vector2f(window->getSize());
}
