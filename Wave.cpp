#include "Wave.h"

Wave::Wave(int enemies, int on_screen, int wn) : total_enemies(enemies),
	max_enemies_on_screen(on_screen), wave_number(wn) {}


Wave::~Wave()
{
}

int Wave::get_wave_number()
{
	return wave_number;
}

int Wave::get_total_enemies()
{
	return total_enemies;
}

int Wave::get_max_enemies_on_screen()
{
	return max_enemies_on_screen;
}
