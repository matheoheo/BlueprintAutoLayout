#pragma once
#include <SFML/Graphics.hpp>
#include "GraphRenderer.h"
#include "GraphViewHandler.h"
#include "BlueprintGraph.h"

class Application
{
public:
	Application(const sf::ContextSettings& settings);

	void start(int argc, char* argv[]);
private:
	void processEvents();
	void update(const sf::Time& deltaTime);
	void render();

	void saveResult(const BlueprintGraph& graphData, const GraphLayout& layout);
private:
	sf::RenderWindow mWindow;
	std::unique_ptr<GraphRenderer> mGraphRenderer; //lazy initalization
	GraphViewHandler mViewHandler;
};