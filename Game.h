#pragma once

#ifndef LD43_GAME_H
#define LD43_GAME_H

#include <SFML/Graphics.hpp>

class State;
class EntityManager;

class Game {
public:
	Game();
	~Game();

	void change_state(State *state);
	void exit();

	sf::Vector2f get_window_size_as_float();

private:
	sf::RenderWindow *window;
	bool exit_flag = false;
	State *current_state;
	State *next_state;
};


#endif //LD43_GAME_H