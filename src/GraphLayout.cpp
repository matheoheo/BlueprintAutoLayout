#include "GraphLayout.h"
#include <queue>

bool GraphLayout::create(const BlueprintData& data)
{
	clearAll();
	initalize(data);
	if (hasCycle())
		return false;
	assignLayers();
	insertDummyNodes();
	createLayers();
	orderInsideLayers();
	assignPositions();

	return true;
}

const std::deque<GraphNode>& GraphLayout::getGraphNodes() const
{
	return mGraphNodes;
}

void GraphLayout::initalize(const BlueprintData& data)
{
	//mGraphNodes.reserve(data.nodes.size());
	std::unordered_map<int, GraphNode*> nodesMap;

	for (const auto& node : data.nodes)
	{
		mGraphNodes.emplace_back(GraphNode{ .data = &node });
		nodesMap[node.id] = &mGraphNodes.back();
	}

	auto getNodeFromMap = [&](int id) -> GraphNode*
	{
		auto it = nodesMap.find(id);
		if (it != std::end(nodesMap))
			return it->second;

		return nullptr;
	};

	for (const auto& edge : data.edges)
	{
		GraphNode* fromNode = getNodeFromMap(edge.from);
		GraphNode* toNode = getNodeFromMap(edge.to);

		if (fromNode && toNode)
		{
			fromNode->outNodes.push_back(toNode);
			toNode->inNodes.push_back(fromNode);
		}
	}

}

void GraphLayout::assignLayers()
{
	std::queue<GraphNode*> q;

	for (auto& n : mGraphNodes)
	{
		if (n.inNodes.empty())
		{
			n.layer = 0;
			q.push(&n);
		}
	}

	while (!q.empty())
	{
		GraphNode* curr = q.front();
		q.pop();

		for (auto& n : curr->outNodes)
		{
			if (n->layer < curr->layer + 1)
			{
				n->layer = curr->layer + 1;
				q.push(n);
			}
		}
	}
}

void GraphLayout::insertDummyNodes()
{
	using NodePair = std::pair < GraphNode*, GraphNode*>;
	std::vector<NodePair> nodesToProcess;

	for (auto& node : mGraphNodes)
	{
		for (GraphNode* child : node.outNodes)
			nodesToProcess.emplace_back(&node, child);

		node.outNodes.clear();
		node.inNodes.clear();
	}

	for (auto& [from, to] : nodesToProcess)
	{
		int diff = to->layer - from->layer;
		if (diff <= 1)
		{
			from->outNodes.push_back(to);
			to->inNodes.push_back(from);
			continue;
		}

		GraphNode* prev = from;
		for (int i = 1; i < diff; ++i)
		{
			GraphNode* dummy = &mGraphNodes.emplace_back();
			dummy->isDummy = true;
			dummy->layer = from->layer + i;

			prev->outNodes.push_back(dummy);
			dummy->inNodes.push_back(prev);

			prev = dummy;
		}

		prev->outNodes.push_back(to);
		to->inNodes.push_back(prev);
	}
}

void GraphLayout::createLayers()
{
	int maxLayers = -1;
	for (const auto& node : mGraphNodes)
		maxLayers = std::max(node.layer, maxLayers);

	mLayers.resize(static_cast<size_t>(maxLayers + 1));

	for (auto& node : mGraphNodes)
	{
		mLayers[node.layer].push_back(&node);
		node.tmpIndex = static_cast<float>(mLayers[node.layer].size() - 1);
	}
}

void GraphLayout::orderInsideLayers()
{
	if (mLayers.size() < 2)
		return;

	auto order = [&](std::vector<GraphNode*>& layer, bool orderFromLeft)
	{
		for (GraphNode* node : layer)
		{
			auto& vec = orderFromLeft ? node->inNodes : node->outNodes;
			if (vec.empty())
				continue;

			float sum = 0.f;
			for (GraphNode* pc : vec)
				sum += pc->tmpIndex;

			node->tmpIndex = sum / static_cast<float>(vec.size());
		}
		std::ranges::sort(layer, std::less<>{}, & GraphNode::tmpIndex);
		for (int j = 0; j < layer.size(); ++j)
			layer[j]->tmpIndex = static_cast<float>(j);
	};

	for (int i = 0; i < 4; ++i)
	{
		//sort from left to right
		for (size_t i = 1; i < mLayers.size(); ++i)
			order(mLayers[i], true);

		//smoothen from right to left
		for (int i = static_cast<int>(mLayers.size()) - 2; i >= 0; --i)
			order(mLayers[i], false);
	}
}

void GraphLayout::assignPositions()
{
	constexpr float baseX = 350;
	constexpr float baseY = 230;

	constexpr float startX = 50;
	constexpr float startY = 50;

	size_t maxLayerSize = 0;
	for (auto& layer : mLayers)
		maxLayerSize = std::max(maxLayerSize, layer.size());

	for (size_t layerIndex = 0; layerIndex < mLayers.size(); ++layerIndex)
	{
		auto& layer = mLayers[layerIndex];
		float offsetY = (maxLayerSize - layer.size()) * 0.5f * baseY;

		for (GraphNode* node : layer)
		{
			node->posX = startX + baseX * layerIndex;
			node->posY = startY+ offsetY + baseY * node->tmpIndex;
		}
	}
}

bool GraphLayout::hasCycle() 
{
	auto dfsHasCycle = [](const auto& self, GraphNode* node) -> bool
	{
		if (node->state == GraphNode::VisitState::Gray)
			return true;

		if (node->state == GraphNode::VisitState::Black)
			return false;

		node->state = GraphNode::VisitState::Gray;
		for (GraphNode* childNode : node->outNodes)
		{
			if (self(self, childNode))
				return true;
		}
		node->state = GraphNode::VisitState::Black;
		return false;
	};

	for (auto& node : mGraphNodes)
	{
		if (node.state == GraphNode::VisitState::White)
		{
			if (dfsHasCycle(dfsHasCycle, &node))
				return true;
		}
	}
	return false;
}

void GraphLayout::clearAll()
{
	mGraphNodes.clear();
	mLayers.clear();
}

