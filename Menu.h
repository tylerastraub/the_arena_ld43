#pragma once

#include "State.h"

class Menu : public State
{
public:
	Menu();
	~Menu();

	virtual void init() override;
	virtual void handle_event(sf::Event event) override;
	virtual void tick(sf::Time elapsed) override;
	virtual void render(sf::RenderWindow& window) override;
private:
	sf::Font menu_font;
};

