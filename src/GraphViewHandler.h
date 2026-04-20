#pragma once
#include <SFML/Graphics.hpp>

class GraphViewHandler
{
public:
	GraphViewHandler(const sf::Vector2u& size);
	
	void processEvents(const sf::Event& event, const sf::RenderWindow& window);
	void update(const sf::Time& deltaTime);
	const sf::View& getView() const;
private:
	sf::View mGraphView;
};