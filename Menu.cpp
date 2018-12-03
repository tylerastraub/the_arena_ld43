/*
THIS MENU CLASS IS NOT USED BUT IS KEPT IN CASE IT WILL BE IN THE FUTURE
*/

#include "Menu.h"

#include <iostream>

Menu::Menu()
{

}

Menu::~Menu()
{

}

void Menu::init()
{
	if (!menu_font.loadFromFile("fonts/manaspc.ttf"))
	{
		std::cout << "ERROR: could not load font manaspc.ttf" << std::endl;
	}
	menu_font.loadFromFile("fonts/manaspc.ttf");
}

void Menu::handle_event(sf::Event event)
{

}

void Menu::tick(sf::Time elapsed)
{
	
}

void Menu::render(sf::RenderWindow& window)
{
	window.clear(sf::Color(44, 232, 245));
	sf::Text title;
	title.setFont(menu_font);
	title.setString("TITLE (this is the main menu state)");
	title.setFillColor(sf::Color(255, 255, 255));
	title.setCharacterSize(40);
	// center text horizontally
	sf::FloatRect title_rect = title.getLocalBounds();
	title.setOrigin(title_rect.left + title_rect.width / 2.0f,
		title_rect.top + title_rect.height / 2.0f);
	title.setPosition(sf::Vector2f(window.getSize().x / 2.0f, 100));

	window.draw(title);
}
