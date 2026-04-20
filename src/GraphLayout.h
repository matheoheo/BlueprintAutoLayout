#pragma once
#include "BlueprintData.h"
#include "GraphNode.h"
#include <unordered_map>
#include <deque>

class GraphLayout
{
public:
	bool create(const BlueprintData& data);

	const std::deque<GraphNode>& getGraphNodes() const;
private:
	void initalize(const BlueprintData& data);
	void assignLayers();
	void insertDummyNodes();
	void createLayers();
	void orderInsideLayers();
	void assignPositions();

	bool hasCycle();
	void clearAll();
private:
	std::deque<GraphNode> mGraphNodes;
	std::vector<std::vector<GraphNode*>> mLayers;
};