#pragma once
#include "GraphLayout.h"
#include <SFML/Graphics.hpp>

class GraphRenderer
{
public:
	GraphRenderer(const std::deque<GraphNode>& nodes, sf::RenderWindow& window);
	void render() const;
private:
	struct VisualNode
	{
		const GraphNode* source = nullptr;
		sf::RectangleShape nodeRect;
		sf::Text nodeText;

		VisualNode(const sf::Font& font)
			:nodeText(font) {}
	};

	struct VisualEdge
	{
		sf::VertexArray line;
		sf::CircleShape circle;
		std::optional<sf::Text> edgeText;
	};
	void createVisualNode(const GraphNode& source, const std::string& label);
	void setupPath(VisualEdge& edge, const std::vector<sf::Vector2f>& path);
	void setupEdgeText(VisualEdge& edge, const std::vector<sf::Vector2f>& path, const std::string& label);

	void build();
	void buildNodes();
	void buildEdges();
private:
	const std::deque<GraphNode>& mNodes;
	sf::RenderWindow& mWindow;
	sf::Font mFont;
	unsigned int mCharSize;
	std::vector<VisualNode> mVisualNodes;
	std::vector<VisualEdge> mVisualEdges;
};