#pragma once

#include "Game.h"

class State
{
public:
	State();
	virtual ~State();

	virtual void init() = 0;
	virtual void handle_event(sf::Event event) = 0;
	virtual void tick(sf::Time time_elapsed) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

	void set_game(Game *game);
	Game *get_game();

protected:
	Game *game = nullptr;
	float tick_rate = 1.0 / 60.0; // ticks per second, default is 60 times per second
	float tick_count = tick_rate;
};

