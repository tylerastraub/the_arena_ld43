#include "Downgrade.h"

Downgrade::Downgrade()
{
}

Downgrade::~Downgrade() {}

void Downgrade::update_downgrade(Downgrade * dg)
{
	movement_mod *= dg->movement_mod;
	attack_mod *= dg->attack_mod;
	enemy_size_mod *= dg->enemy_size_mod;
	enemy_speed_mod *= dg->enemy_speed_mod;
	enemy_damage_mod *= dg->enemy_damage_mod;
	friction_mod *= dg->friction_mod;
	enemy_health_mod *= dg->enemy_health_mod;
	downgrade_descriptions.push_back(dg->downgrade_descriptions[0]);
}
