#pragma once
#include <string>
#include <vector>

//Plain data read from .json file
struct NodeData
{
	int id = 0;
	std::string name;
	int x = 0;
	int y = 0;
};

struct EdgeData
{
	int from = 0;
	int to = 0;
};

struct BlueprintData
{
	std::vector<NodeData> nodes;
	std::vector<EdgeData> edges;
};