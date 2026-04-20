#include "BlueprintGraph.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

bool BlueprintGraph::loadFromJson(const std::filesystem::path& path)
{
	std::ifstream file(path);
	if (!file)
	{
		std::cout << "Failed to open file: " << path.string() << '\n';
		return false;
	}
	nlohmann::json j;
	file >> j;

	mBlueprintData.nodes.clear();
	mBlueprintData.edges.clear();

	/*
		Reading is based on following json format:
		{
			"nodes": [
				{ "id": 1, "name": "Event BeginPlay", "x": 0, "y": 0 },
				{ "id": 2, "name": "Branch", "x": 0, "y": 0 },
				{ "id": 3, "name": "Play Sound (True)", "x": 0, "y": 0 },
				{ "id": 4, "name": "Spawn Actor (False)", "x": 0, "y": 0 }
			],
			"edges": [
				{ "from": 1, "to": 2 },
				{ "from": 2, "to": 3 },
				{ "from": 2, "to": 4 }
			  ]			
			}
	*/

	for (const auto& node : j["nodes"])
	{
		NodeData n;
		n.name = node["name"];
		n.id = node["id"].get<int>();
		n.x = node["x"].get<int>();
		n.y = node["y"].get<int>();
		mBlueprintData.nodes.push_back(n);
	}

	for (const auto& edge : j["edges"])
	{
		EdgeData e;
		e.from = edge["from"].get<int>();
		e.to = edge["to"].get<int>();
		mBlueprintData.edges.push_back(e);
	}

	return true;
}

const BlueprintData& BlueprintGraph::getBlueprintData() const
{
	return mBlueprintData;
}
