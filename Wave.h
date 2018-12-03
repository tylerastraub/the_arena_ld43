#pragma once
class Wave
{
public:
	Wave(int enemies, int on_screen, int wn);
	~Wave();

	int get_wave_number();
	int get_total_enemies();
	int get_max_enemies_on_screen();

private:
	int total_enemies, max_enemies_on_screen, wave_number;
};

