#pragma once
#include "BlueprintData.h"

struct GraphNode
{
	const NodeData* data = nullptr;

	std::vector<GraphNode*> inNodes;
	std::vector<GraphNode*> outNodes;

	int layer = -1;
	float posX = 0.f;
	float posY = 0.f;

	float tmpIndex = 0.f;
	bool isDummy = false;

	//To find cycles
	enum class VisitState
	{
		Gray,
		Black,
		White
	};

	VisitState state{ VisitState::White }; 
};