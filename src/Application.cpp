#include "Application.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

Application::Application(const sf::ContextSettings& settings)
	:mWindow(sf::VideoMode(sf::Vector2u(1366, 766)), "Blueprint Auto Layout", sf::State::Windowed, settings),
	mViewHandler(mWindow.getSize())
{
}

void Application::start(int argc, char* argv[])
{
	std::string pathToFile;
	if (argc > 1)
	{
		pathToFile = argv[1];
	}
	BlueprintGraph graphData;
	if (!graphData.loadFromJson(pathToFile))
	{
		std::cout << "Failed to load data from: " << pathToFile << '\n';
		return;
	}
	GraphLayout layout;
	if (!layout.create(graphData.getBlueprintData()))
	{
		std::cout << "Layout has cycles, cannot create graph.\n";
		return;
	}
	saveResult(graphData, layout);
	mGraphRenderer = std::make_unique<GraphRenderer>(layout.getGraphNodes(), mWindow);
	sf::Clock fpsClock;

	while (mWindow.isOpen())
	{
		sf::Time time = fpsClock.restart();
		processEvents();
		update(time);
		render();
	}
}

void Application::processEvents()
{
	while (const auto event = mWindow.pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			mWindow.close();
		mViewHandler.processEvents(*event, mWindow);
	}
}

void Application::update(const sf::Time& deltaTime)
{
	mViewHandler.update(deltaTime);
}

void Application::render()
{
	mWindow.setView(mViewHandler.getView());
	mWindow.clear();
	mGraphRenderer->render();
	mWindow.display();
}

void Application::saveResult(const BlueprintGraph& graphData, const GraphLayout& layout)
{
	nlohmann::json j;
	j["nodes"] = nlohmann::json::array();

	for (const auto& node : layout.getGraphNodes())
	{
		if (node.isDummy)
			continue;

		nlohmann::json nodeObj;
		nodeObj["id"] = node.data->id;
		nodeObj["name"] = node.data->name;
		nodeObj["x"] = static_cast<int>(node.posX);
		nodeObj["y"] = static_cast<int>(node.posY);

		j["nodes"].push_back(nodeObj);
	}

	j["edges"] = nlohmann::json::array();
	for (const auto& edge : graphData.getBlueprintData().edges)
	{
		j["edges"].push_back({
			{"from", edge.from},
			{"to", edge.to}
		});
	}

	std::filesystem::path pathToResult("resultLayout.json");
	std::ofstream file(pathToResult);
	if (!file)
	{
		std::cout << "Cannot save result to file\n";
		return;
	}

	file << j.dump(2);
}

