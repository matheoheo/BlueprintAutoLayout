#include "GraphRenderer.h"

namespace
{
	constexpr float NodeWidth = 200;
	constexpr float NodeHeight = 90;
	constexpr sf::Color NodeColor{ 70, 70, 70 };
	constexpr sf::Color OutlineColor{ sf::Color::White };
	constexpr float OutlineThickness = 2.f;
	constexpr sf::Color EdgeColor{ 150, 150, 150 };
	constexpr sf::Color CircleColor{ 200, 200, 200 };
	constexpr sf::Color EdgeTextColor{ 220, 220, 220 };

	constexpr sf::Color StartNodeColor{190, 50, 50};
	constexpr sf::Color EndNodeColor{50, 150, 50};
	constexpr sf::Color BranchNodeColor{ 220, 180, 20 };

	bool hasNodeAnEdgeName(const std::string& name)
	{
		return name.find('(') != std::string::npos;
	}

	std::pair<std::string, std::string> splitNodeName(const std::string& name)
	{
		size_t pos = name.find('(');
		if (pos == std::string::npos)
			return { name, "" };

		std::string nodeLabel = name.substr(0, pos);
		std::string edgeLabel = name.substr(pos + 1);
		while (!edgeLabel.empty() && edgeLabel.back() == ')')
			edgeLabel.pop_back();

		while (!nodeLabel.empty() && nodeLabel.back() == ' ')
			nodeLabel.pop_back();
		
		return { nodeLabel, edgeLabel };
	}
}

GraphRenderer::GraphRenderer(const std::deque<GraphNode>& nodes, sf::RenderWindow& window)
	:mNodes(nodes),
	mWindow(window),
	mCharSize(window.getSize().y / 36)
{
	if (!mFont.openFromFile("assets/interFont.ttf"))
		return;

	build();
}

void GraphRenderer::render() const
{
	for (const auto& edge : mVisualEdges)
	{
		mWindow.draw(edge.line);
		mWindow.draw(edge.circle);
		if (edge.edgeText)
			mWindow.draw(*edge.edgeText);
	}
	for (const auto& node : mVisualNodes)
	{
		mWindow.draw(node.nodeRect);
		mWindow.draw(node.nodeText);
	}
}

void GraphRenderer::createVisualNode(const GraphNode& source, const std::string& label)
{
	auto& node = mVisualNodes.emplace_back(mFont);
	node.source = &source;

	node.nodeRect.setSize(sf::Vector2f{ NodeWidth, NodeHeight });
	node.nodeRect.setOutlineColor(OutlineColor);
	node.nodeRect.setOutlineThickness(OutlineThickness);
	node.nodeRect.setPosition({ source.posX, source.posY });

	sf::Color finalColor = NodeColor;

	if (source.inNodes.empty())
		finalColor = StartNodeColor;
	else if (source.outNodes.empty())
		finalColor = EndNodeColor;
	else if (source.outNodes.size() > 1)
		finalColor = BranchNodeColor;

	node.nodeRect.setFillColor(finalColor);

	node.nodeText.setCharacterSize(mCharSize);
	node.nodeText.setString(label);
	node.nodeText.setOrigin(node.nodeText.getLocalBounds().position + node.nodeText.getGlobalBounds().size * 0.5f);
	node.nodeText.setPosition(node.nodeRect.getPosition() + node.nodeRect.getSize() * 0.5f);
}

void GraphRenderer::setupPath(VisualEdge& edge, const std::vector<sf::Vector2f>& path)
{
	if (path.empty())
		return;

	edge.line = sf::VertexArray(sf::PrimitiveType::LineStrip, path.size());
	for (size_t i = 0; i < path.size(); ++i)
	{
		edge.line[i].position = path[i];
		edge.line[i].color = EdgeColor;
	}

	constexpr float radius = 6.f;
	edge.circle.setRadius(radius);
	edge.circle.setFillColor(CircleColor);
	edge.circle.setOrigin({ radius, radius });
	edge.circle.setPosition(path.back());
}

void GraphRenderer::setupEdgeText(VisualEdge& edge, const std::vector<sf::Vector2f>& path, const std::string& label)
{
	if (path.size() < 2)
		return;

	edge.edgeText.emplace(mFont, label, static_cast<unsigned int>(mCharSize / 1.5f));
	edge.edgeText->setFillColor(EdgeTextColor);
	edge.edgeText->setOrigin(edge.edgeText->getLocalBounds().position + edge.edgeText->getGlobalBounds().size * 0.5f);

	size_t bestIdx = 0;
	float bestLen = 0.f;

	for (size_t i = 0; i + 1 < path.size(); ++i)
	{
		sf::Vector2f d = path[i + 1] - path[i];
		float l = d.x * d.x + d.y * d.y;

		if (l > bestLen)
		{
			bestLen = l;
			bestIdx = i;
		}
	}

	sf::Vector2f a = path[bestIdx];
	sf::Vector2f b = path[bestIdx + 1];

	float t = 0.4f + (bestIdx % 2) * 0.2f; 
	sf::Vector2f midPoint = a + (b - a) * t;

	sf::Vector2f dir = b - a;
	float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
	if (len < 0.001f)
		return;
	dir /= len;

	sf::Vector2f normal(-dir.y, dir.x);

	float textSize = edge.edgeText->getLocalBounds().size.y;
	float offset = std::clamp(len * 0.15f, textSize * 0.6f, textSize * 1.4f);

	sf::Vector2f finalPos = midPoint + normal * offset;
	edge.edgeText->setPosition(finalPos);
}

void GraphRenderer::build()
{
	buildNodes();
	buildEdges();
}

void GraphRenderer::buildNodes()
{
	for (const auto& node : mNodes)
	{
		if (node.isDummy)
			continue;

		if (!hasNodeAnEdgeName(node.data->name))
			createVisualNode(node, node.data->name);
		else
			createVisualNode(node, splitNodeName(node.data->name).first);
	}
}

void GraphRenderer::buildEdges()
{
	constexpr float margin = NodeWidth * 0.1f;

	for (const auto& sourceNode : mNodes)
	{
		if (sourceNode.isDummy)
			continue;

		for (const GraphNode* targetNode : sourceNode.outNodes)
		{
			auto& edge = mVisualEdges.emplace_back();
			std::vector<sf::Vector2f> path;

			sf::Vector2f start{
				sourceNode.posX + NodeWidth + margin,
				sourceNode.posY + NodeHeight * 0.5f
			};
			path.push_back(start);

			const GraphNode* curr = targetNode;

			while (curr->isDummy)
			{
				path.push_back({
					curr->posX,
					curr->posY + NodeHeight * 0.5f
					});

				curr = curr->outNodes[0];
			}

			sf::Vector2f end{
				curr->posX - margin,
				curr->posY + NodeHeight * 0.5f
			};
			path.push_back(end);

			setupPath(edge, path);
			
			if (!targetNode->isDummy &&  hasNodeAnEdgeName(targetNode->data->name))
				setupEdgeText(edge, path, splitNodeName(targetNode->data->name).second);
			
		}
	}
}
