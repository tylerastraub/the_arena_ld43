#pragma once

#include <vector>

class Downgrade
{
public:
	Downgrade();
	~Downgrade();

	void update_downgrade(Downgrade* dg);

	float movement_mod = 1.0;
	int attack_mod = 1.0;
	float enemy_size_mod = 1.0;
	float enemy_speed_mod = 1.0;
	int enemy_damage_mod = 1.0;
	float friction_mod = 1.0;
	int enemy_health_mod = 1.0;
	std::vector<std::string> downgrade_descriptions;
};

