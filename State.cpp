#include "State.h"

State::State()
{
}

State::~State()
{
}

void State::set_game(Game * game)
{
	this->game = game;
}

Game *State::get_game()
{
	return game;
}
